#ifndef CHRONOS_POWER_LITE_H
#define CHRONOS_POWER_LITE_H

/*
 * ChronosPower Lite v2.5 - Ultra-Lightweight Power Management
 * 
 * This is the minimal version for severely memory-constrained devices.
 * Provides core power management functionality in under 2KB.
 */

#include "Arduino.h"

// Minimal feature set
#define CHRONOS_LITE_VERSION

namespace ChronosPowerLite {

// Minimal enums using smallest possible types
enum Status : int8_t { OK = 0, FAIL = -1 };
enum Profile : uint8_t { MAX_PERF = 0, BALANCED = 1, LOW_POWER = 2, ULTRA_LOW = 3 };
enum Peripheral : uint8_t { 
    UART0 = 0, SPI0 = 1, I2C0 = 2, ADC0 = 3, TIMER0 = 4, TIMER1 = 5, 
    WIFI = 6, BT = 7, RTC = 8, WDT = 9 
};

// Minimal structures
struct __attribute__((packed)) Energy {
    uint16_t currentMicroAmps;
    uint8_t batteryPercent;
    uint32_t uptimeSeconds;
};

struct __attribute__((packed)) WakeupConfig {
    uint32_t timeoutMs;
    uint8_t pin;
    bool enablePin : 1;
    bool enableTimer : 1;
    uint8_t reserved : 6;
};

// Ultra-compact power manager
class PowerManager {
public:
    PowerManager() : profile_(BALANCED), initialized_(false) {}
    
    Status begin(Profile p = BALANCED) {
        if (initialized_) return OK;
        initialized_ = true;
        return setProfile(p);
    }
    
    Status setProfile(Profile p) {
        profile_ = p;
        return applyProfile();
    }
    
    Status sleep(uint32_t ms) {
        WakeupConfig cfg = {ms, 0, false, true, 0};
        return sleep(cfg);
    }
    
    Status sleep(const WakeupConfig& cfg) {
        if (!initialized_) return FAIL;
        
        #if defined(__AVR__)
        return avrSleep(cfg);
        #elif defined(ESP32)
        return esp32Sleep(cfg);
        #else
        delay(cfg.timeoutMs);
        return OK;
        #endif
    }
    
    Status control(Peripheral p, bool enable) {
        #if defined(__AVR__)
        return avrControl(p, enable);
        #elif defined(ESP32)
        return esp32Control(p, enable);
        #else
        return OK;
        #endif
    }
    
    Energy getEnergy() const { return energy_; }
    Profile getProfile() const { return profile_; }
    
private:
    Profile profile_;
    bool initialized_;
    Energy energy_;
    
    Status applyProfile() {
        switch (profile_) {
            case ULTRA_LOW:
                control(UART0, false);
                control(SPI0, false);
                control(I2C0, false);
                setCpuFreq(1000000); // 1MHz
                break;
            case LOW_POWER:
                control(UART0, true);
                control(SPI0, false);
                setCpuFreq(8000000); // 8MHz
                break;
            case BALANCED:
                control(UART0, true);
                control(SPI0, true);
                control(I2C0, true);
                setCpuFreq(16000000); // 16MHz
                break;
            case MAX_PERF:
                setCpuFreq(240000000); // Max freq
                break;
        }
        return OK;
    }
    
    void setCpuFreq(uint32_t hz) {
        #if defined(__AVR__)
        // AVR frequency scaling
        uint8_t prescaler = 0;
        uint32_t target = F_CPU / hz;
        while (target > 1 && prescaler < 8) {
            target >>= 1;
            prescaler++;
        }
        CLKPR = (1 << CLKPCE);
        CLKPR = prescaler;
        #elif defined(ESP32)
        setCpuFrequencyMhz(hz / 1000000);
        #endif
    }
    
