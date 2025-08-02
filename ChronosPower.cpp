#include "ChronosPower.h"
#include <math.h>

namespace ChronosPower {

ChronosPowerManager::ChronosPowerManager() :
    m_currentProfile(PowerProfile::BALANCED_MOBILE),
    m_lastWakeupReason(WakeupReason::POWER_ON_RESET),
    m_preSleepMillis(0),
    m_postWakeupMillis(0),
    m_eventCallback(nullptr),
    m_isInitialized(false)
{
    for(int i = 0; i < 4; ++i) m_customPolicy.peripheralMask[i] = 0;
}

Status ChronosPowerManager::begin(PowerProfile initialProfile) {
    if (m_isInitialized) return Status::OK;
    m_isInitialized = true;
    m_lastWakeupReason = platform_getWakeupReason();
    return setProfile(initialProfile);
}

Status ChronosPowerManager::setProfile(PowerProfile profile) {
    if (!m_isInitialized) return Status::FAILURE_GENERIC;
    m_currentProfile = profile;
    return applyProfile(profile);
}

PowerProfile ChronosPowerManager::getProfile() const {
    return m_currentProfile;
}

Status ChronosPowerManager::setCustomPolicy(const PowerPolicy& policy) {
    if (!m_isInitialized) return Status::FAILURE_GENERIC;
    m_customPolicy = policy;
    m_currentProfile = PowerProfile::CUSTOM_POLICY;
    applyPolicy(m_customPolicy);
    return Status::OK;
}

Status ChronosPowerManager::getCustomPolicy(PowerPolicy& policy) const {
    policy = m_customPolicy;
    return Status::OK;
}

Status ChronosPowerManager::applyProfile(PowerProfile profile) {
    PowerPolicy policy;
    for(int i = 0; i < 4; ++i) policy.peripheralMask[i] = 0;
    
    auto set_mask = [&](Peripheral p) {
        uint16_t val = static_cast<uint16_t>(p);
        policy.peripheralMask[val / 64] |= (1ULL << (val % 64));
    };

    set_mask(Peripheral::CPU_CORE_0);
    set_mask(Peripheral::SRAM_BANK_0);
    set_mask(Peripheral::FLASH_CONTROLLER);
    set_mask(Peripheral::SYSTEM_CONTROLLER);
    set_mask(Peripheral::PSEUDO_PERIPHERAL_ARDUINO_MILLIS);
    
    switch(profile) {
        case PowerProfile::MAX_PERFORMANCE:
            policy.cpuFrequencyHz = 240000000;
            policy.flashDeepPowerDown = false;
            policy.voltageScaleLevel = 0;
            for(int i = 0; i < 4; ++i) policy.peripheralMask[i] = ~0ULL;
            break;

        case PowerProfile::HIGH_PERFORMANCE:
            policy.cpuFrequencyHz = 160000000;
            policy.flashDeepPowerDown = false;
            policy.voltageScaleLevel = 1;
            for(int i = 0; i < 4; ++i) policy.peripheralMask[i] = ~0ULL;
            policy.peripheralMask[static_cast<uint16_t>(Peripheral::TRNG_UNIT) / 64] &= ~(1ULL << (static_cast<uint16_t>(Peripheral::TRNG_UNIT) % 64));
            break;
            
        case PowerProfile::BALANCED_DESKTOP_REPLACEMENT:
            policy.cpuFrequencyHz = 120000000;
            policy.flashDeepPowerDown = false;
            policy.voltageScaleLevel = 2;
            set_mask(Peripheral::DMA_CONTROLLER_0);
            set_mask(Peripheral::USB_OTG_FS);
            set_mask(Peripheral::UART_0);
            set_mask(Peripheral::SPI_0);
            set_mask(Peripheral::I2C_0);
            set_mask(Peripheral::TIMER_1_16BIT_GENERAL);
            set_mask(Peripheral::ADC_0_12BIT);
            set_mask(Peripheral::PSEUDO_PERIPHERAL_SERIAL_PRINT);
            break;

        case PowerProfile::BALANCED_MOBILE:
            policy.cpuFrequencyHz = 80000000;
            policy.flashDeepPowerDown = false;
            policy.voltageScaleLevel = 2;
            set_mask(Peripheral::DMA_CONTROLLER_0);
            set_mask(Peripheral::UART_0);
            set_mask(Peripheral::SPI_0);
            set_mask(Peripheral::I2C_0);
            set_mask(Peripheral::TIMER_1_16BIT_GENERAL);
            set_mask(Peripheral::ADC_0_12BIT);
            set_mask(Peripheral::PSEUDO_PERIPHERAL_SERIAL_PRINT);
            set_mask(Peripheral::WIFI_MAC_2_4GHZ);
            set_mask(Peripheral::BLUETOOTH_LE_PHY);
            break;

        case PowerProfile::LOW_POWER_ACTIVE:
            policy.cpuFrequencyHz = 40000000;
            policy.flashDeepPowerDown = true;
            policy.voltageScaleLevel = 3;
            set_mask(Peripheral::RTC_INTERNAL);
            set_mask(Peripheral::UART_LP_0);
            set_mask(Peripheral::TIMER_8_LOW_POWER);
            break;

        case PowerProfile::ULTRA_LOW_POWER_IDLE:
            policy.cpuFrequencyHz = 10000000;
            policy.flashDeepPowerDown = true;
            policy.voltageScaleLevel = 4;
            set_mask(Peripheral::RTC_INTERNAL);
            set_mask(Peripheral::WATCHDOG_SYSTEM);
            set_mask(Peripheral::BROWN_OUT_DETECTOR);
            set_mask(Peripheral::GPIO_PORT_A);
            break;
            
        case PowerProfile::DEEP_SLEEP_PREP:
        case PowerProfile::HIBERNATION_PREP:
        case PowerProfile::SHUTDOWN_PREP:
            policy.cpuFrequencyHz = 1000000;
            policy.flashDeepPowerDown = true;
            policy.voltageScaleLevel = 5;
            set_mask(Peripheral::RTC_INTERNAL);
            set_mask(Peripheral::GPIO_PORT_A);
            break;

        case PowerProfile::CUSTOM_POLICY:
            applyPolicy(m_customPolicy);
            return Status::OK;
    }
    
    m_customPolicy = policy;
    applyPolicy(policy);
    return Status::OK;
}

void ChronosPowerManager::applyPolicy(const PowerPolicy& policy) {
    platform_setCpuFrequency(policy.cpuFrequencyHz);
    uint64_t allPeripherals[4] = {~0ULL, ~0ULL, ~0ULL, ~0ULL};
    uint64_t disabledMask[4];
    for(int i = 0; i < 4; ++i) disabledMask[i] = allPeripherals[i] & ~policy.peripheralMask[i];
    massControl(disabledMask, 4, false);
    massControl(policy.peripheralMask, 4, true);
}

Status ChronosPowerManager::setCpuFrequency(uint32_t frequencyHz) {
    if (!m_isInitialized) return Status::FAILURE_GENERIC;
    return platform_setCpuFrequency(frequencyHz);
}

uint32_t ChronosPowerManager::getCpuFrequency() const {
    if (!m_isInitialized) return 0;
    return platform_getCpuFrequency();
}

Status ChronosPowerManager::sleep(const WakeupPolicy& policy) {
    if (!m_isInitialized) return Status::FAILURE_GENERIC;
    if (m_eventCallback) m_eventCallback(WakeupReason::UNKNOWN);
    m_preSleepMillis = millis();
    platform_sleep(policy);
    m_postWakeupMillis = millis();
    m_lastWakeupReason = platform_getWakeupReason();
    synchronizeSystemTime();
    if (m_eventCallback) m_eventCallback(m_lastWakeupReason);
    return Status::OK;
}

[[noreturn]] void ChronosPowerManager::hibernate(const WakeupPolicy& policy) {
    applyProfile(PowerProfile::HIBERNATION_PREP);
    if (m_eventCallback) m_eventCallback(WakeupReason::UNKNOWN);
    platform_hibernate(policy);
}

[[noreturn]] void ChronosPowerManager::shutdown() {
    applyProfile(PowerProfile::SHUTDOWN_PREP);
    WakeupPolicy p;
    platform_hibernate(p);
}

Status ChronosPowerManager::control(Peripheral p, bool enable) {
    if (!m_isInitialized) return Status::FAILURE_GENERIC;
    return platform_control(p, enable);
}

Status ChronosPowerManager::massControl(const uint64_t masks[], uint8_t maskCount, bool enable) {
    for (uint8_t i = 0; i < maskCount; ++i) {
        for (uint8_t j = 0; j < 64; ++j) {
            if ((masks[i] >> j) & 1) {
                platform_control(static_cast<Peripheral>(i * 64 + j), enable);
            }
        }
    }
    return Status::OK;
}

Status ChronosPowerManager::registerEventCallback(ChronosCallback callback) {
    m_eventCallback = callback;
    return Status::OK;
}

WakeupReason ChronosPowerManager::getWakeupReason() { return m_lastWakeupReason; }
void ChronosPowerManager::clearWakeupReason() { m_lastWakeupReason = WakeupReason::UNKNOWN; }
uint32_t ChronosPowerManager::getSleepDurationMillis() const { return m_postWakeupMillis - m_preSleepMillis; }
void ChronosPowerManager::synchronizeSystemTime() { platform_synchronizeSystemTime(getSleepDurationMillis()); }
Status ChronosPowerManager::getInternalTemperature(float& temp) { return platform_getInternalTemperature(temp); }
Status ChronosPowerManager::getCoreVoltage(float& voltage) { return platform_getCoreVoltage(voltage); }
Status ChronosPowerManager::getPeripheralClock(Peripheral p, uint32_t& frequencyHz) { return Status::FAILURE_UNSUPPORTED_FEATURE; }
Status ChronosPowerManager::calibrateInternalOscillator(SystemClockSource osc, uint32_t referenceFreq) { return Status::FAILURE_UNSUPPORTED_FEATURE; }

#if defined(__AVR__)

Status ChronosPowerManager::platform_setCpuFrequency(uint32_t f) {
    if (F_CPU < f) return Status::FAILURE_INVALID_FREQUENCY;
    uint8_t d = F_CPU / f;
    uint8_t p = 0;
    while(d > 1 && p < 8) { d >>= 1; p++; }
    if (d != 1) return Status::FAILURE_INVALID_FREQUENCY;
    CLKPR = (1 << CLKPCE);
    CLKPR = p;
    return Status::OK;
}
uint32_t ChronosPowerManager::platform_getCpuFrequency() const { return F_CPU / (1 << (CLKPR & 0x0F)); }

Status ChronosPowerManager::platform_control(Peripheral p, bool e) {
    switch (p) {
        case Peripheral::ADC_0_12BIT: if(e) power_adc_enable(); else power_adc_disable(); break;
        case Peripheral::TIMER_0_8BIT_BASIC: if(e) power_timer0_enable(); else power_timer0_disable(); break;
        case Peripheral::TIMER_1_16BIT_GENERAL: if(e) power_timer1_enable(); else power_timer1_disable(); break;
        case Peripheral::TIMER_2_8BIT_ASYNC: if(e) power_timer2_enable(); else power_timer2_disable(); break;
        case Peripheral::SPI_0: if(e) power_spi_enable(); else power_spi_disable(); break;
        case Peripheral::I2C_0: if(e) power_twi_enable(); else power_twi_disable(); break;
        case Peripheral::UART_0: if(e) power_usart0_enable(); else power_usart0_disable(); break;
        #if defined(power_timer3_enable)
        case Peripheral::TIMER_3_16BIT_GENERAL: if(e) power_timer3_enable(); else power_timer3_disable(); break;
        #endif
        #if defined(power_timer4_enable)
        case Peripheral::TIMER_4_16BIT_GENERAL: if(e) power_timer4_enable(); else power_timer4_disable(); break;
        #endif
        #if defined(power_timer5_enable)
        case Peripheral::TIMER_5_16BIT_GENERAL: if(e) power_timer5_enable(); else power_timer5_disable(); break;
        #endif
        #if defined(power_usart1_enable)
        case Peripheral::UART_1: if(e) power_usart1_enable(); else power_usart1_disable(); break;
        #endif
        #if defined(power_usart2_enable)
        case Peripheral::UART_2: if(e) power_usart2_enable(); else power_usart2_disable(); break;
        #endif
        #if defined(power_usart3_enable)
        case Peripheral::UART_3: if(e) power_usart3_enable(); else power_usart3_disable(); break;
        #endif
        case Peripheral::BROWN_OUT_DETECTOR: if (!e) { MCUCR |= (1<<BODS)|(1<<BODSE); MCUCR &= ~(1<<BODSE); } break;
        default: break;
    }
    return Status::OK;
}

void ChronosPowerManager::platform_sleep(const WakeupPolicy& policy) {
    if (policy.wdt.timeoutMillis > 0) {
        uint8_t bits = 0;
        if (policy.wdt.timeoutMillis <= 16) bits = 0; else if (policy.wdt.timeoutMillis <= 32) bits = 1;
        else if (policy.wdt.timeoutMillis <= 64) bits = 2; else if (policy.wdt.timeoutMillis <= 125) bits = 3;
        else if (policy.wdt.timeoutMillis <= 250) bits = 4; else if (policy.wdt.timeoutMillis <= 500) bits = 5;
        else if (policy.wdt.timeoutMillis <= 1000) bits = 6; else if (policy.wdt.timeoutMillis <= 2000) bits = 7;
        else if (policy.wdt.timeoutMillis <= 4000) bits = 8; else bits = 9;
        uint8_t wdtcsr_val = (bits & 8 ? 1<<WDP3 : 0) | (bits & 7);
        if (policy.wdt.generateInterrupt) wdtcsr_val |= (1<<WDIE);
        cli(); wdt_reset(); MCUSR &= ~(1 << WDRF); WDTCSR |= (1 << WDCE) | (1 << WDE); WDTCSR = wdtcsr_val; sei();
    }
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); cli(); sleep_enable(); sleep_bod_disable(); sei(); sleep_cpu(); sleep_disable();
    if (policy.wdt.timeoutMillis > 0) wdt_disable();
}

