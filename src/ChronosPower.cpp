#include "ChronosPower.h"
#include <math.h>
#include <algorithm>
#include <cstring>

namespace ChronosPower {


std::unique_ptr<ChronosPowerManager> g_powerManager;


static const char* STATUS_STRINGS[] = {
    "OK",
    "FAILURE_GENERIC",
    "FAILURE_UNSUPPORTED_ARCH",
    "FAILURE_UNSUPPORTED_FEATURE",
    "FAILURE_INVALID_PARAMETER",
    "FAILURE_TIMEOUT",
    "FAILURE_INSUFFICIENT_MEMORY",
    "FAILURE_SYSTEM_STATE_INVALID",
    "FAILURE_CONFIG_LOCKED",
    "FAILURE_PERIPHERAL_ACTIVE",
    "FAILURE_REGISTER_WRITE_PROTECTED",
    "FAILURE_HARDWARE_FAULT",
    "FAILURE_INVALID_FREQUENCY",
    "FAILURE_PLL_LOCK_FAILED",
    "FAILURE_OSCILLATOR_FAILED",
    "FAILURE_CLOCK_GATING_FAILED",
    "FAILURE_PRECISION_UNATTAINABLE",
    "FAILURE_VOLTAGE_SCALE",
    "FAILURE_FLASH_POWERDOWN",
    "FAILURE_BOD_CONFIG",
    "FAILURE_THERMAL_PROTECTION",
    "FAILURE_POWER_DOMAIN_LOCKED",
    "FAILURE_NO_WAKEUP_SOURCE",
    "FAILURE_WAKEUP_CONFLICT",
    "FAILURE_RTC_UNINITIALIZED",
    "FAILURE_TIMER_UNAVAILABLE",
    "FAILURE_INTERRUPT_MAP",
    "FAILURE_PERIPHERAL_NOT_FOUND",
    "FAILURE_DMA_CHANNEL_ACTIVE",
    "FAILURE_CALLBACK_NOT_SET",
    "FAILURE_ADAPTIVE_SCALING_DISABLED",
    "FAILURE_ENERGY_BUDGET_EXCEEDED",
    "FAILURE_THERMAL_THROTTLING_ACTIVE",
    "FAILURE_BATTERY_CRITICAL",
    "FAILURE_SECURITY_VIOLATION"
};

ChronosPowerManager::ChronosPowerManager() :
    m_currentProfile(PowerProfile::ADAPTIVE_BATTERY_LIFE),
    m_lastWakeupReason(WakeupReason::POWER_ON_RESET),
    m_lastResetReason(ResetReason::UNKNOWN),
    m_preSleepMillis(0),
    m_postWakeupMillis(0),
    m_totalSleepTime(0),
    m_totalUptime(0),
    m_isInitialized(false),
    m_logLevel(LogLevel::INFO),
    m_adaptiveScalingEnabled(false),
    m_lastAdaptiveUpdate(0),
    m_energyConsumedToday(0.0f),
    m_lastEnergyUpdate(0),
    m_hasPendingChanges(false)
{
    
    memset(&m_energyMetrics, 0, sizeof(m_energyMetrics));
    memset(&m_thermalMetrics, 0, sizeof(m_thermalMetrics));
    memset(&m_performanceMetrics, 0, sizeof(m_performanceMetrics));
    
    
    m_energyMetrics.batteryPercentage = 100.0f;
    m_thermalMetrics.currentTemperatureCelsius = 25.0f;
    m_thermalMetrics.maxTemperatureCelsius = 25.0f;
    m_thermalMetrics.averageTemperatureCelsius = 25.0f;
    
    
    for(int i = 0; i < 8; ++i) {
        m_customPolicy.peripheralMask[i] = 0;
        m_pendingPolicy.peripheralMask[i] = 0;
    }
    
    logMessage(LogLevel::INFO, "ChronosPower v2.5 initialized");
}

ChronosPowerManager::~ChronosPowerManager() {
    logMessage(LogLevel::INFO, "ChronosPower v2.5 shutting down");
}


ChronosPowerManager& ChronosPowerManager::withProfile(PowerProfile profile) {
    m_pendingPolicy = PowerPolicy(); 
    applyProfile(profile); 
    m_hasPendingChanges = true;
    return *this;
}

ChronosPowerManager& ChronosPowerManager::withFrequency(uint32_t frequencyHz) {
    m_pendingPolicy.cpuFrequencyHz = frequencyHz;
    m_hasPendingChanges = true;
    return *this;
}

ChronosPowerManager& ChronosPowerManager::withPeripheral(Peripheral p, bool enable) {
    uint16_t val = static_cast<uint16_t>(p);
    uint8_t bank = val / 64;
    uint8_t bit = val % 64;
    
    if (bank < 8) {
        if (enable) {
            m_pendingPolicy.peripheralMask[bank] |= (1ULL << bit);
        } else {
            m_pendingPolicy.peripheralMask[bank] &= ~(1ULL << bit);
        }
        m_hasPendingChanges = true;
    }
    return *this;
}

ChronosPowerManager& ChronosPowerManager::withDomain(PowerDomain domain, bool enable) {
    auto peripherals = getPeripheralsInDomain(domain);
    for (auto p : peripherals) {
        withPeripheral(p, enable);
    }
    return *this;
}

ChronosPowerManager& ChronosPowerManager::withWakeupSource(WakeupReason reason) {
    
    
    m_hasPendingChanges = true;
    return *this;
}

ChronosPowerManager& ChronosPowerManager::withEnergyBudget(float dailyBudgetWh) {
    m_pendingPolicy.energyBudget.dailyBudgetMicroWattHours = dailyBudgetWh * 1000000.0f;
    m_pendingPolicy.energyBudget.hourlyBudgetMicroWattHours = dailyBudgetWh * 1000000.0f / 24.0f;
    m_hasPendingChanges = true;
    return *this;
}

ChronosPowerManager& ChronosPowerManager::withThermalLimit(float maxTempCelsius) {
    m_pendingPolicy.adaptiveScaling.temperatureThresholdCelsius = maxTempCelsius;
    m_hasPendingChanges = true;
    return *this;
}

ChronosPowerManager& ChronosPowerManager::withAdaptiveScaling(bool enable) {
    m_pendingPolicy.adaptiveScaling.enabled = enable;
    m_hasPendingChanges = true;
    return *this;
}

ChronosPowerManager& ChronosPowerManager::withLogging(LogLevel level) {
    m_logLevel = level;
    return *this;
}

Status ChronosPowerManager::begin(PowerProfile initialProfile) {
    if (m_isInitialized) {
        logMessage(LogLevel::WARNING, "Already initialized");
        return Status::OK;
    }
    
    m_isInitialized = true;
    m_lastWakeupReason = platform_getWakeupReason();
    m_lastResetReason = platform_getResetReason();
    
    logMessage(LogLevel::INFO, "Beginning initialization");
    
    Status result = setProfile(initialProfile);
    if (result != Status::OK) {
        logMessage(LogLevel::ERROR, "Failed to set initial profile", result);
        return result;
    }
    
    
    updateMetrics();
    
    logMessage(LogLevel::INFO, "Initialization complete");
    return Status::OK;
}

Status ChronosPowerManager::apply() {
    if (!m_hasPendingChanges) {
        return Status::OK;
    }
    
    Status result = setCustomPolicy(m_pendingPolicy);
    if (result == Status::OK) {
        m_hasPendingChanges = false;
        logMessage(LogLevel::INFO, "Applied pending configuration");
    } else {
        logMessage(LogLevel::ERROR, "Failed to apply pending configuration", result);
    }
    
    return result;
}

Status ChronosPowerManager::setProfile(PowerProfile profile) {
    if (!m_isInitialized) {
        logMessage(LogLevel::ERROR, "Not initialized", Status::FAILURE_SYSTEM_STATE_INVALID);
        return Status::FAILURE_SYSTEM_STATE_INVALID;
    }
    
    m_currentProfile = profile;
    Status result = applyProfile(profile);
    
    if (result == Status::OK) {
        logMessage(LogLevel::INFO, "Profile set successfully");
    } else {
        logMessage(LogLevel::ERROR, "Failed to set profile", result);
    }
    
    return result;
}

PowerProfile ChronosPowerManager::getProfile() const {
    return m_currentProfile;
}

Status ChronosPowerManager::setCustomPolicy(const PowerPolicy& policy) {
    if (!m_isInitialized) {
        return Status::FAILURE_SYSTEM_STATE_INVALID;
    }
    
    m_customPolicy = policy;
    m_currentProfile = PowerProfile::CUSTOM_POLICY;
    
    applyPolicy(m_customPolicy);
    
    
    if (policy.energyBudget.dailyBudgetMicroWattHours > 0) {
        m_energyBudget = policy.energyBudget;
    }
    
    
    if (policy.adaptiveScaling.enabled) {
        m_adaptiveConfig = policy.adaptiveScaling;
        m_adaptiveScalingEnabled = true;
    }
    
    logMessage(LogLevel::INFO, "Custom policy applied");
    return Status::OK;
}

Status ChronosPowerManager::getCustomPolicy(PowerPolicy& policy) const {
    policy = m_customPolicy;
    return Status::OK;
}

Status ChronosPowerManager::sleep(const WakeupPolicy& policy) {
    if (!m_isInitialized) {
        return Status::FAILURE_SYSTEM_STATE_INVALID;
    }
    
    
    bool hasWakeupSource = (policy.wdt.timeoutMillis > 0) ||
                          (policy.rtc.timeoutSeconds > 0) ||
                          (policy.extInterrupt.pinMask > 0) ||
                          (policy.touchThreshold > 0) ||
                          policy.enableUlpWakeup ||
                          policy.enableWifiWakeup ||
                          policy.enableBluetoothWakeup ||
                          policy.enableAnalogComparatorWakeup ||
                          policy.enableCanWakeup ||
                          policy.enableUsbWakeup;
    
    if (!hasWakeupSource) {
        logMessage(LogLevel::ERROR, "No wakeup source specified", Status::FAILURE_NO_WAKEUP_SOURCE);
        return Status::FAILURE_NO_WAKEUP_SOURCE;
    }
    
    
    if (m_wakeupCallback) {
        m_wakeupCallback(WakeupReason::UNKNOWN);
    }
    
    
    updateMetrics();
    updateEnergyBudget();
    
    m_preSleepMillis = millis();
    
    logMessage(LogLevel::DEBUG, "Entering sleep mode");
    
    
    platform_sleep(policy);
    
    m_postWakeupMillis = millis();
    m_lastWakeupReason = platform_getWakeupReason();
    
    
    uint32_t sleepDuration = m_postWakeupMillis - m_preSleepMillis;
    m_totalSleepTime += sleepDuration;
    
    
    synchronizeSystemTime();
    
    
    updateMetrics();
    
    
    if (m_wakeupCallback) {
        m_wakeupCallback(m_lastWakeupReason);
    }
    
    logMessage(LogLevel::DEBUG, "Woke up from sleep");
    
    return Status::OK;
}

Status ChronosPowerManager::sleep(uint32_t durationMs) {
    WakeupPolicy policy;
    policy.wdt.timeoutMillis = durationMs;
    policy.wdt.generateInterrupt = true;
    return sleep(policy);
}

Status ChronosPowerManager::sleepUntil(WakeupReason reason) {
    WakeupPolicy policy;
    
    
    switch (reason) {
        case WakeupReason::EXTERNAL_PIN_0:
            policy.extInterrupt.pinMask = (1ULL << 0);
            policy.extInterrupt.triggerMode = FALLING;
            break;
        case WakeupReason::RTC_ALARM_SECONDS:
            policy.rtc.timeoutSeconds = 60; 
            break;
        case WakeupReason::TOUCH_SENSOR_GLOBAL:
            policy.touchThreshold = 40;
            break;
        default:
            return Status::FAILURE_UNSUPPORTED_FEATURE;
    }
    
    return sleep(policy);
}

[[noreturn]] void ChronosPowerManager::hibernate(const WakeupPolicy& policy) {
    if (!m_isInitialized) {
        for(;;); 
    }
    
    logMessage(LogLevel::INFO, "Entering hibernation");
    
    
    applyProfile(PowerProfile::HIBERNATION_PREP);
    
    
    if (m_wakeupCallback) {
        m_wakeupCallback(WakeupReason::UNKNOWN);
    }
    
    
    platform_hibernate(policy);
}

[[noreturn]] void ChronosPowerManager::shutdown() {
    if (!m_isInitialized) {
        for(;;);
    }
    
    logMessage(LogLevel::INFO, "Shutting down");
    
    
    applyProfile(PowerProfile::SHUTDOWN_PREP);
    
    
    WakeupPolicy policy;
    platform_hibernate(policy);
}

Status ChronosPowerManager::setCpuFrequency(uint32_t frequencyHz) {
    if (!m_isInitialized) {
        return Status::FAILURE_SYSTEM_STATE_INVALID;
    }
    
    Status result = platform_setCpuFrequency(frequencyHz);
    
    if (result == Status::OK) {
        m_customPolicy.cpuFrequencyHz = frequencyHz;
        logMessage(LogLevel::DEBUG, "CPU frequency changed");
        updateMetrics();
    } else {
        logMessage(LogLevel::ERROR, "Failed to change CPU frequency", result);
    }
    
    return result;
}

uint32_t ChronosPowerManager::getCpuFrequency() const {
    if (!m_isInitialized) return 0;
    return platform_getCpuFrequency();
}

Status ChronosPowerManager::control(Peripheral p, bool enable) {
    if (!m_isInitialized) {
        return Status::FAILURE_SYSTEM_STATE_INVALID;
    }
    
    Status result = platform_control(p, enable);
    
    if (result == Status::OK) {
        
        uint16_t val = static_cast<uint16_t>(p);
        uint8_t bank = val / 64;
        uint8_t bit = val % 64;
        
        if (bank < 8) {
            if (enable) {
                m_customPolicy.peripheralMask[bank] |= (1ULL << bit);
            } else {
                m_customPolicy.peripheralMask[bank] &= ~(1ULL << bit);
            }
        }
        
        logMessage(LogLevel::DEBUG, enable ? "Peripheral enabled" : "Peripheral disabled");
    } else {
        logMessage(LogLevel::ERROR, "Failed to control peripheral", result);
    }
    
    return result;
}

Status ChronosPowerManager::massControl(const std::vector<Peripheral>& peripherals, bool enable) {
    Status result = Status::OK;
    
    for (auto p : peripherals) {
        Status singleResult = control(p, enable);
        if (singleResult != Status::OK) {
            result = singleResult; 
        }
    }
    
    return result;
}

Status ChronosPowerManager::massControl(const uint64_t masks[], uint8_t maskCount, bool enable) {
    if (!m_isInitialized) {
        return Status::FAILURE_SYSTEM_STATE_INVALID;
    }
    
    for (uint8_t i = 0; i < maskCount && i < 8; ++i) {
        for (uint8_t j = 0; j < 64; ++j) {
            if ((masks[i] >> j) & 1) {
                Peripheral p = static_cast<Peripheral>(i * 64 + j);
                platform_control(p, enable);
            }
        }
    }
    
    logMessage(LogLevel::DEBUG, "Mass peripheral control completed");
    return Status::OK;
}

Status ChronosPowerManager::setClockGatingForDomain(PowerDomain domain, bool enable) {
    auto peripherals = getPeripheralsInDomain(domain);
    return massControl(peripherals, enable);
}

Status ChronosPowerManager::enableAdaptiveScaling(const AdaptiveScalingConfig& config) {
    m_adaptiveConfig = config;
    m_adaptiveScalingEnabled = true;
    m_lastAdaptiveUpdate = millis();
    
    logMessage(LogLevel::INFO, "Adaptive scaling enabled");
    return Status::OK;
}

Status ChronosPowerManager::disableAdaptiveScaling() {
    m_adaptiveScalingEnabled = false;
    logMessage(LogLevel::INFO, "Adaptive scaling disabled");
    return Status::OK;
}

Status ChronosPowerManager::setEnergyBudget(const EnergyBudget& budget) {
    m_energyBudget = budget;
    logMessage(LogLevel::INFO, "Energy budget updated");
    return Status::OK;
}

Status ChronosPowerManager::optimizeForBatteryLife() {
    return setProfile(PowerProfile::ADAPTIVE_BATTERY_LIFE);
}

Status ChronosPowerManager::optimizeForPerformance() {
    return setProfile(PowerProfile::ADAPTIVE_PERFORMANCE);
}

Status ChronosPowerManager::optimizeForThermal() {
    return setProfile(PowerProfile::ADAPTIVE_THERMAL);
}


EnergyMetrics ChronosPowerManager::getEnergyMetrics() const {
    return m_energyMetrics;
}

ThermalMetrics ChronosPowerManager::getThermalMetrics() const {
    return m_thermalMetrics;
}

PerformanceMetrics ChronosPowerManager::getPerformanceMetrics() const {
    return m_performanceMetrics;
}

Status ChronosPowerManager::getInternalTemperature(float& temp) {
    Status result = platform_getInternalTemperature(temp);
    if (result == Status::OK) {
        m_thermalMetrics.currentTemperatureCelsius = temp;
        if (temp > m_thermalMetrics.maxTemperatureCelsius) {
            m_thermalMetrics.maxTemperatureCelsius = temp;
        }
    }
    return result;
}

Status ChronosPowerManager::getCoreVoltage(float& voltage) {
    return platform_getCoreVoltage(voltage);
}

Status ChronosPowerManager::getFlashState(FlashState& state) {
    return platform_getFlashState(state);
}

Status ChronosPowerManager::getBatteryLevel(float& percentage) {
    return platform_getBatteryLevel(percentage);
}


Status ChronosPowerManager::registerWakeupCallback(WakeupCallback callback) {
    m_wakeupCallback = callback;
    logMessage(LogLevel::DEBUG, "Wakeup callback registered");
    return Status::OK;
}

Status ChronosPowerManager::registerErrorCallback(ErrorCallback callback) {
    m_errorCallback = callback;
    logMessage(LogLevel::DEBUG, "Error callback registered");
    return Status::OK;
}

Status ChronosPowerManager::registerEnergyCallback(EnergyCallback callback) {
    m_energyCallback = callback;
    logMessage(LogLevel::DEBUG, "Energy callback registered");
    return Status::OK;
}

Status ChronosPowerManager::registerThermalCallback(ThermalCallback callback) {
    m_thermalCallback = callback;
    logMessage(LogLevel::DEBUG, "Thermal callback registered");
    return Status::OK;
}

Status ChronosPowerManager::registerPerformanceCallback(PerformanceCallback callback) {
    m_performanceCallback = callback;
    logMessage(LogLevel::DEBUG, "Performance callback registered");
    return Status::OK;
}


WakeupReason ChronosPowerManager::getWakeupReason() {
    return m_lastWakeupReason;
}

ResetReason ChronosPowerManager::getResetReason() {
    return m_lastResetReason;
}

void ChronosPowerManager::clearWakeupReason() {
    m_lastWakeupReason = WakeupReason::UNKNOWN;
}

uint32_t ChronosPowerManager::getSleepDurationMillis() const {
    return m_postWakeupMillis - m_preSleepMillis;
}

uint32_t ChronosPowerManager::getTotalSleepTime() const {
    return m_totalSleepTime;
}

uint32_t ChronosPowerManager::getTotalUptime() const {
    return millis() - m_totalSleepTime;
}


void ChronosPowerManager::synchronizeSystemTime() {
    uint32_t sleepDuration = getSleepDurationMillis();
    platform_synchronizeSystemTime(sleepDuration);
}

Status ChronosPowerManager::calibrateInternalOscillator(SystemClockSource osc, uint32_t referenceFreq) {
    
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

Status ChronosPowerManager::setSystemClockSource(SystemClockSource source) {
    return platform_setSystemClockSource(source);
}


Status ChronosPowerManager::setLogLevel(LogLevel level) {
    m_logLevel = level;
    logMessage(LogLevel::INFO, "Log level changed");
    return Status::OK;
}

LogLevel ChronosPowerManager::getLogLevel() const {
    return m_logLevel;
}

std::vector<LogEntry> ChronosPowerManager::getLogEntries(uint32_t maxEntries) const {
    std::vector<LogEntry> result;
    uint32_t count = std::min(maxEntries, static_cast<uint32_t>(m_logEntries.size()));
    
    if (count > 0) {
        auto start = m_logEntries.end() - count;
        result.assign(start, m_logEntries.end());
    }
    
    return result;
}

void ChronosPowerManager::clearLog() {
    m_logEntries.clear();
    logMessage(LogLevel::INFO, "Log cleared");
}

Status ChronosPowerManager::enablePerformanceMonitoring(bool enable) {
    
    logMessage(LogLevel::INFO, enable ? "Performance monitoring enabled" : "Performance monitoring disabled");
    return Status::OK;
}


Status ChronosPowerManager::createPeripheralGroup(const std::string& name, const std::vector<Peripheral>& peripherals) {
    m_peripheralGroups[name] = peripherals;
    logMessage(LogLevel::DEBUG, "Peripheral group created");
    return Status::OK;
}

Status ChronosPowerManager::enablePeripheralGroup(const std::string& name) {
    auto it = m_peripheralGroups.find(name);
    if (it == m_peripheralGroups.end()) {
        return Status::FAILURE_INVALID_PARAMETER;
    }
    
    return massControl(it->second, true);
}

Status ChronosPowerManager::disablePeripheralGroup(const std::string& name) {
    auto it = m_peripheralGroups.find(name);
    if (it == m_peripheralGroups.end()) {
        return Status::FAILURE_INVALID_PARAMETER;
    }
    
    return massControl(it->second, false);
}

Status ChronosPowerManager::saveConfiguration(const std::string& name) {
    m_savedConfigurations[name] = m_customPolicy;
    logMessage(LogLevel::DEBUG, "Configuration saved");
    return Status::OK;
}

Status ChronosPowerManager::loadConfiguration(const std::string& name) {
    auto it = m_savedConfigurations.find(name);
    if (it == m_savedConfigurations.end()) {
        return Status::FAILURE_INVALID_PARAMETER;
    }
    
    return setCustomPolicy(it->second);
}


Status ChronosPowerManager::setPlatformSpecificOption(const std::string& option, const std::string& value) {
    return platform_setPlatformOption(option, value);
}

std::string ChronosPowerManager::getPlatformSpecificOption(const std::string& option) const {
    return platform_getPlatformOption(option);
}


const char* ChronosPowerManager::statusToString(Status status) {
    int index = -static_cast<int>(status);
    if (index >= 0 && index < sizeof(STATUS_STRINGS) / sizeof(STATUS_STRINGS[0])) {
        return STATUS_STRINGS[index];
    }
    return "UNKNOWN_STATUS";
}

const char* ChronosPowerManager::profileToString(PowerProfile profile) {
    switch (profile) {
        case PowerProfile::MAX_PERFORMANCE: return "MAX_PERFORMANCE";
        case PowerProfile::HIGH_PERFORMANCE: return "HIGH_PERFORMANCE";
        case PowerProfile::BALANCED_DESKTOP: return "BALANCED_DESKTOP";
        case PowerProfile::BALANCED_MOBILE: return "BALANCED_MOBILE";
        case PowerProfile::LOW_POWER_ACTIVE: return "LOW_POWER_ACTIVE";
        case PowerProfile::ULTRA_LOW_POWER: return "ULTRA_LOW_POWER";
        case PowerProfile::DEEP_SLEEP_PREP: return "DEEP_SLEEP_PREP";
        case PowerProfile::HIBERNATION_PREP: return "HIBERNATION_PREP";
        case PowerProfile::SHUTDOWN_PREP: return "SHUTDOWN_PREP";
        case PowerProfile::MACHINE_LEARNING_INFERENCE: return "MACHINE_LEARNING_INFERENCE";
        case PowerProfile::AUDIO_PROCESSING: return "AUDIO_PROCESSING";
        case PowerProfile::VIDEO_PROCESSING: return "VIDEO_PROCESSING";
        case PowerProfile::DATA_LOGGING: return "DATA_LOGGING";
        case PowerProfile::SENSOR_FUSION: return "SENSOR_FUSION";
        case PowerProfile::COMMUNICATION_INTENSIVE: return "COMMUNICATION_INTENSIVE";
        case PowerProfile::COMPUTATION_INTENSIVE: return "COMPUTATION_INTENSIVE";
        case PowerProfile::ADAPTIVE_PERFORMANCE: return "ADAPTIVE_PERFORMANCE";
        case PowerProfile::ADAPTIVE_BATTERY_LIFE: return "ADAPTIVE_BATTERY_LIFE";
        case PowerProfile::ADAPTIVE_THERMAL: return "ADAPTIVE_THERMAL";
        case PowerProfile::ADAPTIVE_WORKLOAD: return "ADAPTIVE_WORKLOAD";
        case PowerProfile::CUSTOM_POLICY: return "CUSTOM_POLICY";
        default: return "UNKNOWN_PROFILE";
    }
}

const char* ChronosPowerManager::peripheralToString(Peripheral peripheral) {
    
    switch (peripheral) {
        case Peripheral::SYSTEM_CONTROLLER: return "SYSTEM_CONTROLLER";
        case Peripheral::CPU_CORE_0: return "CPU_CORE_0";
        case Peripheral::UART_0: return "UART_0";
        case Peripheral::SPI_0: return "SPI_0";
        case Peripheral::I2C_0: return "I2C_0";
        case Peripheral::ADC_0_12BIT: return "ADC_0_12BIT";
        case Peripheral::TIMER_0_8BIT_BASIC: return "TIMER_0_8BIT_BASIC";
        
        default: return "UNKNOWN_PERIPHERAL";
    }
}

const char* ChronosPowerManager::wakeupReasonToString(WakeupReason reason) {
    switch (reason) {
        case WakeupReason::UNKNOWN: return "UNKNOWN";
        case WakeupReason::POWER_ON_RESET: return "POWER_ON_RESET";
        case WakeupReason::EXTERNAL_PIN_0: return "EXTERNAL_PIN_0";
        case WakeupReason::RTC_ALARM_SECONDS: return "RTC_ALARM_SECONDS";
        case WakeupReason::WATCHDOG_TIMEOUT_INTERRUPT: return "WATCHDOG_TIMEOUT_INTERRUPT";
        case WakeupReason::TOUCH_SENSOR_GLOBAL: return "TOUCH_SENSOR_GLOBAL";
        case WakeupReason::UART_RX_ACTIVITY_0: return "UART_RX_ACTIVITY_0";
        case WakeupReason::WIFI_EVENT: return "WIFI_EVENT";
        case WakeupReason::BLUETOOTH_LE_CONNECTION: return "BLUETOOTH_LE_CONNECTION";
        
        default: return "UNKNOWN_WAKEUP_REASON";
    }
}

std::vector<Peripheral> ChronosPowerManager::getPeripheralsInDomain(PowerDomain domain) {
    std::vector<Peripheral> peripherals;
    
    switch (domain) {
        case PowerDomain::COMMUNICATIONS:
            peripherals = {
                Peripheral::UART_0, Peripheral::UART_1, Peripheral::UART_2,
                Peripheral::SPI_0, Peripheral::SPI_1, Peripheral::SPI_2,
                Peripheral::I2C_0, Peripheral::I2C_1, Peripheral::I2C_2,
                Peripheral::I2S_0_STD, Peripheral::I2S_1_STD,
                Peripheral::CAN_BUS_0, Peripheral::CAN_BUS_1,
                Peripheral::USB_DEVICE, Peripheral::USB_HOST,
                Peripheral::ETHERNET_MAC
            };
            break;
            
        case PowerDomain::TIMERS_AND_PWM:
            peripherals = {
                Peripheral::TIMER_0_8BIT_BASIC, Peripheral::TIMER_1_16BIT_GENERAL,
                Peripheral::TIMER_2_8BIT_ASYNC, Peripheral::TIMER_3_16BIT_GENERAL,
                Peripheral::PWM_MODULE_0, Peripheral::PWM_MODULE_1,
                Peripheral::PWM_MODULE_2_MOTOR_CONTROL
            };
            break;
            
        case PowerDomain::ANALOG_SUBSYSTEM:
            peripherals = {
                Peripheral::ADC_0_12BIT, Peripheral::ADC_1_12BIT, Peripheral::ADC_2_16BIT,
                Peripheral::DAC_0_8BIT, Peripheral::DAC_1_12BIT,
                Peripheral::ANALOG_COMPARATOR_0, Peripheral::ANALOG_COMPARATOR_1,
                Peripheral::OPAMP_0, Peripheral::OPAMP_1
            };
            break;
            
        case PowerDomain::MEMORY_SYSTEM:
            peripherals = {
                Peripheral::FLASH_CONTROLLER, Peripheral::EEPROM_CONTROLLER,
                Peripheral::SRAM_CONTROLLER, Peripheral::CACHE_CONTROLLER,
                Peripheral::SRAM_BANK_0, Peripheral::SRAM_BANK_1,
                Peripheral::FLASH_CACHE_INSTRUCTION, Peripheral::FLASH_CACHE_DATA
            };
            break;
            
        case PowerDomain::GPIO_PORTS:
            peripherals = {
                Peripheral::GPIO_PORT_A, Peripheral::GPIO_PORT_B,
                Peripheral::GPIO_PORT_C, Peripheral::GPIO_PORT_D
            };
            break;
            
        case PowerDomain::SECURITY_MODULES:
            peripherals = {
                Peripheral::AES_ACCELERATOR_128, Peripheral::AES_ACCELERATOR_256,
                Peripheral::SHA_ACCELERATOR_256, Peripheral::TRNG_UNIT,
                Peripheral::PKA_UNIT, Peripheral::RSA_ACCELERATOR
            };
            break;
            
        case PowerDomain::MULTIMEDIA:
            peripherals = {
                Peripheral::LCD_CONTROLLER_PARALLEL, Peripheral::CAMERA_INTERFACE_DCMI,
                Peripheral::JPEG_CODEC, Peripheral::GPU_2D, Peripheral::GPU_3D,
                Peripheral::AUDIO_CODEC, Peripheral::AUDIO_DSP
            };
            break;
            
        case PowerDomain::RADIO_SUBSYSTEM:
            peripherals = {
                Peripheral::WIFI_MAC_2_4GHZ, Peripheral::WIFI_PHY_2_4GHZ,
                Peripheral::BLUETOOTH_LE_PHY, Peripheral::BLUETOOTH_CONTROLLER,
                Peripheral::RADIO_802_15_4, Peripheral::RADIO_LORA
            };
            break;
            
        default:
            break;
    }
    
    return peripherals;
}

bool ChronosPowerManager::isPeripheralAvailable(Peripheral p) {
    
    
    uint16_t val = static_cast<uint16_t>(p);
    return val < static_cast<uint16_t>(Peripheral::PERIPHERAL_COUNT);
}

uint32_t ChronosPowerManager::estimateEnergyConsumption(PowerProfile profile, uint32_t durationMs) {
    
    uint32_t baseConsumption = 0;
    
    switch (profile) {
        case PowerProfile::MAX_PERFORMANCE:
            baseConsumption = 100000; 
            break;
        case PowerProfile::HIGH_PERFORMANCE:
            baseConsumption = 50000;  
            break;
        case PowerProfile::BALANCED_MOBILE:
            baseConsumption = 20000;  
            break;
        case PowerProfile::LOW_POWER_ACTIVE:
            baseConsumption = 5000;   
            break;
        case PowerProfile::ULTRA_LOW_POWER:
            baseConsumption = 100;    
            break;
        default:
            baseConsumption = 10000;  
            break;
    }
    
    return (baseConsumption * durationMs) / 3600; 
}


Status ChronosPowerManager::applyProfile(PowerProfile profile) {
    PowerPolicy policy;
    
    
    for(int i = 0; i < 8; ++i) {
        policy.peripheralMask[i] = 0;
    }
    
    auto setPeripheralMask = [&](Peripheral p) {
        uint16_t val = static_cast<uint16_t>(p);
        if (val / 64 < 8) {
            policy.peripheralMask[val / 64] |= (1ULL << (val % 64));
        }
    };
    
    
    setPeripheralMask(Peripheral::SYSTEM_CONTROLLER);
    setPeripheralMask(Peripheral::CPU_CORE_0);
    setPeripheralMask(Peripheral::SRAM_BANK_0);
    setPeripheralMask(Peripheral::FLASH_CONTROLLER);
    
    switch(profile) {
        case PowerProfile::MAX_PERFORMANCE:
            policy.cpuFrequencyHz = 240000000;
            policy.flashDeepPowerDown = false;
            policy.voltageScaleLevel = 0;
            policy.enableDynamicVoltageScaling = false;
            policy.performanceLevel = 100;
            policy.powerEfficiencyLevel = 0;
            
            for(int i = 0; i < 8; ++i) {
                policy.peripheralMask[i] = ~0ULL;
            }
            break;

        case PowerProfile::HIGH_PERFORMANCE:
            policy.cpuFrequencyHz = 160000000;
            policy.flashDeepPowerDown = false;
            policy.voltageScaleLevel = 1;
            policy.enableDynamicVoltageScaling = true;
            policy.performanceLevel = 80;
            policy.powerEfficiencyLevel = 20;
            
            for(int i = 0; i < 8; ++i) {
                policy.peripheralMask[i] = ~0ULL;
            }
            break;
            
        case PowerProfile::BALANCED_MOBILE:
            policy.cpuFrequencyHz = 80000000;
            policy.flashDeepPowerDown = false;
            policy.voltageScaleLevel = 2;
            policy.enableDynamicVoltageScaling = true;
            policy.performanceLevel = 50;
            policy.powerEfficiencyLevel = 50;
            
            setPeripheralMask(Peripheral::UART_0);
            setPeripheralMask(Peripheral::SPI_0);
            setPeripheralMask(Peripheral::I2C_0);
            setPeripheralMask(Peripheral::ADC_0_12BIT);
            setPeripheralMask(Peripheral::TIMER_1_16BIT_GENERAL);
            setPeripheralMask(Peripheral::GPIO_PORT_A);
            setPeripheralMask(Peripheral::GPIO_PORT_B);
            break;

        case PowerProfile::LOW_POWER_ACTIVE:
            policy.cpuFrequencyHz = 40000000;
            policy.flashDeepPowerDown = true;
            policy.voltageScaleLevel = 3;
            policy.enableDynamicVoltageScaling = true;
            policy.performanceLevel = 25;
            policy.powerEfficiencyLevel = 75;
            
            setPeripheralMask(Peripheral::RTC_INTERNAL);
            setPeripheralMask(Peripheral::UART_0);
            setPeripheralMask(Peripheral::TIMER_0_8BIT_BASIC);
            break;

        case PowerProfile::ULTRA_LOW_POWER:
            policy.cpuFrequencyHz = 10000000;
            policy.flashDeepPowerDown = true;
            policy.voltageScaleLevel = 4;
            policy.enableDynamicVoltageScaling = true;
            policy.enablePowerGating = true;
            policy.performanceLevel = 10;
            policy.powerEfficiencyLevel = 90;
            
            setPeripheralMask(Peripheral::RTC_INTERNAL);
            setPeripheralMask(Peripheral::WATCHDOG_SYSTEM);
            setPeripheralMask(Peripheral::BROWN_OUT_DETECTOR);
            break;
            
        case PowerProfile::MACHINE_LEARNING_INFERENCE:
            policy.cpuFrequencyHz = 200000000;
            policy.flashDeepPowerDown = false;
            policy.voltageScaleLevel = 1;
            policy.performanceLevel = 90;
            policy.powerEfficiencyLevel = 10;
            
            setPeripheralMask(Peripheral::FPU_SINGLE_PRECISION);
            setPeripheralMask(Peripheral::DSP_INSTRUCTION_SET);
            setPeripheralMask(Peripheral::NEURAL_PROCESSING_UNIT);
            setPeripheralMask(Peripheral::DMA_CONTROLLER_0);
            setPeripheralMask(Peripheral::SRAM_BANK_1);
            setPeripheralMask(Peripheral::SRAM_BANK_2);
            setPeripheralMask(Peripheral::CACHE_CONTROLLER);
            break;
            
        case PowerProfile::AUDIO_PROCESSING:
            policy.cpuFrequencyHz = 120000000;
            policy.flashDeepPowerDown = false;
            policy.voltageScaleLevel = 2;
            policy.performanceLevel = 70;
            policy.powerEfficiencyLevel = 30;
            
            setPeripheralMask(Peripheral::I2S_0_STD);
            setPeripheralMask(Peripheral::I2S_1_STD);
            setPeripheralMask(Peripheral::SAI_0);
            setPeripheralMask(Peripheral::AUDIO_CODEC);
            setPeripheralMask(Peripheral::AUDIO_DSP);
            setPeripheralMask(Peripheral::DMA_AUDIO);
            setPeripheralMask(Peripheral::ADC_0_12BIT);
            setPeripheralMask(Peripheral::DAC_1_12BIT);
            break;
            
        case PowerProfile::ADAPTIVE_PERFORMANCE:
            policy.cpuFrequencyHz = 120000000; 
            policy.enableDynamicVoltageScaling = true;
            policy.adaptiveScaling.enabled = true;
            policy.adaptiveScaling.performanceThresholdPercent = 80;
            policy.adaptiveScaling.enableWorkloadPrediction = true;
            policy.performanceLevel = 75;
            policy.powerEfficiencyLevel = 25;
            break;
            
        case PowerProfile::ADAPTIVE_BATTERY_LIFE:
            policy.cpuFrequencyHz = 60000000; 
            policy.enableDynamicVoltageScaling = true;
            policy.adaptiveScaling.enabled = true;
            policy.adaptiveScaling.powerThresholdPercent = 30;
            policy.adaptiveScaling.enableEnergyBudgeting = true;
            policy.performanceLevel = 30;
            policy.powerEfficiencyLevel = 70;
            break;
            
        case PowerProfile::ADAPTIVE_THERMAL:
            policy.cpuFrequencyHz = 100000000; 
            policy.enableDynamicVoltageScaling = true;
            policy.adaptiveScaling.enabled = true;
            policy.adaptiveScaling.temperatureThresholdCelsius = 65.0f;
            policy.performanceLevel = 60;
            policy.powerEfficiencyLevel = 40;
            break;
            
        case PowerProfile::CUSTOM_POLICY:
            
            return Status::OK;
            
        default:
            return Status::FAILURE_INVALID_PARAMETER;
    }
    
    m_customPolicy = policy;
    applyPolicy(policy);
    return Status::OK;
}

void ChronosPowerManager::applyPolicy(const PowerPolicy& policy) {
    
    if (policy.cpuFrequencyHz > 0) {
        platform_setCpuFrequency(policy.cpuFrequencyHz);
    }
    
    
    if (policy.enableDynamicVoltageScaling) {
        platform_enableDynamicVoltageScaling(true);
    }
    
    
    uint64_t allPeripherals[8];
    for(int i = 0; i < 8; ++i) {
        allPeripherals[i] = ~0ULL;
    }
    
    uint64_t disabledMask[8];
    for(int i = 0; i < 8; ++i) {
        disabledMask[i] = allPeripherals[i] & ~policy.peripheralMask[i];
    }
    
    
    massControl(disabledMask, 8, false); 
    massControl(policy.peripheralMask, 8, true); 
    
    logMessage(LogLevel::DEBUG, "Power policy applied");
}

void ChronosPowerManager::updateMetrics() {
    uint32_t currentTime = millis();
    
    
    m_energyMetrics.uptimeSeconds = (currentTime - m_totalSleepTime) / 1000;
    m_energyMetrics.sleepTimeSeconds = m_totalSleepTime / 1000;
    
    
    float temp;
    if (getInternalTemperature(temp) == Status::OK) {
        m_thermalMetrics.currentTemperatureCelsius = temp;
        
        
        m_thermalMetrics.averageTemperatureCelsius = 
            (m_thermalMetrics.averageTemperatureCelsius * 0.9f) + (temp * 0.1f);
        
        
        if (temp > m_adaptiveConfig.temperatureThresholdCelsius) {
            m_thermalMetrics.thermalThrottlingActive = true;
        } else if (temp < m_adaptiveConfig.temperatureThresholdCelsius - 5.0f) {
            m_thermalMetrics.thermalThrottlingActive = false;
        }
    }
    
    
    m_performanceMetrics.cpuUtilizationPercent = 
        std::min(100u, (getCpuFrequency() * 100) / 240000000); 
    
    
    triggerCallbacks();
}

void ChronosPowerManager::updateAdaptiveScaling() {
    if (!m_adaptiveScalingEnabled) return;
    
    uint32_t currentTime = millis();
    if (currentTime - m_lastAdaptiveUpdate < m_adaptiveConfig.samplingIntervalMs) {
        return;
    }
    
    m_lastAdaptiveUpdate = currentTime;
    
    
    bool needsPerformanceBoost = m_performanceMetrics.cpuUtilizationPercent > 
                                m_adaptiveConfig.performanceThresholdPercent;
    
    bool needsPowerReduction = m_thermalMetrics.thermalThrottlingActive ||
                              (m_energyMetrics.batteryPercentage < 20.0f);
    
    if (needsPerformanceBoost && !needsPowerReduction) {
        
        uint32_t currentFreq = getCpuFrequency();
        uint32_t newFreq = std::min(240000000u, currentFreq + 20000000);
        if (newFreq != currentFreq) {
            setCpuFrequency(newFreq);
            logMessage(LogLevel::DEBUG, "Adaptive scaling: frequency increased");
        }
    } else if (needsPowerReduction || 
               m_performanceMetrics.cpuUtilizationPercent < m_adaptiveConfig.powerThresholdPercent) {
        
        uint32_t currentFreq = getCpuFrequency();
        uint32_t newFreq = std::max(10000000u, currentFreq - 10000000);
        if (newFreq != currentFreq) {
            setCpuFrequency(newFreq);
            logMessage(LogLevel::DEBUG, "Adaptive scaling: frequency decreased");
        }
    }
}

void ChronosPowerManager::updateEnergyBudget() {
    if (m_energyBudget.dailyBudgetMicroWattHours <= 0) return;
    
    uint32_t currentTime = millis();
    if (currentTime - m_lastEnergyUpdate < 1000) { 
        return;
    }
    
    float timeDeltaHours = (currentTime - m_lastEnergyUpdate) / 3600000.0f;
    m_lastEnergyUpdate = currentTime;
    
    
    uint32_t freq = getCpuFrequency();
    float estimatedCurrentMa = (freq / 1000000.0f) * 0.5f; 
    
    
    int activePeripherals = 0;
    for (int i = 0; i < 8; ++i) {
        activePeripherals += __builtin_popcountll(m_customPolicy.peripheralMask[i]);
    }
    estimatedCurrentMa += activePeripherals * 0.1f; 
    
    m_energyMetrics.currentConsumptionMicroAmps = estimatedCurrentMa * 1000.0f;
    m_energyMetrics.energyConsumedMicroWattHours += 
        m_energyMetrics.currentConsumptionMicroAmps * 3.3f * timeDeltaHours; 
    
    
    if (m_energyBudget.enableStrictMode) {
        float hourlyBudget = m_energyBudget.hourlyBudgetMicroWattHours;
        float currentHourlyConsumption = m_energyMetrics.currentConsumptionMicroAmps * 3.3f;
        
        if (currentHourlyConsumption > hourlyBudget) {
            
            logMessage(LogLevel::WARNING, "Energy budget exceeded - reducing performance");
            optimizeForBatteryLife();
        }
    }
}

void ChronosPowerManager::logMessage(LogLevel level, const char* message, Status status) {
    if (level > m_logLevel) return;
    
    LogEntry entry;
    entry.timestamp = millis();
    entry.level = level;
    entry.message = message;
    entry.statusCode = status;
    
    m_logEntries.push_back(entry);
    
    
    if (m_logEntries.size() > MAX_LOG_ENTRIES) {
        m_logEntries.erase(m_logEntries.begin());
    }
    
    
    if (level <= LogLevel::WARNING) {
        Serial.print("[ChronosPower] ");
        Serial.print(entry.timestamp);
        Serial.print(" ");
        switch (level) {
            case LogLevel::ERROR: Serial.print("ERROR: "); break;
            case LogLevel::WARNING: Serial.print("WARN: "); break;
            case LogLevel::INFO: Serial.print("INFO: "); break;
            case LogLevel::DEBUG: Serial.print("DEBUG: "); break;
            default: break;
        }
        Serial.print(message);
        if (status != Status::OK) {
            Serial.print(" (");
            Serial.print(statusToString(status));
            Serial.print(")");
        }
        Serial.println();
    }
    
    
    if (level == LogLevel::ERROR && m_errorCallback) {
        m_errorCallback(status, message);
    }
}

void ChronosPowerManager::triggerCallbacks() {
    if (m_energyCallback) {
        m_energyCallback(m_energyMetrics);
    }
    
    if (m_thermalCallback) {
        m_thermalCallback(m_thermalMetrics);
    }
    
    if (m_performanceCallback) {
        m_performanceCallback(m_performanceMetrics);
    }
}


#if defined(__AVR__)

Status ChronosPowerManager::platform_setCpuFrequency(uint32_t frequencyHz) {
    if (F_CPU < frequencyHz || (F_CPU % frequencyHz) != 0) {
        return Status::FAILURE_INVALID_FREQUENCY;
    }
    
    uint8_t divisor = F_CPU / frequencyHz;
    uint8_t prescaler = 0;
    
    while (divisor > 1 && prescaler < 8) {
        divisor >>= 1;
        prescaler++;
    }
    
    if (divisor != 1) {
        return Status::FAILURE_INVALID_FREQUENCY;
    }
    
    noInterrupts();
    CLKPR = (1 << CLKPCE);
    CLKPR = prescaler;
    interrupts();
    
    return Status::OK;
}

uint32_t ChronosPowerManager::platform_getCpuFrequency() const {
    return F_CPU / (1 << (CLKPR & 0x0F));
}

Status ChronosPowerManager::platform_control(Peripheral p, bool enable) {
    switch (p) {
        case Peripheral::ADC_0_12BIT:
            if (enable) power_adc_enable(); else power_adc_disable();
            break;
        case Peripheral::TIMER_0_8BIT_BASIC:
            if (enable) power_timer0_enable(); else power_timer0_disable();
            break;
        case Peripheral::TIMER_1_16BIT_GENERAL:
            if (enable) power_timer1_enable(); else power_timer1_disable();
            break;
        case Peripheral::TIMER_2_8BIT_ASYNC:
            if (enable) power_timer2_enable(); else power_timer2_disable();
            break;
        case Peripheral::SPI_0:
            if (enable) power_spi_enable(); else power_spi_disable();
            break;
        case Peripheral::I2C_0:
            if (enable) power_twi_enable(); else power_twi_disable();
            break;
        case Peripheral::UART_0:
            if (enable) power_usart0_enable(); else power_usart0_disable();
            break;
        #if defined(power_timer3_enable)
        case Peripheral::TIMER_3_16BIT_GENERAL:
            if (enable) power_timer3_enable(); else power_timer3_disable();
            break;
        #endif
        #if defined(power_usart1_enable)
        case Peripheral::UART_1:
            if (enable) power_usart1_enable(); else power_usart1_disable();
            break;
        #endif
        case Peripheral::BROWN_OUT_DETECTOR:
            if (!enable) {
                noInterrupts();
                MCUCR |= (1<<BODS)|(1<<BODSE);
                MCUCR &= ~(1<<BODSE);
                interrupts();
            }
            break;
        case Peripheral::PSEUDO_PERIPHERAL_ARDUINO_MILLIS:
            return platform_control(Peripheral::TIMER_0_8BIT_BASIC, enable);
        default:
            return Status::FAILURE_PERIPHERAL_NOT_FOUND;
    }
    return Status::OK;
}

void ChronosPowerManager::platform_sleep(const WakeupPolicy& policy) {
    
    if (policy.wdt.timeoutMillis > 0) {
        uint8_t wdtBits = 0;
        uint32_t timeout = policy.wdt.timeoutMillis;
        
        if (timeout <= 16) wdtBits = 0;
        else if (timeout <= 32) wdtBits = 1;
        else if (timeout <= 64) wdtBits = 2;
        else if (timeout <= 125) wdtBits = 3;
        else if (timeout <= 250) wdtBits = 4;
        else if (timeout <= 500) wdtBits = 5;
        else if (timeout <= 1000) wdtBits = 6;
        else if (timeout <= 2000) wdtBits = 7;
        else if (timeout <= 4000) wdtBits = 8;
        else wdtBits = 9;
        
        uint8_t wdtcsr_val = (wdtBits & 8 ? 1<<WDP3 : 0) | (wdtBits & 7);
        if (policy.wdt.generateInterrupt) {
            wdtcsr_val |= (1<<WDIE);
        }
        
        noInterrupts();
        wdt_reset();
        MCUSR &= ~(1 << WDRF);
        WDTCSR |= (1 << WDCE) | (1 << WDE);
        WDTCSR = wdtcsr_val;
        interrupts();
    }
    
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    noInterrupts();
    sleep_enable();
    sleep_bod_disable();
    interrupts();
    sleep_cpu();
    sleep_disable();
    
    
    if (policy.wdt.timeoutMillis > 0) {
        wdt_disable();
    }
}

[[noreturn]] void ChronosPowerManager::platform_hibernate(const WakeupPolicy& policy) {
    
    if (policy.extInterrupt.pinMask > 0) {
        
        
    }
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    noInterrupts();
    sleep_enable();
    sleep_bod_disable();
    interrupts();
    
    for(;;) {
        sleep_cpu();
    }
}

WakeupReason ChronosPowerManager::platform_getWakeupReason() {
    
    
    return WakeupReason::UNKNOWN;
}

ResetReason ChronosPowerManager::platform_getResetReason() {
    uint8_t resetFlags = MCUSR;
    MCUSR = 0; 
    
    if (resetFlags & (1<<WDRF)) return ResetReason::WATCHDOG_RESET;
    if (resetFlags & (1<<BORF)) return ResetReason::BROWN_OUT_RESET;
    if (resetFlags & (1<<EXTRF)) return ResetReason::EXTERNAL_RESET_PIN;
    if (resetFlags & (1<<PORF)) return ResetReason::POWER_ON_RESET;
    
    return ResetReason::UNKNOWN;
}

Status ChronosPowerManager::platform_getInternalTemperature(float& temp) {
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

Status ChronosPowerManager::platform_getCoreVoltage(float& voltage) {
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

Status ChronosPowerManager::platform_getFlashState(FlashState& state) {
    state = FlashState::UNKNOWN;
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

Status ChronosPowerManager::platform_getBatteryLevel(float& percentage) {
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

void ChronosPowerManager::platform_synchronizeSystemTime(uint32_t sleepMillis) {
    
    extern volatile unsigned long timer0_millis;
    noInterrupts();
    timer0_millis += sleepMillis;
    interrupts();
}

Status ChronosPowerManager::platform_setSystemClockSource(SystemClockSource source) {
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

Status ChronosPowerManager::platform_enableDynamicVoltageScaling(bool enable) {
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

Status ChronosPowerManager::platform_setPlatformOption(const std::string& option, const std::string& value) {
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

std::string ChronosPowerManager::platform_getPlatformOption(const std::string& option) const {
    return "";
}


ISR(WDT_vect) {
    
}

#elif defined(ESP32)

Status ChronosPowerManager::platform_setCpuFrequency(uint32_t frequencyHz) {
    uint32_t frequencyMhz = frequencyHz / 1000000;
    return setCpuFrequencyMhz(frequencyMhz) ? Status::OK : Status::FAILURE_INVALID_FREQUENCY;
}

uint32_t ChronosPowerManager::platform_getCpuFrequency() const {
    return getCpuFrequencyMhz() * 1000000;
}

Status ChronosPowerManager::platform_control(Peripheral p, bool enable) {
    esp_err_t result = ESP_OK;
    
    switch(p) {
        case Peripheral::WIFI_MAC_2_4GHZ:
            result = enable ? esp_wifi_start() : esp_wifi_stop();
            break;
        case Peripheral::BLUETOOTH_CONTROLLER:
            result = enable ? esp_bluedroid_enable() : esp_bluedroid_disable();
            break;
        case Peripheral::ADC_0_12BIT:
            
            break;
        case Peripheral::UART_0:
        case Peripheral::UART_1:
        case Peripheral::UART_2:
            
            break;
        case Peripheral::SPI_0:
        case Peripheral::SPI_1:
        case Peripheral::SPI_2:
            
            break;
        case Peripheral::I2C_0:
        case Peripheral::I2C_1:
            
            break;
        default:
            return Status::FAILURE_PERIPHERAL_NOT_FOUND;
    }
    
    return (result == ESP_OK) ? Status::OK : Status::FAILURE_GENERIC;
}

void ChronosPowerManager::platform_sleep(const WakeupPolicy& policy) {
    
    if (policy.extInterrupt.pinMask > 0) {
        esp_sleep_enable_ext1_wakeup(policy.extInterrupt.pinMask, 
                                    (esp_sleep_ext1_wakeup_mode_t)policy.extInterrupt.triggerMode);
    }
    
    if (policy.rtc.timeoutSeconds > 0) {
        esp_sleep_enable_timer_wakeup(policy.rtc.timeoutSeconds * 1000000ULL);
    }
    
    if (policy.wdt.timeoutMillis > 0) {
        esp_sleep_enable_timer_wakeup(policy.wdt.timeoutMillis * 1000ULL);
    }
    
    if (policy.touchThreshold > 0) {
        esp_sleep_enable_touchpad_wakeup();
    }
    
    if (policy.uartWakeup.portIndex > 0) {
        esp_sleep_enable_uart_wakeup(policy.uartWakeup.portIndex - 1);
    }
    
    if (policy.enableUlpWakeup) {
        esp_sleep_enable_ulp_wakeup();
    }
    
    if (policy.enableWifiWakeup) {
        esp_sleep_enable_wifi_wakeup();
    }
    
    if (policy.enableBluetoothWakeup) {
        esp_sleep_enable_bt_wakeup();
    }
    
    
    esp_light_sleep_start();
}

[[noreturn]] void ChronosPowerManager::platform_hibernate(const WakeupPolicy& policy) {
    
    if (policy.extInterrupt.pinMask > 0) {
        esp_sleep_enable_ext1_wakeup(policy.extInterrupt.pinMask,
                                    (esp_sleep_ext1_wakeup_mode_t)policy.extInterrupt.triggerMode);
    }
    
    if (policy.rtc.timeoutSeconds > 0) {
        esp_sleep_enable_timer_wakeup(policy.rtc.timeoutSeconds * 1000000ULL);
    }
    
    if (policy.touchThreshold > 0) {
        esp_sleep_enable_touchpad_wakeup();
    }
    
    
    esp_deep_sleep_start();
}

WakeupReason ChronosPowerManager::platform_getWakeupReason() {
    switch (esp_sleep_get_wakeup_cause()) {
        case ESP_SLEEP_WAKEUP_UNDEFINED: return WakeupReason::UNKNOWN;
        case ESP_SLEEP_WAKEUP_EXT0: return WakeupReason::EXTERNAL_PIN_0;
        case ESP_SLEEP_WAKEUP_EXT1: return WakeupReason::EXTERNAL_PIN_MULTIPLE;
        case ESP_SLEEP_WAKEUP_TIMER: return WakeupReason::RTC_ALARM_SECONDS;
        case ESP_SLEEP_WAKEUP_TOUCHPAD: return WakeupReason::TOUCH_SENSOR_GLOBAL;
        case ESP_SLEEP_WAKEUP_ULP: return WakeupReason::ULP_COPROCESSOR;
        case ESP_SLEEP_WAKEUP_GPIO: return WakeupReason::EXTERNAL_PIN_MULTIPLE;
        case ESP_SLEEP_WAKEUP_UART: return WakeupReason::UART_RX_ACTIVITY_0;
        case ESP_SLEEP_WAKEUP_WIFI: return WakeupReason::WIFI_EVENT;
        case ESP_SLEEP_WAKEUP_BT: return WakeupReason::BLUETOOTH_LE_CONNECTION;
        default: return WakeupReason::UNKNOWN;
    }
}

ResetReason ChronosPowerManager::platform_getResetReason() {
    switch (esp_reset_reason()) {
        case ESP_RST_POWERON: return ResetReason::POWER_ON_RESET;
        case ESP_RST_EXT: return ResetReason::EXTERNAL_RESET_PIN;
        case ESP_RST_SW: return ResetReason::SOFTWARE_RESET_USER;
        case ESP_RST_PANIC: return ResetReason::SOFTWARE_RESET_EXCEPTION;
        case ESP_RST_INT_WDT: return ResetReason::WATCHDOG_RESET;
        case ESP_RST_TASK_WDT: return ResetReason::WATCHDOG_RESET;
        case ESP_RST_WDT: return ResetReason::WATCHDOG_RESET;
        case ESP_RST_DEEPSLEEP: return ResetReason::DEEP_SLEEP_HIBERNATE;
        case ESP_RST_BROWNOUT: return ResetReason::BROWN_OUT_RESET;
        case ESP_RST_SDIO: return ResetReason::UNKNOWN;
        default: return ResetReason::UNKNOWN;
    }
}

Status ChronosPowerManager::platform_getInternalTemperature(float& temp) {
    temp = temperatureRead();
    return Status::OK;
}

Status ChronosPowerManager::platform_getCoreVoltage(float& voltage) {
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

Status ChronosPowerManager::platform_getFlashState(FlashState& state) {
    state = FlashState::ACTIVE; 
    return Status::OK;
}

Status ChronosPowerManager::platform_getBatteryLevel(float& percentage) {
    
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

void ChronosPowerManager::platform_synchronizeSystemTime(uint32_t sleepMillis) {
    
}

Status ChronosPowerManager::platform_setSystemClockSource(SystemClockSource source) {
    
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

Status ChronosPowerManager::platform_enableDynamicVoltageScaling(bool enable) {
    
    esp_pm_config_esp32_t pm_config;
    pm_config.max_freq_mhz = getCpuFrequencyMhz();
    pm_config.min_freq_mhz = enable ? 10 : pm_config.max_freq_mhz;
    pm_config.light_sleep_enable = enable;
    
    esp_err_t result = esp_pm_configure(&pm_config);
    return (result == ESP_OK) ? Status::OK : Status::FAILURE_GENERIC;
}

Status ChronosPowerManager::platform_setPlatformOption(const std::string& option, const std::string& value) {
    
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

std::string ChronosPowerManager::platform_getPlatformOption(const std::string& option) const {
    return "";
}

#elif defined(ARDUINO_ARCH_RP2040)

Status ChronosPowerManager::platform_setCpuFrequency(uint32_t frequencyHz) {
    uint32_t frequencyKhz = frequencyHz / 1000;
    return set_sys_clock_khz(frequencyKhz, true) ? Status::OK : Status::FAILURE_INVALID_FREQUENCY;
}

uint32_t ChronosPowerManager::platform_getCpuFrequency() const {
    return clock_get_hz(clk_sys);
}

Status ChronosPowerManager::platform_control(Peripheral p, bool enable) {
    
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

void ChronosPowerManager::platform_sleep(const WakeupPolicy& policy) {
    
    __wfi(); 
}

[[noreturn]] void ChronosPowerManager::platform_hibernate(const WakeupPolicy& policy) {
    if (policy.rtc.timeoutSeconds > 0) {
        watchdog_enable(policy.rtc.timeoutSeconds * 1000 + 100, true);
    }
    
    for(;;) {
        __wfi();
    }
}

WakeupReason ChronosPowerManager::platform_getWakeupReason() {
    return WakeupReason::UNKNOWN;
}

ResetReason ChronosPowerManager::platform_getResetReason() {
    if (watchdog_caused_reboot()) {
        return ResetReason::WATCHDOG_RESET;
    }
    return ResetReason::POWER_ON_RESET;
}

Status ChronosPowerManager::platform_getInternalTemperature(float& temp) {
    adc_init();
    adc_set_temp_sensor_enabled(true);
    delay(1);
    
    float adcValue = adc_read() * 3.3f / 4096.0f;
    temp = 27.0f - (adcValue - 0.706f) / 0.001721f;
    
    adc_set_temp_sensor_enabled(false);
    return Status::OK;
}

Status ChronosPowerManager::platform_getCoreVoltage(float& voltage) {
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

Status ChronosPowerManager::platform_getFlashState(FlashState& state) {
    state = FlashState::ACTIVE;
    return Status::OK;
}

Status ChronosPowerManager::platform_getBatteryLevel(float& percentage) {
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

void ChronosPowerManager::platform_synchronizeSystemTime(uint32_t sleepMillis) {
    
}

Status ChronosPowerManager::platform_setSystemClockSource(SystemClockSource source) {
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

Status ChronosPowerManager::platform_enableDynamicVoltageScaling(bool enable) {
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

Status ChronosPowerManager::platform_setPlatformOption(const std::string& option, const std::string& value) {
    return Status::FAILURE_UNSUPPORTED_FEATURE;
}

std::string ChronosPowerManager::platform_getPlatformOption(const std::string& option) const {
    return "";
}

#else


Status ChronosPowerManager::platform_setCpuFrequency(uint32_t frequencyHz) {
    return Status::FAILURE_UNSUPPORTED_ARCH;
}

uint32_t ChronosPowerManager::platform_getCpuFrequency() const {
    return 0;
}

Status ChronosPowerManager::platform_control(Peripheral p, bool enable) {
    return Status::FAILURE_UNSUPPORTED_ARCH;
}

void ChronosPowerManager::platform_sleep(const WakeupPolicy& policy) {
    delay(1000); 
}

[[noreturn]] void ChronosPowerManager::platform_hibernate(const WakeupPolicy& policy) {
    for(;;) {
        delay(1000);
    }
}

WakeupReason ChronosPowerManager::platform_getWakeupReason() {
    return WakeupReason::UNKNOWN;
}

ResetReason ChronosPowerManager::platform_getResetReason() {
    return ResetReason::UNKNOWN;
}

Status ChronosPowerManager::platform_getInternalTemperature(float& temp) {
    return Status::FAILURE_UNSUPPORTED_ARCH;
}

Status ChronosPowerManager::platform_getCoreVoltage(float& voltage) {
    return Status::FAILURE_UNSUPPORTED_ARCH;
}

Status ChronosPowerManager::platform_getFlashState(FlashState& state) {
    state = FlashState::UNKNOWN;
    return Status::FAILURE_UNSUPPORTED_ARCH;
}

Status ChronosPowerManager::platform_getBatteryLevel(float& percentage) {
    return Status::FAILURE_UNSUPPORTED_ARCH;
}

void ChronosPowerManager::platform_synchronizeSystemTime(uint32_t sleepMillis) {
    
}

Status ChronosPowerManager::platform_setSystemClockSource(SystemClockSource source) {
    return Status::FAILURE_UNSUPPORTED_ARCH;
}

Status ChronosPowerManager::platform_enableDynamicVoltageScaling(bool enable) {
    return Status::FAILURE_UNSUPPORTED_ARCH;
}

Status ChronosPowerManager::platform_setPlatformOption(const std::string& option, const std::string& value) {
    return Status::FAILURE_UNSUPPORTED_ARCH;
}

std::string ChronosPowerManager::platform_getPlatformOption(const std::string& option) const {
    return "";
}

#endif

} 
