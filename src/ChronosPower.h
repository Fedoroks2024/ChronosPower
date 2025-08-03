#ifndef CHRONOS_POWER_V25_H
#define CHRONOS_POWER_V25_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <functional>
#include <vector>
#include <memory>
#include <map>
#include <string>


#if defined(__AVR__)
#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#elif defined(ESP32)
#include <esp_sleep.h>
#include <esp_wifi.h>
#include <esp_bt.h>
#include <driver/rtc_io.h>
#include <soc/rtc.h>
#include <esp_pm.h>
#include <esp_log.h>
#include <esp_deep_sleep.h>
#include <esp_system.h>
#include <esp_cpu.h>
#elif defined(ARDUINO_ARCH_RP2040)
#include <hardware/pll.h>
#include <hardware/clocks.h>
#include <hardware/structs/scb.h>
#include <hardware/watchdog.h>
#include <hardware/rtc.h>
#include <hardware/adc.h>
#include <hardware/vreg.h>
#include <hardware/rosc.h>
#include <hardware/xosc.h>
#elif defined(ARDUINO_ARCH_STM32)
#include <stm32_def.h>
#include <LowPower.h>
#endif

namespace ChronosPower {


enum class Status : int16_t {
    OK = 0,
    
    
    FAILURE_GENERIC = -1,
    FAILURE_UNSUPPORTED_ARCH = -2,
    FAILURE_UNSUPPORTED_FEATURE = -3,
    FAILURE_INVALID_PARAMETER = -4,
    FAILURE_TIMEOUT = -5,
    FAILURE_INSUFFICIENT_MEMORY = -6,
    
    
    FAILURE_SYSTEM_STATE_INVALID = -10,
    FAILURE_CONFIG_LOCKED = -11,
    FAILURE_PERIPHERAL_ACTIVE = -12,
    FAILURE_REGISTER_WRITE_PROTECTED = -13,
    FAILURE_HARDWARE_FAULT = -14,
    
    
    FAILURE_INVALID_FREQUENCY = -20,
    FAILURE_PLL_LOCK_FAILED = -21,
    FAILURE_OSCILLATOR_FAILED = -22,
    FAILURE_CLOCK_GATING_FAILED = -23,
    FAILURE_PRECISION_UNATTAINABLE = -24,
    
    
    FAILURE_VOLTAGE_SCALE = -30,
    FAILURE_FLASH_POWERDOWN = -31,
    FAILURE_BOD_CONFIG = -32,
    FAILURE_THERMAL_PROTECTION = -33,
    FAILURE_POWER_DOMAIN_LOCKED = -34,
    
    
    FAILURE_NO_WAKEUP_SOURCE = -40,
    FAILURE_WAKEUP_CONFLICT = -41,
    FAILURE_RTC_UNINITIALIZED = -42,
    FAILURE_TIMER_UNAVAILABLE = -43,
    FAILURE_INTERRUPT_MAP = -44,
    
    
    FAILURE_PERIPHERAL_NOT_FOUND = -50,
    FAILURE_DMA_CHANNEL_ACTIVE = -51,
    FAILURE_CALLBACK_NOT_SET = -52,
    
    
    FAILURE_ADAPTIVE_SCALING_DISABLED = -60,
    FAILURE_ENERGY_BUDGET_EXCEEDED = -61,
    FAILURE_THERMAL_THROTTLING_ACTIVE = -62,
    FAILURE_BATTERY_CRITICAL = -63,
    FAILURE_SECURITY_VIOLATION = -64,
    
    
    FAILURE_POWER_RAIL_UNSTABLE = -70,
    FAILURE_CLOCK_DOMAIN_CONFLICT = -71,
    FAILURE_VOLTAGE_REGULATOR_FAULT = -72,
    FAILURE_POWER_ISLAND_ISOLATION = -73,
    FAILURE_DYNAMIC_SCALING_OVERFLOW = -74
};


enum class PowerProfile : uint8_t {
    
    MAX_PERFORMANCE,
    HIGH_PERFORMANCE,
    BALANCED_DESKTOP,
    BALANCED_MOBILE,
    LOW_POWER_ACTIVE,
    ULTRA_LOW_POWER,
    DEEP_SLEEP_PREP,
    HIBERNATION_PREP,
    SHUTDOWN_PREP,
    
    
    MACHINE_LEARNING_INFERENCE,
    AUDIO_PROCESSING,
    VIDEO_PROCESSING,
    DATA_LOGGING,
    SENSOR_FUSION,
    COMMUNICATION_INTENSIVE,
    COMPUTATION_INTENSIVE,
    
    
    ADAPTIVE_PERFORMANCE,      
    ADAPTIVE_BATTERY_LIFE,     
    ADAPTIVE_THERMAL,          
    ADAPTIVE_WORKLOAD,         
    ADAPTIVE_ENERGY_HARVEST,   
    
    
    QUANTUM_EFFICIENCY,        
    NEURAL_POWER_MANAGEMENT,   
    PREDICTIVE_SCALING,        
    ENERGY_HARVESTING_SOLAR,   
    ENERGY_HARVESTING_KINETIC, 
    ENERGY_HARVESTING_RF,      
    BATTERY_PRESERVATION,      
    EMERGENCY_POWER,           
    
    CUSTOM_POLICY
};


enum class PowerDomain : uint8_t {
    CORE_LOGIC,
    MEMORY_SYSTEM,
    CACHE_SYSTEM,
    ANALOG_SUBSYSTEM,
    DIGITAL_PERIPHERALS,
    COMMUNICATIONS,
    TIMERS_AND_PWM,
    GPIO_PORTS,
    SECURITY_MODULES,
    MULTIMEDIA,
    RADIO_SUBSYSTEM,
    SENSOR_INTERFACES,
    MOTOR_CONTROL,
    HIGH_SPEED_IO,
    LOW_POWER_PERIPHERALS,
    DEBUG_INTERFACES,
    
    
    CPU_CORE_0, CPU_CORE_1, CPU_CORE_2, CPU_CORE_3,
    L1_CACHE, L2_CACHE, L3_CACHE,
    MEMORY_CONTROLLER, DMA_CONTROLLER,
    VOLTAGE_REGULATORS, CLOCK_GENERATORS,
    POWER_ISLANDS, RETENTION_LOGIC,
    
    ALL
};


enum class Peripheral : uint16_t {
    