void ChronosPowerManager::platform_hibernate(const WakeupPolicy& policy) {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); cli(); sleep_enable(); sleep_bod_disable(); sei(); for(;;) sleep_cpu();
}

WakeupReason ChronosPowerManager::platform_getWakeupReason() {
    uint8_t r = MCUSR; MCUSR = 0;
    if (r & (1<<WDRF)) return WakeupReason::WATCHDOG_TIMEOUT_SYSTEM_RESET;
    if (r & (1<<BORF)) return WakeupReason::BROWN_OUT_RESET_LEVEL1;
    if (r & (1<<EXTRF)) return WakeupReason::EXTERNAL_PIN_0;
    if (r & (1<<PORF)) return WakeupReason::POWER_ON_RESET;
    return WakeupReason::UNKNOWN;
}

void ChronosPowerManager::platform_synchronizeSystemTime(uint32_t ms) { extern volatile unsigned long timer0_millis; cli(); timer0_millis += ms; sei(); }
Status ChronosPowerManager::platform_getInternalTemperature(float& temp) { return Status::FAILURE_UNSUPPORTED_FEATURE; }
Status ChronosPowerManager::platform_getCoreVoltage(float& voltage) { return Status::FAILURE_UNSUPPORTED_FEATURE; }
ISR(WDT_vect) {}