    #if defined(__AVR__)
    Status avrSleep(const WakeupConfig& cfg) {
        if (cfg.enableTimer && cfg.timeoutMs > 0) {
            // Setup watchdog timer
            uint8_t wdtBits = 6; // ~1 second
            if (cfg.timeoutMs <= 16) wdtBits = 0;
            else if (cfg.timeoutMs <= 32) wdtBits = 1;
            else if (cfg.timeoutMs <= 64) wdtBits = 2;
            else if (cfg.timeoutMs <= 125) wdtBits = 3;
            else if (cfg.timeoutMs <= 250) wdtBits = 4;
            else if (cfg.timeoutMs <= 500) wdtBits = 5;
            else if (cfg.timeoutMs <= 1000) wdtBits = 6;
            else if (cfg.timeoutMs <= 2000) wdtBits = 7;
            else if (cfg.timeoutMs <= 4000) wdtBits = 8;
            else wdtBits = 9;
            
            WDTCSR |= (1 << WDCE) | (1 << WDE);
            WDTCSR = (1 << WDIE) | (wdtBits & 8 ? 1<<WDP3 : 0) | (wdtBits & 7);
        }
        
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_cpu();
        sleep_disable();
        
        if (cfg.enableTimer) {
            WDTCSR = 0; // Disable watchdog
        }
        
        return OK;
    }
    
    Status avrControl(Peripheral p, bool enable) {
        switch (p) {
            case UART0:
                if (enable) PRR &= ~(1 << PRUSART0);
                else PRR |= (1 << PRUSART0);
                break;
            case SPI0:
                if (enable) PRR &= ~(1 << PRSPI);
                else PRR |= (1 << PRSPI);
                break;
            case I2C0:
                if (enable) PRR &= ~(1 << PRTWI);
                else PRR |= (1 << PRTWI);
                break;
            case ADC0:
                if (enable) PRR &= ~(1 << PRADC);
                else PRR |= (1 << PRADC);
                break;
            case TIMER0:
                if (enable) PRR &= ~(1 << PRTIM0);
                else PRR |= (1 << PRTIM0);
                break;
            case TIMER1:
                if (enable) PRR &= ~(1 << PRTIM1);
                else PRR |= (1 << PRTIM1);
                break;
            default:
                return FAIL;
        }
        return OK;
    }
    #endif
    
    #if defined(ESP32)
    Status esp32Sleep(const WakeupConfig& cfg) {
        if (cfg.enableTimer && cfg.timeoutMs > 0) {
            esp_sleep_enable_timer_wakeup(cfg.timeoutMs * 1000ULL);
        }
        if (cfg.enablePin) {
            esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(cfg.pin), 0);
        }
        esp_light_sleep_start();
        return OK;
    }
    
    Status esp32Control(Peripheral p, bool enable) {
        switch (p) {
            case WIFI:
                return (enable ? esp_wifi_start() : esp_wifi_stop()) == ESP_OK ? OK : FAIL;
            case BT:
                return (enable ? esp_bluedroid_enable() : esp_bluedroid_disable()) == ESP_OK ? OK : FAIL;
            default:
                return OK; // Most peripherals controlled by drivers
        }
    }
    #endif
};

// Global instance
extern PowerManager Power;

// Convenience functions
inline Status sleepMs(uint32_t ms) { return Power.sleep(ms); }
inline Status setLowPower() { return Power.setProfile(LOW_POWER); }
inline Status setUltraLow() { return Power.setProfile(ULTRA_LOW); }
inline Status enablePeripheral(Peripheral p) { return Power.control(p, true); }
inline Status disablePeripheral(Peripheral p) { return Power.control(p, false); }

} // namespace ChronosPowerLite

// Macros for even more compact usage
#define SLEEP_MS(ms) ChronosPowerLite::sleepMs(ms)
#define SET_LOW_POWER() ChronosPowerLite::setLowPower()
#define SET_ULTRA_LOW() ChronosPowerLite::setUltraLow()
#define ENABLE_PERIPHERAL(p) ChronosPowerLite::enablePeripheral(p)
#define DISABLE_PERIPHERAL(p) ChronosPowerLite::disablePeripheral(p)

} // namespace ChronosPowerLite

#endif // CHRONOS_POWER_LITE_H
