#ifndef CHRONOS_POWER_H
#define CHRONOS_POWER_H
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#if defined(__AVR__)
#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#elif defined(ESP32)
#include <esp_sleep.h>
#include <driver/rtc_io.h>
#include <soc/rtc_cntl_reg.h>
#include <soc/soc.h>
#include <esp_bt.h>
#elif defined(ESP8266)
extern "C" {
#include "user_interface.h"
}
#elif defined(ARDUINO_ARCH_STM32)
#include <libmaple/pwr.h>
#include <libmaple/rcc.h>
#include <RTClock.h>
#elif defined(ARDUINO_ARCH_SAMD)
#include <RTCZero.h>
#include <samd.h>
#elif defined(ARDUINO_ARCH_RP2040)
#include <hardware/pll.h>
#include <hardware/clocks.h>
#include <hardware/structs/scb.h>
#include <hardware/watchdog.h>
#include <hardware/rtc.h>
#include <hardware/adc.h>
#elif defined(TEENSYDUINO)
#include <kinetis.h>
#elif defined(__IMXRT1062__)
#include "imxrt.h"
#endif

namespace ChronosPower {

enum class Status : int16_t {
    OK = 0,
    FAILURE_GENERIC = -1,
    FAILURE_UNSUPPORTED_ARCH = -2,
    FAILURE_UNSUPPORTED_FEATURE = -3,
    FAILURE_INVALID_PARAMETER = -4,
    FAILURE_CONFIG_LOCKED = -5,
    FAILURE_PERIPHERAL_ACTIVE = -6,
    FAILURE_RTC_UNINITIALIZED = -7,
    FAILURE_INVALID_FREQUENCY = -8,
    FAILURE_WAKEUP_CONFLICT = -9,
    FAILURE_PLL_LOCK_FAILED = -10,
    FAILURE_OSCILLATOR_FAILED = -11,
    FAILURE_CLOCK_GATING_FAILED = -12,
    FAILURE_VOLTAGE_SCALE = -13,
    FAILURE_FLASH_POWERDOWN = -14,
    FAILURE_BOD_CONFIG = -15,
    FAILURE_CALLBACK_NOT_SET = -16,
    FAILURE_TIMER_UNAVAILABLE = -17,
    FAILURE_INTERRUPT_MAP = -18,
    FAILURE_NO_WAKEUP_SOURCE = -19,
    FAILURE_DMA_CHANNEL_ACTIVE = -20,
    FAILURE_PERIPHERAL_NOT_FOUND = -21,
    FAILURE_PRECISION_UNATTAINABLE = -22,
    FAILURE_REGISTER_WRITE_PROTECTED = -23,
    FAILURE_SYSTEM_STATE_INVALID = -24
};

enum class SystemClockSource : uint8_t {
    INTERNAL_RC_SLOW,
    INTERNAL_RC_FAST,
    EXTERNAL_CRYSTAL,
    EXTERNAL_BYPASS,
    PLL_INTERNAL,
    PLL_EXTERNAL
};

enum class PowerDomain : uint8_t {
    CORE,
    MEMORY,
    ANALOG,
    COMMUNICATIONS,
    TIMERS,
    GPIO,
    HIGH_SPEED_PERIPHERALS,
    LOW_POWER_PERIPHERALS
};

enum class PowerProfile : uint8_t {
    MAX_PERFORMANCE,
    HIGH_PERFORMANCE,
    BALANCED_DESKTOP_REPLACEMENT,
    BALANCED_MOBILE,
    LOW_POWER_ACTIVE,
    ULTRA_LOW_POWER_IDLE,
    DEEP_SLEEP_PREP,
    HIBERNATION_PREP,
    SHUTDOWN_PREP,
    CUSTOM_POLICY
};

enum class WakeupReason : uint8_t {
    UNKNOWN,
    POWER_ON_RESET,
    EXTERNAL_PIN_0,
    EXTERNAL_PIN_1,
    EXTERNAL_PIN_2,
    EXTERNAL_PIN_3,
    EXTERNAL_PIN_4,
    EXTERNAL_PIN_5,
    EXTERNAL_PIN_6,
    EXTERNAL_PIN_7,
    EXTERNAL_PIN_MULTIPLE,
    RTC_ALARM_SECONDS,
    RTC_ALARM_DATE,
    WATCHDOG_TIMEOUT_SYSTEM_RESET,
    WATCHDOG_TIMEOUT_INTERRUPT,
    TOUCH_SENSOR_GLOBAL,
    TOUCH_SENSOR_PAD_0,
    TOUCH_SENSOR_PAD_1,
    TOUCH_SENSOR_PAD_2,
    UART_RX_ACTIVITY_0,
    UART_RX_ACTIVITY_1,
    UART_RX_ACTIVITY_2,
    I2C_SLAVE_ADDR_MATCH_0,
    I2C_SLAVE_ADDR_MATCH_1,
    SPI_SLAVE_CS_LOW,
    TIMER_COMPARE_MATCH_A,
    TIMER_COMPARE_MATCH_B,
    ULP_COPROCESSOR,
    BROWN_OUT_RESET_LEVEL1,
    BROWN_OUT_RESET_LEVEL2,
    SOFTWARE_RESET_USER,
    SOFTWARE_RESET_EXCEPTION,
    USB_ATTACH,
    USB_ACTIVITY,
    WIFI_ASSOCIATION,
    WIFI_DEASSOCIATION,
    WIFI_PATTERN_MATCH,
    BLUETOOTH_LE_ADV,
    BLUETOOTH_LE_CONNECTION,
    BLUETOOTH_HCI_COMMAND,
    NFC_FIELD_DETECTED,
    NFC_READ_EVENT,
    ANALOG_COMPARATOR_0_RISE,
    ANALOG_COMPARATOR_0_FALL,
    ANALOG_COMPARATOR_1_RISE,
    ANALOG_COMPARATOR_1_FALL,
    THERMAL_SHUTDOWN,
    THERMAL_WARNING_HIGH,
    VOLTAGE_GLITCH
};

enum class Peripheral : uint16_t {
    SYSTEM_CONTROLLER,
    CPU_CORE_0,
    CPU_CORE_1,
    CPU_CORE_2,
    CPU_CORE_3,
    FPU_SINGLE_PRECISION,
    FPU_DOUBLE_PRECISION,
    DMA_CONTROLLER_0,
    DMA_CONTROLLER_1,
    DMA_CHANNEL_0,
    DMA_CHANNEL_1,
    DMA_CHANNEL_2,
    DMA_CHANNEL_3,
    DMA_CHANNEL_4,
    DMA_CHANNEL_5,
    DMA_CHANNEL_6,
    DMA_CHANNEL_7,
    CRC_UNIT_32BIT,
    CRC_UNIT_16BIT,
    FLASH_CONTROLLER,
    FLASH_CACHE,
    EEPROM_CONTROLLER,
    SRAM_BANK_0,
    SRAM_BANK_1,
    SRAM_BANK_2,
    SRAM_BANK_3,
    SRAM_BANK_4,
    SRAM_BANK_5,
    SRAM_BANK_6,
    SRAM_BANK_7,
    RTC_INTERNAL,
    RTC_EXTERNAL_PCF8523,
    RTC_EXTERNAL_DS3231,
    WATCHDOG_SYSTEM,
    WATCHDOG_WINDOWED,
    BROWN_OUT_DETECTOR,
    INTERNAL_TEMP_SENSOR,
    INTERNAL_VOLTAGE_REF,
    PLL_MAIN_SYSTEM,
    PLL_USB,
    PLL_AUDIO,
    PLL_ETHERNET,
    OSC_INTERNAL_RC_8MHZ,
    OSC_INTERNAL_RC_48MHZ,
    OSC_INTERNAL_RC_32KHZ,
    OSC_EXTERNAL_CRYSTAL_HS,
    OSC_EXTERNAL_CRYSTAL_LS,
    OSC_EXTERNAL_BYPASS,
    ADC_0_12BIT,
    ADC_1_12BIT,
    ADC_2_16BIT,
    ADC_INTERNAL_TEMP,
    ADC_INTERNAL_VREF,
    DAC_0_8BIT,
    DAC_1_12BIT,
    ANALOG_COMPARATOR_0,
    ANALOG_COMPARATOR_1,
    ANALOG_COMPARATOR_2,
    TIMER_0_8BIT_BASIC,
    TIMER_1_16BIT_GENERAL,
    TIMER_2_8BIT_ASYNC,
    TIMER_3_16BIT_GENERAL,
    TIMER_4_16BIT_GENERAL,
    TIMER_5_16BIT_GENERAL,
    TIMER_6_16BIT_ADVANCED,
    TIMER_7_16BIT_ADVANCED,
    TIMER_8_LOW_POWER,
    I2C_0,
    I2C_1,
    I2C_2,
    I2C_3_FAST_MODE_PLUS,
    SPI_0,
    SPI_1,
    SPI_2,
    SPI_3_QUAD,
    UART_0,
    UART_1,
    UART_2,
    UART_3,
    UART_4,
    UART_LP_0,
    USB_DEVICE,
    USB_HOST,
    USB_OTG_FS,
    USB_OTG_HS,
    SDIO_0,
    SDIO_1,
    ETHERNET_MAC,
    ETHERNET_PHY_INTERNAL,
    ETHERNET_PHY_EXTERNAL,
    I2S_0_STD,
    I2S_1_STD,
    I2S_2_TDM,
    PWM_MODULE_0,
    PWM_MODULE_1,
    PWM_MODULE_2_MOTOR_CONTROL,
    QUADRATURE_ENCODER_0,
    QUADRATURE_ENCODER_1,
    TOUCH_SENSOR_UNIT,
    TOUCH_PAD_0,
    TOUCH_PAD_1,
    TOUCH_PAD_2,
    TOUCH_PAD_3,
    TOUCH_PAD_4,
    TOUCH_PAD_5,
    TOUCH_PAD_6,
    TOUCH_PAD_7,
    ULP_COPROCESSOR,
    GPIO_PORT_A,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
    GPIO_PORT_F,
    GPIO_PORT_G,
    GPIO_PORT_H,
    GPIO_PORT_I,
    GPIO_PORT_J,
    GPIO_PORT_K,
    AES_ACCELERATOR_128,
    AES_ACCELERATOR_256,
    SHA_ACCELERATOR_256,
    SHA_ACCELERATOR_512,
    TRNG_UNIT,
    PKA_UNIT,
    CAN_BUS_0,
    CAN_BUS_1,
    CAN_BUS_FD,
    LIN_BUS_0,
    WIFI_MAC_2_4GHZ,
    WIFI_PHY_2_4GHZ,
    BLUETOOTH_LE_PHY,
    BLUETOOTH_CLASSIC_PHY,
    BLUETOOTH_CONTROLLER,
    RADIO_802_15_4,
    NFC_CONTROLLER_TYPE_A,
    NFC_CONTROLLER_TYPE_F,
    LCD_CONTROLLER_PARALLEL,
    LCD_CONTROLLER_SPI,
    CAMERA_INTERFACE_DCMI,
    PSEUDO_PERIPHERAL_SERIAL_PRINT,
    PSEUDO_PERIPHERAL_ARDUINO_MILLIS,
    PERIPHERAL_COUNT
};

struct ExternalInterruptConfig {
    uint64_t pinMask;
    int triggerMode;
    uint32_t debounceMicros;
};

struct RtcConfig {
    uint32_t timeoutSeconds;
    uint8_t alarmHour;
    uint8_t alarmMinute;
    uint8_t alarmSecond;
    bool matchDate;
    uint8_t alarmDay;
    uint8_t alarmMonth;
};

struct WatchdogConfig {
    uint32_t timeoutMillis;
    bool generateInterrupt;
    bool windowedMode;
    uint8_t windowPercent;
};

struct ThermalConfig {
    float warningTempCelsius;
    float shutdownTempCelsius;
};

struct VoltageConfig {
    float warningVoltage;
    float shutdownVoltage;
};

struct WakeupPolicy {
    ExternalInterruptConfig extInterrupt;
    RtcConfig rtc;
    WatchdogConfig wdt;
    uint32_t touchThreshold;
    uint8_t uartPortIndex;
    bool enableUlpWakeup;
    bool enableWifiWakeup;
    bool enableBluetoothWakeup;
    bool enableAnalogComparatorWakeup;
};

struct PowerPolicy {
    uint32_t cpuFrequencyHz;
    bool flashDeepPowerDown;
    int8_t voltageScaleLevel;
    uint64_t peripheralMask[4];
};

typedef void (*ChronosCallback)(WakeupReason reason);

class ChronosPowerManager {
public:
    ChronosPowerManager();