#elif defined(ESP32)

Status ChronosPowerManager::platform_setCpuFrequency(uint32_t f) { return setCpuFrequencyMhz(f/1000000) ? Status::OK : Status::FAILURE_INVALID_FREQUENCY; }
uint32_t ChronosPowerManager::platform_getCpuFrequency() const { return getCpuFrequencyMhz() * 1000000; }
Status ChronosPowerManager::platform_control(Peripheral p, bool e) {
    switch(p) {
        case Peripheral::WIFI_MAC_2_4GHZ: if(e) esp_wifi_start(); else esp_wifi_stop(); break;
        case Peripheral::BLUETOOTH_CONTROLLER: if(e) esp_bluedroid_enable(); else esp_bluedroid_disable(); break;
        default: break;
    }
    return Status::OK;
}

void ChronosPowerManager::platform_sleep(const WakeupPolicy& policy) {
    if (policy.extInterrupt.pinMask > 0) esp_sleep_enable_ext1_wakeup(policy.extInterrupt.pinMask, (esp_sleep_ext1_wakeup_mode_t)policy.extInterrupt.triggerMode);
    if (policy.wdt.timeoutMillis > 0) esp_sleep_enable_timer_wakeup(policy.wdt.timeoutMillis * 1000);
    if (policy.touchThreshold > 0) { esp_sleep_enable_touchpad_wakeup(); touchAttachInterrupt(T0, nullptr, policy.touchThreshold); }
    if (policy.uartPortIndex > 0) esp_sleep_enable_uart_wakeup(policy.uartPortIndex - 1);
    if (policy.enableUlpWakeup) esp_sleep_enable_ulp_wakeup();
    if (policy.enableWifiWakeup) esp_sleep_enable_wifi_wakeup();
    if (policy.enableBluetoothWakeup) esp_sleep_enable_bt_wakeup();
    esp_light_sleep_start();
}