    SYSTEM_CONTROLLER = 0,
    CPU_CORE_0, CPU_CORE_1, CPU_CORE_2, CPU_CORE_3,
    CPU_CORE_4, CPU_CORE_5, CPU_CORE_6, CPU_CORE_7,
    CPU_CLUSTER_0, CPU_CLUSTER_1,
    
    
    FPU_SINGLE_PRECISION = 10, FPU_DOUBLE_PRECISION,
    DSP_INSTRUCTION_SET, VECTOR_PROCESSING_UNIT,
    NEURAL_PROCESSING_UNIT, CRYPTO_ACCELERATOR,
    AI_ACCELERATOR, ML_COPROCESSOR,
    QUANTUM_PROCESSOR, TENSOR_UNIT,
    
    
    FLASH_CONTROLLER = 30, EEPROM_CONTROLLER,
    SRAM_CONTROLLER, CACHE_CONTROLLER,
    MMU_UNIT, MPU_UNIT, IOMMU_UNIT,
    MEMORY_ENCRYPTION_UNIT, ECC_CONTROLLER,
    
    
    SRAM_BANK_0 = 60, SRAM_BANK_1, SRAM_BANK_2, SRAM_BANK_3,
    SRAM_BANK_4, SRAM_BANK_5, SRAM_BANK_6, SRAM_BANK_7,
    SRAM_BANK_8, SRAM_BANK_9, SRAM_BANK_10, SRAM_BANK_11,
    SRAM_BANK_12, SRAM_BANK_13, SRAM_BANK_14, SRAM_BANK_15,
    SRAM_LOW_LEAKAGE, SRAM_RTC_MEMORY, SRAM_BACKUP,
    SRAM_ULTRA_LOW_POWER, SRAM_RETENTION,
    
    
    FLASH_CACHE_INSTRUCTION = 100, FLASH_CACHE_DATA,
    L1_CACHE_INSTRUCTION, L1_CACHE_DATA,
    L2_CACHE_UNIFIED, L3_CACHE_SHARED,
    CACHE_COHERENCY_UNIT, CACHE_PREFETCHER,
    
    
    DMA_CONTROLLER_0 = 120, DMA_CONTROLLER_1, DMA_CONTROLLER_2,
    DMA_CHANNEL_0, DMA_CHANNEL_1, DMA_CHANNEL_2, DMA_CHANNEL_3,
    DMA_CHANNEL_4, DMA_CHANNEL_5, DMA_CHANNEL_6, DMA_CHANNEL_7,
    DMA_CHANNEL_8, DMA_CHANNEL_9, DMA_CHANNEL_10, DMA_CHANNEL_11,
    DMA_CHANNEL_12, DMA_CHANNEL_13, DMA_CHANNEL_14, DMA_CHANNEL_15,
    DMA_2D_GRAPHICS, DMA_CRYPTO, DMA_AUDIO, DMA_VIDEO,
    DMA_SCATTER_GATHER, DMA_LINKED_LIST,
    
    
    PLL_MAIN_SYSTEM = 160, PLL_USB, PLL_AUDIO, PLL_ETHERNET,
    PLL_SAI, PLL_I2S, PLL_MIPI, PLL_GPU, PLL_AI,
    OSC_INTERNAL_RC_8MHZ, OSC_INTERNAL_RC_48MHZ, OSC_INTERNAL_RC_32KHZ,
    OSC_EXTERNAL_CRYSTAL_HS, OSC_EXTERNAL_CRYSTAL_LS,
    OSC_EXTERNAL_BYPASS, OSC_CERAMIC_RESONATOR,
    CLOCK_DIVIDER_0, CLOCK_DIVIDER_1, CLOCK_DIVIDER_2,
    VOLTAGE_REGULATOR_CORE, VOLTAGE_REGULATOR_IO,
    VOLTAGE_REGULATOR_ANALOG, VOLTAGE_REGULATOR_RF,
    POWER_SWITCH_0, POWER_SWITCH_1, POWER_SWITCH_2,
    
    
    TIMER_0_8BIT_BASIC = 200, TIMER_1_16BIT_GENERAL, TIMER_2_8BIT_ASYNC,
    TIMER_3_16BIT_GENERAL, TIMER_4_16BIT_GENERAL, TIMER_5_16BIT_GENERAL,
    TIMER_6_16BIT_ADVANCED, TIMER_7_16BIT_ADVANCED, TIMER_8_LOW_POWER,
    TIMER_9_32BIT_GENERAL, TIMER_10_32BIT_GENERAL, TIMER_11_HRTIM,
    TIMER_12_SYSTICK, TIMER_13_WATCHDOG, TIMER_14_RTC,
    TIMER_15_DEADTIME, TIMER_16_CAPTURE_COMPARE,
    TIMER_17_ULTRA_LOW_POWER, TIMER_18_PRECISION,
    TIMER_19_QUANTUM, TIMER_20_AI_SCHEDULER,
    
    
    ADC_0_12BIT = 250, ADC_1_12BIT, ADC_2_16BIT, ADC_3_10BIT,
    ADC_4_24BIT_SIGMA_DELTA, ADC_INTERNAL_TEMP, ADC_INTERNAL_VREF,
    ADC_INTERNAL_VBAT, ADC_DIFFERENTIAL_0, ADC_DIFFERENTIAL_1,
    ADC_SAR_0, ADC_SAR_1, ADC_PIPELINE, ADC_FLASH,
    DAC_0_8BIT, DAC_1_12BIT, DAC_2_16BIT, DAC_AUDIO_STEREO,
    DAC_CURRENT_OUTPUT, DAC_VOLTAGE_OUTPUT,
    
    
    ANALOG_COMPARATOR_0 = 300, ANALOG_COMPARATOR_1, ANALOG_COMPARATOR_2,
    ANALOG_COMPARATOR_3, OPAMP_0, OPAMP_1, OPAMP_2, OPAMP_3,
    VOLTAGE_REFERENCE_INTERNAL, VOLTAGE_REFERENCE_EXTERNAL,
    CURRENT_REFERENCE, BANDGAP_REFERENCE,
    PGA_0, PGA_1, INSTRUMENTATION_AMP,
    ANALOG_SWITCH_MATRIX, ANALOG_MUX,
    
    
    UART_0 = 350, UART_1, UART_2, UART_3, UART_4, UART_5, UART_6, UART_7,
    UART_LP_0, UART_LP_1, UART_ISO7816, UART_LIN,
    SPI_0, SPI_1, SPI_2, SPI_3_QUAD, SPI_4, SPI_5, SPI_6_OCTO,
    I2C_0, I2C_1, I2C_2, I2C_3_FAST_MODE_PLUS, I2C_4_SMBUS,
    I2S_0_STD, I2S_1_STD, I2S_2_TDM, I2S_3_PDM,
    SAI_0, SAI_1, SAI_2_SPDIF,
    UART_9, UART_10, UART_11, UART_12,
    SPI_7, SPI_8, SPI_9, SPI_10,
    I2C_5, I2C_6, I2C_7, I2C_8,
    
    
    USB_DEVICE = 450, USB_HOST, USB_OTG_FS, USB_OTG_HS,
    USB_HS_PHY, USB_TYPE_C_CONTROLLER, USB_PD_CONTROLLER,
    USB_3_0_CONTROLLER, USB_4_CONTROLLER,
    
    
    ETHERNET_MAC = 470, ETHERNET_PHY_INTERNAL, ETHERNET_PHY_EXTERNAL,
    ETHERNET_PTP, ETHERNET_TSN, WIFI_MAC_2_4GHZ, WIFI_PHY_2_4GHZ,
    WIFI_MAC_5GHZ, WIFI_PHY_5GHZ, WIFI_MAC_6GHZ, WIFI_PHY_6GHZ,
    BLUETOOTH_LE_PHY, BLUETOOTH_CLASSIC_PHY, BLUETOOTH_CONTROLLER,
    RADIO_802_15_4, RADIO_LORA, RADIO_SIGFOX, RADIO_NB_IOT,
    RADIO_LTE_M, RADIO_5G_NR, RADIO_SATELLITE,
    
    
    CAN_BUS_0 = 520, CAN_BUS_1, CAN_BUS_FD, CAN_BUS_XL,
    LIN_BUS_0, LIN_BUS_1, FLEXRAY_CONTROLLER,
    AUTOMOTIVE_ETHERNET, MOST_CONTROLLER,
    
    
    SDIO_0 = 540, SDIO_1, EMMC_CONTROLLER, NAND_CONTROLLER,
    NOR_FLASH_CONTROLLER, QSPI_CONTROLLER, OSPI_CONTROLLER,
    SATA_CONTROLLER, NVME_CONTROLLER,
    
    
    GPIO_PORT_A = 560, GPIO_PORT_B, GPIO_PORT_C, GPIO_PORT_D,
    GPIO_PORT_E, GPIO_PORT_F, GPIO_PORT_G, GPIO_PORT_H,
    GPIO_PORT_I, GPIO_PORT_J, GPIO_PORT_K, GPIO_PORT_L,
    GPIO_PORT_M, GPIO_PORT_N, GPIO_PORT_O, GPIO_PORT_P,
    
    
    PWM_MODULE_0 = 600, PWM_MODULE_1, PWM_MODULE_2_MOTOR_CONTROL,
    PWM_MODULE_3_ADVANCED, QUADRATURE_ENCODER_0, QUADRATURE_ENCODER_1,
    HALL_SENSOR_INTERFACE, RESOLVER_INTERFACE,
    MOTOR_CONTROL_UNIT, SERVO_CONTROLLER,
    
    
    AES_ACCELERATOR_128 = 630, AES_ACCELERATOR_256, AES_ACCELERATOR_512,
    SHA_ACCELERATOR_256, SHA_ACCELERATOR_512, SHA_ACCELERATOR_1024,
    TRNG_UNIT, PKA_UNIT, RSA_ACCELERATOR, ECC_ACCELERATOR,
    SECURE_BOOT_CONTROLLER, TAMPER_DETECTION,
    HSM_UNIT, SECURE_ENCLAVE, QUANTUM_CRYPTO,
    
    
    LCD_CONTROLLER_PARALLEL = 680, LCD_CONTROLLER_SPI, LCD_CONTROLLER_MIPI,
    MIPI_DSI_INTERFACE, MIPI_CSI_INTERFACE, CAMERA_INTERFACE_DCMI,
    CAMERA_INTERFACE_PARALLEL, JPEG_CODEC, H264_CODEC, H265_CODEC,
    GPU_2D, GPU_3D, VIDEO_SCALER, DISPLAY_PROCESSOR,
    
    
    AUDIO_CODEC = 730, AUDIO_DSP, AUDIO_MIXER, AUDIO_EFFECTS,
    MICROPHONE_INTERFACE, SPEAKER_AMPLIFIER,
    AUDIO_NEURAL_PROCESSOR, VOICE_RECOGNITION,
    
    
    TOUCH_SENSOR_UNIT = 760, TOUCH_PAD_0, TOUCH_PAD_1, TOUCH_PAD_2,
    TOUCH_PAD_3, TOUCH_PAD_4, TOUCH_PAD_5, TOUCH_PAD_6, TOUCH_PAD_7,
    ACCELEROMETER_INTERFACE, GYROSCOPE_INTERFACE, MAGNETOMETER_INTERFACE,
    PRESSURE_SENSOR_INTERFACE, HUMIDITY_SENSOR_INTERFACE,
    LIDAR_INTERFACE, RADAR_INTERFACE, ULTRASONIC_INTERFACE,
    
    
    POWER_MANAGEMENT_UNIT = 800, VOLTAGE_REGULATOR_CORE,
    VOLTAGE_REGULATOR_IO, VOLTAGE_REGULATOR_ANALOG,
    BATTERY_CHARGER, FUEL_GAUGE, THERMAL_SENSOR,
    POWER_SEQUENCER, LOAD_SWITCH, ENERGY_HARVESTER,
    
    
    DEBUG_INTERFACE = 830, JTAG_CONTROLLER, SWD_CONTROLLER,
    TRACE_PORT_INTERFACE, INSTRUMENTATION_TRACE,
    PERFORMANCE_MONITOR, POWER_ANALYZER,
    
    
    ULP_COPROCESSOR = 850, RISC_V_COPROCESSOR, ARM_CORTEX_M0_PLUS,
    NEURAL_COPROCESSOR, QUANTUM_COPROCESSOR,
    
    
    BROWN_OUT_DETECTOR = 880, WATCHDOG_SYSTEM, WATCHDOG_WINDOWED,
    INTERNAL_TEMP_SENSOR, INTERNAL_VOLTAGE_REF, CLOCK_MONITOR,
    POWER_MONITOR, CURRENT_MONITOR, VOLTAGE_MONITOR,
    
    
    NFC_CONTROLLER_TYPE_A = 910, NFC_CONTROLLER_TYPE_B,
    NFC_CONTROLLER_TYPE_F, RFID_CONTROLLER_125KHZ,
    RFID_CONTROLLER_13_56MHZ, NFC_SECURE_ELEMENT,
    
    
    AI_TENSOR_UNIT = 930, ML_INFERENCE_ENGINE, NEURAL_NETWORK_ACCELERATOR,
    DEEP_LEARNING_PROCESSOR, COMPUTER_VISION_UNIT,
    NATURAL_LANGUAGE_PROCESSOR, EDGE_AI_UNIT,
    
    
    SOLAR_ENERGY_HARVESTER = 960, KINETIC_ENERGY_HARVESTER,
    RF_ENERGY_HARVESTER, THERMAL_ENERGY_HARVESTER,
    PIEZOELECTRIC_HARVESTER, ELECTROMAGNETIC_HARVESTER,
    
    
    QUANTUM_SENSOR = 980, QUANTUM_COMMUNICATION,
    QUANTUM_RANDOM_GENERATOR, QUANTUM_ENTANGLEMENT_UNIT,
    MOLECULAR_SENSOR, NANO_SENSOR,
    
    
    RTC_INTERNAL = 995,
    PSEUDO_PERIPHERAL_SERIAL_PRINT = 996,
    PSEUDO_PERIPHERAL_ARDUINO_MILLIS = 997,
    PSEUDO_PERIPHERAL_WIRE_LIBRARY = 998,
    PSEUDO_PERIPHERAL_SPI_LIBRARY = 999,
    