    Status begin(PowerProfile initialProfile = PowerProfile::BALANCED_MOBILE);
    Status setProfile(PowerProfile profile);
    PowerProfile getProfile() const;

    Status setCustomPolicy(const PowerPolicy& policy);
    Status getCustomPolicy(PowerPolicy& policy) const;
    
    Status setCpuFrequency(uint32_t frequencyHz);
    uint32_t getCpuFrequency() const;

    Status sleep(const WakeupPolicy& policy);
    [[noreturn]] void hibernate(const WakeupPolicy& policy);
    [[noreturn]] void shutdown();

    Status control(Peripheral p, bool enable);
    Status massControl(const uint64_t masks[], uint8_t maskCount, bool enable);

    Status registerEventCallback(ChronosCallback callback);
    
    WakeupReason getWakeupReason();
    void clearWakeupReason();

    uint32_t getSleepDurationMillis() const;
    void synchronizeSystemTime();
    
    Status getInternalTemperature(float& temp);
    Status getCoreVoltage(float& voltage);
    Status getPeripheralClock(Peripheral p, uint32_t& frequencyHz);

    Status calibrateInternalOscillator(SystemClockSource osc, uint32_t referenceFreq);

private:
    Status applyProfile(PowerProfile profile);
    void applyPolicy(const PowerPolicy& policy);

    Status platform_setCpuFrequency(uint32_t frequencyHz);
    uint32_t platform_getCpuFrequency() const;
    Status platform_control(Peripheral p, bool enable);
    void platform_sleep(const WakeupPolicy& policy);
    void platform_hibernate(const WakeupPolicy& policy);
    WakeupReason platform_getWakeupReason();
    Status platform_getInternalTemperature(float& temp);
    Status platform_getCoreVoltage(float& voltage);
    void platform_synchronizeSystemTime(uint32_t sleepMillis);

    PowerProfile m_currentProfile;
    PowerPolicy m_customPolicy;
    WakeupReason m_lastWakeupReason;
    unsigned long m_preSleepMillis;
    unsigned long m_postWakeupMillis;
    ChronosCallback m_eventCallback;
    bool m_isInitialized;
    
    ChronosPowerManager(const ChronosPowerManager&) = delete;
    ChronosPowerManager& operator=(const ChronosPowerManager&) = delete;
};

}

extern ChronosPower::ChronosPowerManager CPower;
#endif