void ChronosPowerManager::platform_hibernate(const WakeupPolicy& policy) {
    if (policy.extInterrupt.pinMask > 0) esp_sleep_enable_ext1_wakeup(policy.extInterrupt.pinMask, (esp_sleep_ext1_wakeup_mode_t)policy.extInterrupt.triggerMode);
    if (policy.wdt.timeoutMillis > 0) esp_sleep_enable_timer_wakeup(policy.wdt.timeoutMillis * 1000);
    esp_deep_sleep_start();
}

WakeupReason ChronosPowerManager::platform_getWakeupReason() {
    switch (esp_sleep_get_wakeup_cause()) {
        case ESP_SLEEP_WAKEUP_UNDEFINED: return WakeupReason::POWER_ON_RESET;
        case ESP_SLEEP_WAKEUP_EXT0: return WakeupReason::EXTERNAL_PIN_0;
        case ESP_SLEEP_WAKEUP_EXT1: return WakeupReason::EXTERNAL_PIN_MULTIPLE;
        case ESP_SLEEP_WAKEUP_TIMER: return WakeupReason::WATCHDOG_TIMEOUT_INTERRUPT;
        case ESP_SLEEP_WAKEUP_TOUCHPAD: return WakeupReason::TOUCH_SENSOR_GLOBAL;
        case ESP_SLEEP_WAKEUP_ULP: return WakeupReason::ULP_COPROCESSOR;
        case ESP_SLEEP_WAKEUP_GPIO: return WakeupReason::EXTERNAL_PIN_MULTIPLE;
        case ESP_SLEEP_WAKEUP_UART: return WakeupReason::UART_RX_ACTIVITY_0;
        case ESP_SLEEP_WAKEUP_WIFI: return WakeupReason::WIFI_EVENT;
        case ESP_SLEEP_WAKEUP_BT: return WakeupReason::BLUETOOTH_LE_CONNECTION;
        default: return WakeupReason::UNKNOWN;
    }
}
void ChronosPowerManager::platform_synchronizeSystemTime(uint32_t ms) {}
Status ChronosPowerManager::platform_getInternalTemperature(float& temp) { temp = temperatureRead(); return Status::OK; }
Status ChronosPowerManager::platform_getCoreVoltage(float& voltage) { return Status::FAILURE_UNSUPPORTED_FEATURE; }