    PERIPHERAL_COUNT = 1000
};


enum class WakeupReason : uint16_t {
    UNKNOWN = 0,
    POWER_ON_RESET,
    
    
    EXTERNAL_PIN_0 = 2, EXTERNAL_PIN_1, EXTERNAL_PIN_2, EXTERNAL_PIN_3,
    EXTERNAL_PIN_4, EXTERNAL_PIN_5, EXTERNAL_PIN_6, EXTERNAL_PIN_7,
    EXTERNAL_PIN_8, EXTERNAL_PIN_9, EXTERNAL_PIN_10, EXTERNAL_PIN_11,
    EXTERNAL_PIN_12, EXTERNAL_PIN_13, EXTERNAL_PIN_14, EXTERNAL_PIN_15,
    EXTERNAL_PIN_16, EXTERNAL_PIN_17, EXTERNAL_PIN_18, EXTERNAL_PIN_19,
    EXTERNAL_PIN_20, EXTERNAL_PIN_21, EXTERNAL_PIN_22, EXTERNAL_PIN_23,
    EXTERNAL_PIN_24, EXTERNAL_PIN_25, EXTERNAL_PIN_26, EXTERNAL_PIN_27,
    EXTERNAL_PIN_28, EXTERNAL_PIN_29, EXTERNAL_PIN_30, EXTERNAL_PIN_31,
    EXTERNAL_PIN_MULTIPLE, EXTERNAL_PIN_CHANGE_DETECTION,
    
    
    RTC_ALARM_SECONDS = 50, RTC_ALARM_MINUTES, RTC_ALARM_HOURS,
    RTC_ALARM_DATE, RTC_ALARM_WEEK, RTC_ALARM_MONTH,
    RTC_PERIODIC_INTERRUPT, RTC_CALIBRATION_COMPLETE,
    RTC_TAMPER_DETECTION, RTC_TIMESTAMP_EVENT,
    
    
    WATCHDOG_TIMEOUT_SYSTEM_RESET = 80, WATCHDOG_TIMEOUT_INTERRUPT,
    WATCHDOG_EARLY_WARNING, WATCHDOG_WINDOW_VIOLATION,
    WATCHDOG_REFRESH_ERROR, WATCHDOG_CONFIGURATION_ERROR,
    
    
    TOUCH_SENSOR_GLOBAL = 100, TOUCH_SENSOR_PAD_0, TOUCH_SENSOR_PAD_1,
    TOUCH_SENSOR_PAD_2, TOUCH_SENSOR_PAD_3, TOUCH_SENSOR_PAD_4,
    TOUCH_SENSOR_PAD_5, TOUCH_SENSOR_PAD_6, TOUCH_SENSOR_PAD_7,
    TOUCH_SENSOR_PAD_8, TOUCH_SENSOR_PAD_9, TOUCH_SENSOR_PROXIMITY,
    TOUCH_SENSOR_GESTURE, TOUCH_SENSOR_FORCE,
    
    
    UART_RX_ACTIVITY_0 = 130, UART_RX_ACTIVITY_1, UART_RX_ACTIVITY_2,
    UART_RX_ACTIVITY_3, UART_RX_ADDRESS_MATCH_0, UART_RX_ADDRESS_MATCH_1,
    UART_BREAK_DETECTION, UART_IDLE_LINE_DETECTION,
    I2C_SLAVE_ADDR_MATCH_0, I2C_SLAVE_ADDR_MATCH_1, I2C_SLAVE_GENERAL_CALL,
    I2C_SMBUS_ALERT, SPI_SLAVE_CS_LOW, SPI_SLAVE_DATA_RECEIVED,
    CAN_BUS_MESSAGE_RX, CAN_BUS_ERROR, CAN_BUS_WAKEUP,
    
    
    TIMER_COMPARE_MATCH_A = 180, TIMER_COMPARE_MATCH_B, TIMER_OVERFLOW,
    TIMER_CAPTURE_EVENT, PWM_FAULT_DETECTION, PWM_PERIOD_COMPLETE,
    TIMER_QUANTUM_EVENT, TIMER_AI_TRIGGER,
    
    
    ANALOG_COMPARATOR_0_RISE = 200, ANALOG_COMPARATOR_0_FALL,
    ANALOG_COMPARATOR_1_RISE, ANALOG_COMPARATOR_1_FALL,
    ADC_WINDOW_OUTSIDE, ADC_WINDOW_INSIDE, ADC_CONVERSION_COMPLETE,
    ADC_OVERRUN, DAC_UNDERRUN, ADC_THRESHOLD_CROSSED,
    
    
    DMA_CHANNEL_0_COMPLETE = 230, DMA_CHANNEL_1_COMPLETE,
    DMA_CHANNEL_2_COMPLETE, DMA_CHANNEL_3_COMPLETE, DMA_ERROR,
    DMA_FIFO_ERROR, DMA_TRANSFER_ERROR, DMA_SCATTER_GATHER_COMPLETE,
    
    
    BROWN_OUT_RESET_LEVEL1 = 250, BROWN_OUT_RESET_LEVEL2,
    THERMAL_SHUTDOWN, THERMAL_WARNING_HIGH, THERMAL_WARNING_LOW,
    VOLTAGE_GLITCH, POWER_SUPPLY_FAILURE, BATTERY_LOW,
    BATTERY_CRITICAL, ENERGY_HARVESTER_ACTIVE,
    
    
    SOFTWARE_RESET_USER = 280, SOFTWARE_RESET_EXCEPTION,
    SYSTEM_FAULT, MEMORY_FAULT, BUS_FAULT, USAGE_FAULT,
    STACK_OVERFLOW, HEAP_CORRUPTION,
    
    
    ADAPTIVE_TIMER_OPTIMIZED = 300, ENERGY_BUDGET_ALARM,
    WORKLOAD_PREDICTION_TRIGGER, THERMAL_OPTIMIZATION_REQUIRED,
    BATTERY_OPTIMIZATION_TRIGGER, PERFORMANCE_BOOST_REQUIRED,
    AI_INFERENCE_COMPLETE, ML_MODEL_UPDATE,
    QUANTUM_STATE_CHANGE, NEURAL_PATTERN_DETECTED
};


enum class ResetReason : uint8_t {
    UNKNOWN = 0,
    POWER_ON_RESET,
    EXTERNAL_RESET_PIN,
    SOFTWARE_RESET_USER,
    SOFTWARE_RESET_EXCEPTION,
    WATCHDOG_RESET,
    BROWN_OUT_RESET,
    LOCKUP_RESET,
    CLOCK_FAILURE_SYSTEM,
    CLOCK_FAILURE_WATCHDOG,
    DEEP_SLEEP_HIBERNATE,
    JTAG_DEBUG_RESET,
    THERMAL_SHUTDOWN_RESET,
    SECURITY_VIOLATION_RESET,
    MEMORY_PROTECTION_FAULT,
    STACK_OVERFLOW_RESET,
    HEAP_CORRUPTION_RESET,
    FIRMWARE_UPDATE_RESET,
    QUANTUM_ERROR_RESET,
    AI_SAFETY_RESET
};

enum class FlashState : uint8_t {
    ACTIVE,
    IDLE,
    DEEP_POWER_DOWN,
    STANDBY,
    RETENTION,
    UNKNOWN
};


enum class SystemClockSource : uint8_t {
    INTERNAL_RC_FAST,
    INTERNAL_RC_SLOW,
    INTERNAL_RC_ULTRA_LOW_POWER,
    EXTERNAL_CRYSTAL,
    EXTERNAL_OSCILLATOR,
    PLL_INTERNAL,
    PLL_EXTERNAL,
    PLL_FRACTIONAL,
    QUANTUM_OSCILLATOR,
    AUTO_SELECT
};


struct EnergyMetrics {
    float currentConsumptionMicroAmps = 0.0f;
    float averageConsumptionMicroAmps = 0.0f;
    float peakConsumptionMicroAmps = 0.0f;
    float minimumConsumptionMicroAmps = 1000000.0f;
    float energyConsumedMicroWattHours = 0.0f;
    float energyHarvestedMicroWattHours = 0.0f;
    float batteryVoltage = 0.0f;
    float batteryPercentage = 100.0f;
    float batteryHealthPercentage = 100.0f;
    uint32_t uptimeSeconds = 0;
    uint32_t sleepTimeSeconds = 0;
    uint32_t deepSleepTimeSeconds = 0;
    float powerEfficiencyPercent = 0.0f;
    float energyEfficiencyPercent = 0.0f;
    uint32_t wakeupCount = 0;
    uint32_t sleepCycleCount = 0;
    float averageSleepDurationMs = 0.0f;
    float carbonFootprintGrams = 0.0f; 
};


struct ThermalMetrics {
    float currentTemperatureCelsius = 25.0f;
    float maxTemperatureCelsius = 25.0f;
    float minTemperatureCelsius = 25.0f;
    float averageTemperatureCelsius = 25.0f;
    float ambientTemperatureCelsius = 25.0f;
    bool thermalThrottlingActive = false;
    uint8_t thermalZone = 0;
    uint8_t thermalThrottlingLevel = 0;
    float thermalResistance = 0.0f;
    float heatDissipationWatts = 0.0f;
    float junctionTemperatureCelsius = 25.0f;
    uint32_t thermalCycleCount = 0;
    float thermalStress = 0.0f;
};


struct PerformanceMetrics {
    uint32_t cpuUtilizationPercent = 0;
    uint32_t memoryUtilizationPercent = 0;
    uint32_t cacheHitRatePercent = 100;
    uint32_t interruptsPerSecond = 0;
    uint32_t contextSwitchesPerSecond = 0;
    uint32_t instructionsPerSecond = 0;
    uint32_t cyclesPerInstruction = 1;
    float mipsRating = 0.0f;
    float flopsRating = 0.0f;
    uint32_t branchMispredictionRate = 0;
    uint32_t tlbMissRate = 0;
    uint32_t busUtilizationPercent = 0;
    float aiInferenceRate = 0.0f;
    float quantumCoherenceTime = 0.0f;
};


struct ExternalInterruptConfig {
    uint64_t pinMask = 0;
    int triggerMode = 0;
    uint32_t debounceMicros = 0;
    bool enablePullup = true;
    bool enablePulldown = false;
    uint8_t priority = 0;
    bool enableGlitchFilter = false;
    uint32_t glitchFilterMicros = 0;
    bool enableEdgeDetection = true;
    bool enableLevelDetection = false;
};

struct RtcConfig {
    uint32_t timeoutSeconds = 0;
    uint8_t alarmHour = 0, alarmMinute = 0, alarmSecond = 0;
    bool matchDate = false;
    uint8_t alarmDay = 0, alarmMonth = 0;
    bool enableCalibration = false;
    int8_t calibrationPpm = 0;
    bool enableTamperDetection = false;
    bool enableTimestamp = false;
    uint32_t wakeupCounter = 0;
    bool enableSubSecondAlarm = false;
};

struct WatchdogConfig {
    uint32_t timeoutMillis = 0;
    bool generateInterrupt = true;
    bool windowedMode = false;
    uint8_t windowPercent = 50;
    bool enableInSleep = false;
    bool enableInDebug = false;
    bool enableEarlyWarning = false;
    uint32_t earlyWarningMillis = 0;
    bool enableRefreshProtection = false;
};

struct UartWakeupConfig {
    uint8_t portIndex = 0;
    bool wakeupOnStartBit = true;
    bool wakeupOnAddressMatch = false;
    uint8_t address = 0;
    uint32_t baudRate = 9600;
    bool enableDMA = false;
    bool enableFifo = false;
    uint8_t fifoThreshold = 1;
    bool enableIdleDetection = false;
    uint32_t idleTimeoutMicros = 0;
};

struct ThermalPolicy {
    bool enabled = false;
    float warningTempCelsius = 85.0f;
    float shutdownTempCelsius = 105.0f;
    float hysteresisCelsius = 5.0f;
    bool enableThrottling = true;
    uint8_t throttlingSteps = 4;
    float throttlingStepPercent = 25.0f;
    bool enableFanControl = false;
    uint8_t fanSpeedPercent = 0;
    bool enableThermalShutdown = true;
    float emergencyTempCelsius = 125.0f;
};

struct VoltagePolicy {
    bool enabled = false;
    float warningVoltage = 2.8f;
    float shutdownVoltage = 2.5f;
    float hysteresisVoltage = 0.1f;
    bool enableBrownOutDetection = true;
    float brownOutThreshold = 2.7f;
    bool enableVoltageScaling = true;
    float nominalVoltage = 3.3f;
    float maxVoltage = 3.6f;
    float minVoltage = 1.8f;
};


struct AdaptiveScalingConfig {
    bool enabled = false;
    uint32_t samplingIntervalMs = 1000;
    uint8_t performanceThresholdPercent = 80;
    uint8_t powerThresholdPercent = 20;
    float temperatureThresholdCelsius = 70.0f;
    bool enableWorkloadPrediction = false;
    bool enableEnergyBudgeting = false;
    bool enableThermalAwareness = true;
    bool enableBatteryAwareness = true;
    bool enableQuantumOptimization = false;
    bool enableNeuralNetworkScaling = false;
    float learningRate = 0.01f;
    uint32_t predictionWindowMs = 10000;
    uint8_t scalingAggressiveness = 50; 
    bool enablePredictivePreemption = false;
    float confidenceThreshold = 0.8f;
};


struct EnergyBudget {
    float dailyBudgetMicroWattHours = 1000000.0f; 
    float hourlyBudgetMicroWattHours = 41666.7f;  
    float currentConsumptionLimit = 10000.0f;     
    bool enableStrictMode = false;
    bool enablePredictiveScaling = true;
    bool enableEnergyHarvesting = false;
    float harvestingEfficiency = 0.8f;
    float solarPanelWatts = 0.0f;
    float kineticHarvesterWatts = 0.0f;
    float rfHarvesterWatts = 0.0f;
    bool enableBatteryProtection = true;
    float batteryCapacityMah = 1000.0f;
    float batteryNominalVoltage = 3.7f;
    uint32_t batteryLifeCycles = 500;
    bool enableCarbonTracking = false;
};


struct QuantumPowerConfig {
    bool enabled = false;
    float coherenceTime = 0.0f;
    float entanglementStrength = 0.0f;
    uint8_t quantumStates = 2;
    bool enableSuperposition = false;
    bool enableQuantumTunneling = false;
    float tunnelingProbability = 0.1f;
    bool enableQuantumAnnealing = false;
    float annealingTemperature = 1.0f;
    uint32_t quantumIterations = 100;
};


struct NeuralPowerConfig {
    bool enabled = false;
    uint8_t networkLayers = 3;
    uint16_t neuronsPerLayer = 10;
    float learningRate = 0.001f;
    uint32_t trainingEpochs = 1000;
    bool enableOnlineTraining = false;
    bool enableTransferLearning = false;
    float dropoutRate = 0.2f;
    bool enableBatchNormalization = true;
    uint8_t activationFunction = 0; 
};

struct WakeupPolicy {
    ExternalInterruptConfig extInterrupt;
    RtcConfig rtc;
    WatchdogConfig wdt;
    UartWakeupConfig uartWakeup;
    uint32_t touchThreshold = 0;
    bool enableUlpWakeup = false;
    bool enableWifiWakeup = false;
    bool enableBluetoothWakeup = false;
    bool enableAnalogComparatorWakeup = false;
    bool enableCanWakeup = false;
    bool enableUsbWakeup = false;
    ThermalPolicy thermalPolicy;
    VoltagePolicy voltagePolicy;
    
    
    bool enableIntelligentWakeup = false;
    uint32_t maxSleepDurationMs = 0; 
    bool enableWakeupLogging = false;
    bool enableWakeupPrediction = false;
    bool enableAdaptiveWakeup = false;
    float wakeupPredictionAccuracy = 0.0f;
    uint32_t wakeupHistorySize = 100;
    bool enableQuantumWakeup = false;
    bool enableNeuralWakeup = false;
};

struct PowerPolicy {
    uint32_t cpuFrequencyHz = 0;
    bool flashDeepPowerDown = false;
    int8_t voltageScaleLevel = 0;
    uint64_t peripheralMask[16] = {0}; 
    
    
    SystemClockSource clockSource = SystemClockSource::AUTO_SELECT;
    bool enableDynamicVoltageScaling = false;
    bool enableClockGating = true;
    bool enablePowerGating = false;
    bool enableRetentionMode = false;
    AdaptiveScalingConfig adaptiveScaling;
    EnergyBudget energyBudget;
    QuantumPowerConfig quantumConfig;
    NeuralPowerConfig neuralConfig;
    uint8_t performanceLevel = 50; 
    uint8_t powerEfficiencyLevel = 50; 
    
    
    bool enableQuantumOptimization = false;
    bool enableNeuralOptimization = false;
    bool enablePredictiveOptimization = false;
    bool enableMolecularOptimization = false;
    float optimizationAggression = 0.5f; 
    uint32_t optimizationWindow = 1000; 
    bool enableMultiObjectiveOptimization = false;
    float performanceWeight = 0.5f;
    float powerWeight = 0.3f;
    float thermalWeight = 0.2f;
};


typedef std::function<void(WakeupReason)> WakeupCallback;
typedef std::function<void(Status, const char*)> ErrorCallback;
typedef std::function<void(const EnergyMetrics&)> EnergyCallback;
typedef std::function<void(const ThermalMetrics&)> ThermalCallback;
typedef std::function<void(const PerformanceMetrics&)> PerformanceCallback;
typedef std::function<void(float)> BatteryCallback;
typedef std::function<void(uint32_t)> FrequencyCallback;
typedef std::function<void(const PowerPolicy&)> PolicyCallback;


enum class LogLevel : uint8_t {
    NONE = 0,
    CRITICAL = 1,
    ERROR = 2,
    WARNING = 3,
    INFO = 4,
    DEBUG = 5,
    VERBOSE = 6,
    TRACE = 7
};

struct LogEntry {
    uint32_t timestamp;
    LogLevel level;
    const char* message;
    Status statusCode;
    uint32_t threadId;
    const char* function;
    uint16_t line;
};


class ChronosPowerManager {
public:
    ChronosPowerManager();
    ~ChronosPowerManager();

    
    ChronosPowerManager& withProfile(PowerProfile profile);
    ChronosPowerManager& withFrequency(uint32_t frequencyHz);
    ChronosPowerManager& withPeripheral(Peripheral p, bool enable = true);
    ChronosPowerManager& withDomain(PowerDomain domain, bool enable = true);
    ChronosPowerManager& withEnergyBudget(float dailyBudgetWh);
    ChronosPowerManager& withThermalLimit(float maxTempCelsius);
    ChronosPowerManager& withAdaptiveScaling(bool enable = true);
    ChronosPowerManager& withQuantumOptimization(bool enable = true);
    ChronosPowerManager& withNeuralOptimization(bool enable = true);
    ChronosPowerManager& withLogging(LogLevel level);
    ChronosPowerManager& withVoltageScaling(bool enable = true);
    ChronosPowerManager& withClockGating(bool enable = true);
    ChronosPowerManager& withPowerGating(bool enable = true);
    ChronosPowerManager& withRetentionMode(bool enable = true);

    
    Status begin(PowerProfile initialProfile = PowerProfile::ADAPTIVE_BATTERY_LIFE);
    Status apply(); 
    
    
    Status setProfile(PowerProfile profile);
    PowerProfile getProfile() const;
    Status setCustomPolicy(const PowerPolicy& policy);
    Status getCustomPolicy(PowerPolicy& policy) const;
    
    
    Status sleep(const WakeupPolicy& policy);
    Status sleep(uint32_t durationMs);
    Status sleepUntil(WakeupReason reason);
    Status quantumSleep(const WakeupPolicy& policy); 
    Status neuralSleep(const WakeupPolicy& policy);  
    [[noreturn]] void hibernate(const WakeupPolicy& policy);
    [[noreturn]] void shutdown();
    
    
    Status setCpuFrequency(uint32_t frequencyHz);
    uint32_t getCpuFrequency() const;
    Status setDynamicFrequencyScaling(bool enable);
    Status setFrequencyRange(uint32_t minHz, uint32_t maxHz);
    Status setQuantumFrequencyScaling(bool enable);
    Status setNeuralFrequencyScaling(bool enable);
    
    
    Status control(Peripheral p, bool enable);
    Status massControl(const std::vector<Peripheral>& peripherals, bool enable);
    Status massControl(const uint64_t masks[], uint8_t maskCount, bool enable);
    Status setClockGatingForDomain(PowerDomain domain, bool enable);
    Status enablePeripheralGroup(const std::vector<Peripheral>& group);
    Status disablePeripheralGroup(const std::vector<Peripheral>& group);
    Status setPeripheralPowerIsland(Peripheral p, bool isolate);
    
    
    Status enableAdaptiveScaling(const AdaptiveScalingConfig& config);
    Status disableAdaptiveScaling();
    Status setEnergyBudget(const EnergyBudget& budget);
    Status enableQuantumOptimization(const QuantumPowerConfig& config);
    Status enableNeuralOptimization(const NeuralPowerConfig& config);
    Status optimizeForBatteryLife();
    Status optimizeForPerformance();
    Status optimizeForThermal();
    Status optimizeForEnergyHarvesting();
    Status optimizeForCarbonFootprint();
    Status optimizeMultiObjective(float perfWeight, float powerWeight, float thermalWeight);
    
    
    EnergyMetrics getEnergyMetrics() const;
    ThermalMetrics getThermalMetrics() const;
    PerformanceMetrics getPerformanceMetrics() const;
    Status getInternalTemperature(float& temp);
    Status getCoreVoltage(float& voltage);
    Status getFlashState(FlashState& state);
    Status getBatteryLevel(float& percentage);
    Status getBatteryHealth(float& healthPercent);
    Status getPowerEfficiency(float& efficiency);
    Status getEnergyEfficiency(float& efficiency);
    Status getCarbonFootprint(float& grams);
    
    
    Status registerWakeupCallback(WakeupCallback callback);
    Status registerErrorCallback(ErrorCallback callback);
    Status registerEnergyCallback(EnergyCallback callback);
    Status registerThermalCallback(ThermalCallback callback);
    Status registerPerformanceCallback(PerformanceCallback callback);
    Status registerBatteryCallback(BatteryCallback callback);
    Status registerFrequencyCallback(FrequencyCallback callback);
    Status registerPolicyCallback(PolicyCallback callback);
    
    
    WakeupReason getWakeupReason();
    ResetReason getResetReason();
    void clearWakeupReason();
    uint32_t getSleepDurationMillis() const;
    uint32_t getTotalSleepTime() const;
    uint32_t getTotalUptime() const;
    uint32_t getWakeupCount() const;
    float getAverageSleepDuration() const;
    float getSleepEfficiency() const;
    
    
    void synchronizeSystemTime();
    Status calibrateInternalOscillator(SystemClockSource osc, uint32_t referenceFreq);
    Status setSystemClockSource(SystemClockSource source);
    Status calibrateEnergyMeasurement();
    Status calibrateThermalSensors();
    Status calibrateQuantumSensors();
    
    
    Status setLogLevel(LogLevel level);
    LogLevel getLogLevel() const;
    std::vector<LogEntry> getLogEntries(uint32_t maxEntries = 100) const;
    void clearLog();
    Status enablePerformanceMonitoring(bool enable);
    Status enableEnergyProfiling(bool enable);
    Status enableThermalProfiling(bool enable);
    Status generatePowerReport(const std::string& filename);
    
    
    Status createPeripheralGroup(const std::string& name, const std::vector<Peripheral>& peripherals);
    Status enablePeripheralGroup(const std::string& name);
    Status disablePeripheralGroup(const std::string& name);
    Status saveConfiguration(const std::string& name);
    Status loadConfiguration(const std::string& name);
    Status exportConfiguration(const std::string& filename);
    Status importConfiguration(const std::string& filename);
    Status cloneConfiguration(const std::string& source, const std::string& destination);
    
    
    Status setPlatformSpecificOption(const std::string& option, const std::string& value);
    std::string getPlatformSpecificOption(const std::string& option) const;
    Status enablePlatformOptimizations();
    Status disablePlatformOptimizations();
    
    
    Status trainPowerModel();
    Status loadPowerModel(const std::string& filename);
    Status savePowerModel(const std::string& filename);
    Status predictPowerConsumption(uint32_t durationMs, float& prediction);
    Status predictBatteryLife(float& hoursRemaining);
    Status predictOptimalFrequency(uint32_t& frequency);
    Status enableReinforcementLearning(bool enable);
    
    
    Status initializeQuantumProcessor();
    Status runQuantumOptimization();
    Status getQuantumState(float& coherence, float& entanglement);
    Status setQuantumParameters(float coherenceTime, float entanglementStrength);
    
    
    Status enableEnergyHarvesting(bool enable);
    Status setSolarPanelWatts(float watts);
    Status setKineticHarvesterWatts(float watts);
    Status setRfHarvesterWatts(float watts);
    Status getHarvestedEnergy(float& microWattHours);
    Status optimizeHarvestingEfficiency();
    
    
    Status enableCarbonTracking(bool enable);
    Status setCarbonIntensity(float gramsPerKwh);
    Status getCarbonFootprint(float& totalGrams);
    Status optimizeForSustainability();
    
    
    Status enableSecurityMode(bool enable);
    Status setTamperDetection(bool enable);
    Status enableSafetyShutdown(bool enable);
    Status setSafetyLimits(float maxTemp, float minVoltage, float maxCurrent);
    Status performSecurityAudit();
    
    
    static const char* statusToString(Status status);
    static const char* profileToString(PowerProfile profile);
    static const char* peripheralToString(Peripheral peripheral);
    static const char* wakeupReasonToString(WakeupReason reason);
    static std::vector<Peripheral> getPeripheralsInDomain(PowerDomain domain);
    static bool isPeripheralAvailable(Peripheral p);
    static uint32_t estimateEnergyConsumption(PowerProfile profile, uint32_t durationMs);
    static float calculateOptimalFrequency(float workload, float thermalLimit, float powerBudget);
    static PowerProfile recommendProfile(float batteryLevel, float temperature, float workload);
    static bool validatePowerPolicy(const PowerPolicy& policy);
    static Status optimizePowerPolicy(PowerPolicy& policy, float targetEfficiency);

private:
    