#elif defined(ARDUINO_ARCH_RP2040)
Status ChronosPowerManager::platform_setCpuFrequency(uint32_t f) { return set_sys_clock_khz(f/1000, true) ? Status::OK : Status::FAILURE_INVALID_FREQUENCY; }
uint32_t ChronosPowerManager::platform_getCpuFrequency() const { return clock_get_hz(clk_sys); }
Status ChronosPowerManager::platform_control(Peripheral p, bool e) {
    switch(p) {
        case Peripheral::ADC_0_12BIT: if(e) adc_init(); else adc_poweroff(); break;
        case Peripheral::USB_DEVICE: if(e) {}; break; // TODO
        default: break;
    }
    return Status::OK;
}
void ChronosPowerManager::platform_sleep(const WakeupPolicy& policy) {
    // Dormant mode is complex, using simple WFI for now.
    __wfi();
}
void ChronosPowerManager::platform_hibernate(const WakeupPolicy& policy) {
    if (policy.rtc.timeoutSeconds > 0) {
        datetime_t t;
        rtc_get_datetime(&t);
        t.sec += policy.rtc.timeoutSeconds;
        rtc_set_alarm(&t, nullptr);
    }
    uint32_t scb_orig = scb_hw->scr;
    uint32_t clock0_orig = clocks_hw->sleep_en0;
    uint32_t clock1_orig = clocks_hw->sleep_en1;
    watchdog_enable(policy.rtc.timeoutSeconds * 1000 + 100, 1);
    for(;;);
}
WakeupReason ChronosPowerManager::platform_getWakeupReason() {
    if (watchdog_caused_reboot()) return WakeupReason::WATCHDOG_TIMEOUT_SYSTEM_RESET;
    return WakeupReason::UNKNOWN;
}
void ChronosPowerManager::platform_synchronizeSystemTime(uint32_t ms) {}
Status ChronosPowerManager::platform_getInternalTemperature(float& temp) { adc_init(); adc_set_temp_sensor_enabled(true); temp = 27.0f - (adc_read() * 3.3f / 4096 - 0.706f) / 0.001721f; adc_set_temp_sensor_enabled(false); return Status::OK; }
Status ChronosPowerManager::platform_getCoreVoltage(float& voltage) { return Status::FAILURE_UNSUPPORTED_FEATURE; }

#else
Status ChronosPowerManager::platform_setCpuFrequency(uint32_t f) { return Status::FAILURE_UNSUPPORTED_ARCH; }
uint32_t ChronosPowerManager::platform_getCpuFrequency() const { return 0; }
Status ChronosPowerManager::platform_control(Peripheral p, bool e) { return Status::FAILURE_UNSUPPORTED_ARCH; }
void ChronosPowerManager::platform_sleep(const WakeupPolicy& p) { delay(1000); }
void ChronosPowerManager::platform_hibernate(const WakeupPolicy& p) { for(;;); }
WakeupReason ChronosPowerManager::platform_getWakeupReason() { return WakeupReason::UNKNOWN; }
void ChronosPowerManager::platform_synchronizeSystemTime(uint32_t ms) {}
Status ChronosPowerManager::platform_getInternalTemperature(float& temp) { return Status::FAILURE_UNSUPPORTED_ARCH; }
Status ChronosPowerManager::platform_getCoreVoltage(float& voltage) { return Status::FAILURE_UNSUPPORTED_ARCH; }
#endif
}
ChronosPower::ChronosPowerManager CPower;