    PowerProfile m_currentProfile;
    PowerPolicy m_customPolicy;
    WakeupReason m_lastWakeupReason;
    ResetReason m_lastResetReason;
    unsigned long m_preSleepMillis;
    unsigned long m_postWakeupMillis;
    unsigned long m_totalSleepTime;
    unsigned long m_totalUptime;
    bool m_isInitialized;
    LogLevel m_logLevel;
    
    
    WakeupCallback m_wakeupCallback;
    ErrorCallback m_errorCallback;
    EnergyCallback m_energyCallback;
    ThermalCallback m_thermalCallback;
    PerformanceCallback m_performanceCallback;
    BatteryCallback m_batteryCallback;
    FrequencyCallback m_frequencyCallback;
    PolicyCallback m_policyCallback;
    
    
    EnergyMetrics m_energyMetrics;
    ThermalMetrics m_thermalMetrics;
    PerformanceMetrics m_performanceMetrics;
    
    
    AdaptiveScalingConfig m_adaptiveConfig;
    QuantumPowerConfig m_quantumConfig;
    NeuralPowerConfig m_neuralConfig;
    bool m_adaptiveScalingEnabled;
    bool m_quantumOptimizationEnabled;
    bool m_neuralOptimizationEnabled;
    uint32_t m_lastAdaptiveUpdate;
    
    
    EnergyBudget m_energyBudget;
    float m_energyConsumedToday;
    float m_energyHarvestedToday;
    uint32_t m_lastEnergyUpdate;
    bool m_energyHarvestingEnabled;
    
    
    std::map<std::string, std::vector<Peripheral>> m_peripheralGroups;
    std::map<std::string, PowerPolicy> m_savedConfigurations;
    std::vector<LogEntry> m_logEntries;
    static const size_t MAX_LOG_ENTRIES = 10000;
    
    
    PowerPolicy m_pendingPolicy;
    bool m_hasPendingChanges;
    
    
    std::vector<float> m_powerHistory;
    std::vector<float> m_workloadHistory;
    std::vector<float> m_thermalHistory;
    bool m_modelTrained;
    
    
    float m_quantumCoherence;
    float m_quantumEntanglement;
    bool m_quantumInitialized;
    
    
    bool m_securityEnabled;
    bool m_tamperDetected;
    uint32_t m_securityViolations;
    
    
    bool m_carbonTrackingEnabled;
    float m_carbonIntensity;
    float m_totalCarbonFootprint;
    
    
    Status applyProfile(PowerProfile profile);
    void applyPolicy(const PowerPolicy& policy);
    void updateMetrics();
    void updateAdaptiveScaling();
    void updateQuantumOptimization();
    void updateNeuralOptimization();
    void updateEnergyBudget();
    void updateEnergyHarvesting();
    void updateCarbonTracking();
    void logMessage(LogLevel level, const char* message, Status status = Status::OK);
    void triggerCallbacks();
    
    
    void trainNeuralNetwork();
    float predictWorkload(uint32_t futureMs);
    float predictTemperature(uint32_t futureMs);
    float predictEnergyConsumption(uint32_t futureMs);
    void updatePowerModel();
    
    
    void initializeQuantumState();
    void updateQuantumState();
    float calculateQuantumOptimization();
    void applyQuantumCorrection();
    
    
    void checkTamperDetection();
    void performSecurityCheck();
    void handleSecurityViolation();
    
    
    Status platform_setCpuFrequency(uint32_t frequencyHz);
    uint32_t platform_getCpuFrequency() const;
    Status platform_control(Peripheral p, bool enable);
    void platform_sleep(const WakeupPolicy& policy);
    void platform_quantumSleep(const WakeupPolicy& policy);
    void platform_neuralSleep(const WakeupPolicy& policy);
    [[noreturn]] void platform_hibernate(const WakeupPolicy& policy);
    WakeupReason platform_getWakeupReason();
    ResetReason platform_getResetReason();
    Status platform_getInternalTemperature(float& temp);
    Status platform_getCoreVoltage(float& voltage);
    Status platform_getFlashState(FlashState& state);
    Status platform_getBatteryLevel(float& percentage);
    Status platform_getBatteryHealth(float& health);
    void platform_synchronizeSystemTime(uint32_t sleepMillis);
    Status platform_setSystemClockSource(SystemClockSource source);
    Status platform_enableDynamicVoltageScaling(bool enable);
    Status platform_enableQuantumOptimization(bool enable);
    Status platform_enableNeuralOptimization(bool enable);
    Status platform_setPlatformOption(const std::string& option, const std::string& value);
    std::string platform_getPlatformOption(const std::string& option) const;
    
    
    ChronosPowerManager(const ChronosPowerManager&) = delete;
    ChronosPowerManager& operator=(const ChronosPowerManager&) = delete;
};


extern std::unique_ptr<ChronosPowerManager> g_powerManager;


#define CHRONOS_SLEEP_MS(ms) ChronosPower::CPower().sleep(ms)
#define CHRONOS_QUANTUM_SLEEP_MS(ms) ChronosPower::CPower().quantumSleep(WakeupPolicy{.wdt = {.timeoutMillis = ms}})
#define CHRONOS_NEURAL_SLEEP_MS(ms) ChronosPower::CPower().neuralSleep(WakeupPolicy{.wdt = {.timeoutMillis = ms}})
#define CHRONOS_SLEEP_UNTIL(reason) ChronosPower::CPower().sleepUntil(reason)
#define CHRONOS_SET_PROFILE(profile) ChronosPower::CPower().setProfile(profile)
#define CHRONOS_ENABLE_PERIPHERAL(p) ChronosPower::CPower().control(p, true)
#define CHRONOS_DISABLE_PERIPHERAL(p) ChronosPower::CPower().control(p, false)
#define CHRONOS_OPTIMIZE_BATTERY() ChronosPower::CPower().optimizeForBatteryLife()
#define CHRONOS_OPTIMIZE_PERFORMANCE() ChronosPower::CPower().optimizeForPerformance()
#define CHRONOS_OPTIMIZE_THERMAL() ChronosPower::CPower().optimizeForThermal()
#define CHRONOS_ENABLE_QUANTUM() ChronosPower::CPower().enableQuantumOptimization(QuantumPowerConfig{.enabled = true})
#define CHRONOS_ENABLE_NEURAL() ChronosPower::CPower().enableNeuralOptimization(NeuralPowerConfig{.enabled = true})


inline ChronosPowerManager& CPower() {
    if (!g_powerManager) {
        g_powerManager = std::make_unique<ChronosPowerManager>();
    }
    return *g_powerManager;
}


inline Status sleepFor(uint32_t milliseconds) {
    return CPower().sleep(milliseconds);
}

inline Status quantumSleepFor(uint32_t milliseconds) {
    WakeupPolicy policy;
    policy.wdt.timeoutMillis = milliseconds;
    return CPower().quantumSleep(policy);
}

inline Status neuralSleepFor(uint32_t milliseconds) {
    WakeupPolicy policy;
    policy.wdt.timeoutMillis = milliseconds;
    return CPower().neuralSleep(policy);
}

inline Status hibernateFor(uint32_t seconds) {
    WakeupPolicy policy;
    policy.rtc.timeoutSeconds = seconds;
    CPower().hibernate(policy);
    return Status::OK; 
}

inline Status enableUltraLowPowerMode() {
    return CPower().setProfile(PowerProfile::ULTRA_LOW_POWER);
}

inline Status enableQuantumEfficiencyMode() {
    return CPower().setProfile(PowerProfile::QUANTUM_EFFICIENCY);
}

inline Status enableNeuralPowerMode() {
    return CPower().setProfile(PowerProfile::NEURAL_POWER_MANAGEMENT);
}

inline Status enableHighPerformanceMode() {
    return CPower().setProfile(PowerProfile::MAX_PERFORMANCE);
}

inline Status enableAdaptiveMode() {
    return CPower().setProfile(PowerProfile::ADAPTIVE_PERFORMANCE);
}

inline Status enableEnergyHarvestingMode(float solarWatts = 0.0f, float kineticWatts = 0.0f, float rfWatts = 0.0f) {
    CPower().setSolarPanelWatts(solarWatts);
    CPower().setKineticHarvesterWatts(kineticWatts);
    CPower().setRfHarvesterWatts(rfWatts);
    return CPower().enableEnergyHarvesting(true);
}

inline Status enableBatteryPreservationMode() {
    return CPower().setProfile(PowerProfile::BATTERY_PRESERVATION);
}

inline Status enableEmergencyPowerMode() {
    return CPower().setProfile(PowerProfile::EMERGENCY_POWER);
}


inline float calculateQuantumEfficiency(float coherenceTime, float entanglementStrength) {
    return (coherenceTime * entanglementStrength) / (1.0f + coherenceTime + entanglementStrength);
}

inline float calculateNeuralOptimization(float learningRate, uint32_t epochs) {
    return 1.0f - exp(-learningRate * epochs);
}

inline float calculateEnergyEfficiency(float energyConsumed, float workDone) {
    return (workDone > 0.0f) ? (workDone / energyConsumed) * 100.0f : 0.0f;
}

inline float calculateThermalEfficiency(float inputPower, float heatDissipated) {
    return (inputPower > 0.0f) ? ((inputPower - heatDissipated) / inputPower) * 100.0f : 0.0f;
}

inline float calculateCarbonFootprint(float energyKwh, float carbonIntensity) {
    return energyKwh * carbonIntensity; 
}

} 

#endif 
