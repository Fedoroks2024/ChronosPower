/*
 * ChronosPower v2.5 - Пример мониторинга энергопотребления
 * 
 * Демонстрирует:
 * - Регистрацию callback функций для мониторинга
 * - Отслеживание энергопотребления в реальном времени
 * - Автоматическую оптимизацию на основе метрик
 * - Тепловой мониторинг и защиту
 */

#include <ChronosPower.h>

using namespace ChronosPower;

// Переменные для статистики
uint32_t totalMeasurements = 0;
uint32_t highPowerEvents = 0;
uint32_t thermalEvents = 0;
float maxTemperature = 0;
uint16_t minBatteryLevel = 100;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("ChronosPower - Мониторинг энергопотребления");
    Serial.println("==========================================");
    
    // Инициализация с адаптивным профилем
    CPower().begin(PowerProfile::ADAPTIVE_BATTERY_LIFE);
    
    // Регистрация callback функций
    setupCallbacks();
    
    Serial.println("Система мониторинга инициализирована");
    Serial.println("Начинаем отслеживание энергопотребления...\n");
}

void loop() {
    // Симуляция различных нагрузок
    simulateWorkload();
    
    // Показ текущей статистики каждые 10 секунд
    static uint32_t lastStatsTime = 0;
    if (millis() - lastStatsTime > 10000) {
        showStatistics();
        lastStatsTime = millis();
    }
    
    delay(1000);
}

void setupCallbacks() {
    Serial.println("Настройка callback функций...");
    
    // Callback для мониторинга энергии
    CPower().registerEnergyCallback([](const EnergyMetrics& metrics) {
        totalMeasurements++;
        
        Serial.println("\n📊 Обновление энергии:");
        Serial.println("  Текущее потребление: " + String(metrics.currentConsumptionMicroAmps) + " мкА");
        Serial.println("  Среднее потребление: " + String(metrics.averageConsumptionMicroAmps) + " мкА");
        Serial.println("  Пиковое потребление: " + String(metrics.peakConsumptionMicroAmps) + " мкА");
        Serial.println("  Уровень батареи: " + String(metrics.batteryPercentage) + "%");
        Serial.println("  Время работы: " + String(metrics.uptimeSeconds) + " сек");
        Serial.println("  Эффективность: " + String(metrics.powerEfficiencyPercent) + "%");
        
        // Отслеживание минимального уровня батареи
        if (metrics.batteryPercentage < minBatteryLevel) {
            minBatteryLevel = metrics.batteryPercentage;
        }
        
        // Проверка высокого энергопотребления
        if (metrics.currentConsumptionMicroAmps > 50000) { // Более 50 мА
            highPowerEvents++;
            Serial.println("  ⚠️ ВЫСОКОЕ ПОТРЕБЛЕНИЕ ОБНАРУЖЕНО!");
            
            // Автоматическое переключение в режим экономии
            if (metrics.batteryPercentage < 30) {
                Serial.println("  🔋 Переключение в режим экономии батареи");
                CPower().setProfile(PowerProfile::ULTRA_LOW_POWER);
            }
        }
        
        // Проверка критически низкого заряда
        if (metrics.batteryPercentage < 10) {
            Serial.println("  🚨 КРИТИЧЕСКИ НИЗКИЙ ЗАРЯД БАТАРЕИ!");
            Serial.println("  Переход в аварийный режим");
            CPower().setProfile(PowerProfile::ULTRA_LOW_POWER);
            
            // Отключение всей ненужной периферии
            CPower().controlDomain(PowerDomain::COMMUNICATIONS, false);
            CPower().controlDomain(PowerDomain::ANALOG_SUBSYSTEM, false);
        }
        
        // Рекомендации по оптимизации
        if (metrics.powerEfficiencyPercent < 50) {
            Serial.println("  💡 Рекомендация: Низкая эффективность, рассмотрите оптимизацию");
        }
    });
    
    // Callback для теплового мониторинга
    CPower().registerThermalCallback([](const ThermalMetrics& thermal) {
        thermalEvents++;
        
        Serial.println("\n🌡️ Тепловое обновление:");
        Serial.println("  Текущая температура: " + String(thermal.currentTemperatureCelsius) + "°C");
        Serial.println("  Максимальная температура: " + String(thermal.maxTemperatureCelsius) + "°C");
        Serial.println("  Средняя температура: " + String(thermal.averageTemperatureCelsius) + "°C");
        Serial.println("  Тепловая зона: " + String(thermal.thermalZone));
        
        // Отслеживание максимальной температуры
        if (thermal.currentTemperatureCelsius > maxTemperature) {
            maxTemperature = thermal.currentTemperatureCelsius;
        }
        
        // Проверка перегрева
        if (thermal.currentTemperatureCelsius > 70) {
            Serial.println("  🔥 ВЫСОКАЯ ТЕМПЕРАТУРА ОБНАРУЖЕНА!");
            
            if (thermal.thermalThrottlingActive) {
                Serial.println("  ❄️ Тепловой троттлинг активен (уровень " + 
                              String(thermal.thermalThrottlingLevel) + ")");
            } else {
                Serial.println("  ❄️ Активация теплового троттлинга");
                CPower().optimizeForThermal();
            }
        }
        
        // Критическая температура
        if (thermal.currentTemperatureCelsius > 85) {
            Serial.println("  🚨 КРИТИЧЕСКАЯ ТЕМПЕРАТУРА!");
            Serial.println("  Аварийное снижение производительности");
            CPower().setProfile(PowerProfile::ULTRA_LOW_POWER);
        }
        
        // Нормализация температуры
        if (thermal.currentTemperatureCelsius < 60 && thermal.thermalThrottlingActive) {
            Serial.println("  ✅ Температура нормализовалась");
        }
    });
    
    // Callback для пробуждения
    CPower().registerWakeupCallback([](WakeupReason reason) {
        Serial.println("\n⏰ Пробуждение:");
        Serial.println("  Причина: " + wakeupReasonToString(reason));
        Serial.println("  Время: " + String(millis()) + " мс");
        
        // Логирование различных причин пробуждения
        switch (reason) {
            case WakeupReason::RTC_ALARM_SECONDS:
                Serial.println("  📅 Запланированное пробуждение");
                break;
            case WakeupReason::EXTERNAL_PIN_2:
                Serial.println("  👆 Пробуждение от кнопки");
                break;
            case WakeupReason::THERMAL_WARNING_HIGH:
                Serial.println("  🌡️ Пробуждение из-за высокой температуры");
                break;
            case WakeupReason::BATTERY_LOW:
                Serial.println("  🔋 Пробуждение из-за низкого заряда");
                break;
            default:
                Serial.println("  ❓ Неожиданное пробуждение");
                break;
        }
    });
    
    Serial.println("✅ Callback функции настроены");
}

void simulateWorkload() {
    static uint8_t workloadCycle = 0;
    workloadCycle++;
    
    switch (workloadCycle % 6) {
        case 0:
            // Режим простоя
            Serial.println("\n💤 Режим простоя");
            CPower().setProfile(PowerProfile::LOW_POWER_ACTIVE);
            delay(2000);
            break;
            
        case 1:
            // Легкая нагрузка
            Serial.println("\n⚡ Легкая нагрузка");
            CPower().setProfile(PowerProfile::BALANCED_MOBILE);
            for (int i = 0; i < 1000; i++) {
                volatile int dummy = i * i;
            }
            break;
            
        case 2:
            // Средняя нагрузка
            Serial.println("\n🔥 Средняя нагрузка");
            CPower().setProfile(PowerProfile::HIGH_PERFORMANCE);
            for (int i = 0; i < 10000; i++) {
                volatile float dummy = sin(i * 0.01);
            }
            break;
            
        case 3:
            // Высокая нагрузка
            Serial.println("\n🚀 Высокая нагрузка");
            CPower().setProfile(PowerProfile::MAX_PERFORMANCE);
            for (int i = 0; i < 50000; i++) {
                volatile double dummy = sqrt(i) * log(i + 1);
            }
            break;
            
        case 4:
            // Тест периферии
            Serial.println("\n🔌 Тест периферии");
            CPower().control(Peripheral::ADC_0_12BIT, true);
            CPower().control(Peripheral::SPI_0, true);
            CPower().control(Peripheral::I2C_0, true);
            
            // Симуляция работы с периферией
            for (int i = 0; i < 100; i++) {
                analogRead(A0);
                delay(10);
            }
            
            CPower().control(Peripheral::SPI_0, false);
            CPower().control(Peripheral::I2C_0, false);
            break;
            
        case 5:
            // Тест сна
            Serial.println("\n😴 Тест сна");
            Serial.println("Засыпаем на 3 секунды...");
            sleepFor(3000);
            break;
    }
}

void showStatistics() {
    Serial.println("\n" + String('=', 50));
    Serial.println("📈 СТАТИСТИКА МОНИТОРИНГА");
    Serial.println(String('=', 50));
    
    // Текущие метрики
    EnergyMetrics energy = CPower().getEnergyMetrics();
    ThermalMetrics thermal = CPower().getThermalMetrics();
    PerformanceMetrics perf = CPower().getPerformanceMetrics();
    
    Serial.println("🔋 ЭНЕРГИЯ:");
    Serial.println("  Текущее потребление: " + String(energy.currentConsumptionMicroAmps) + " мкА");
    Serial.println("  Среднее потребление: " + String(energy.averageConsumptionMicroAmps) + " мкА");
    Serial.println("  Пиковое потребление: " + String(energy.peakConsumptionMicroAmps) + " мкА");
    Serial.println("  Уровень батареи: " + String(energy.batteryPercentage) + "%");
    Serial.println("  Минимальный уровень: " + String(minBatteryLevel) + "%");
    Serial.println("  Эффективность: " + String(energy.powerEfficiencyPercent) + "%");
    
    Serial.println("\n🌡️ ТЕМПЕРАТУРА:");
    Serial.println("  Текущая: " + String(thermal.currentTemperatureCelsius) + "°C");
    Serial.println("  Максимальная за сессию: " + String(maxTemperature) + "°C");
    Serial.println("  Средняя: " + String(thermal.averageTemperatureCelsius) + "°C");
    Serial.println("  Троттлинг: " + String(thermal.thermalThrottlingActive ? "Активен" : "Неактивен"));
    
    Serial.println("\n⚡ ПРОИЗВОДИТЕЛЬНОСТЬ:");
    Serial.println("  Использование CPU: " + String(perf.cpuUtilizationPercent) + "%");
    Serial.println("  Использование памяти: " + String(perf.memoryUtilizationPercent) + "%");
    Serial.println("  Частота CPU: " + String(CPower().getCpuFrequency()) + " Гц");
    Serial.println("  Профиль: " + profileToString(CPower().getProfile()));
    
    Serial.println("\n📊 СОБЫТИЯ:");
    Serial.println("  Всего измерений: " + String(totalMeasurements));
    Serial.println("  События высокого потребления: " + String(highPowerEvents));
    Serial.println("  Тепловые события: " + String(thermalEvents));
    Serial.println("  Количество пробуждений: " + String(energy.wakeupCount));
    
    // Расчет времени работы от батареи
    if (energy.currentConsumptionMicroAmps > 0) {
        float batteryCapacityMah = 2000.0; // Предполагаем батарею 2000 мАч
        float currentMa = energy.currentConsumptionMicroAmps / 1000.0;
        float estimatedHours = (batteryCapacityMah * energy.batteryPercentage / 100.0) / currentMa;
        
        Serial.println("\n⏱️ ПРОГНОЗ:");
        Serial.println("  Оставшееся время работы: " + String(estimatedHours, 1) + " часов");
        
        if (estimatedHours < 1.0) {
            Serial.println("  ⚠️ ВНИМАНИЕ: Менее часа работы!");
        } else if (estimatedHours > 24.0) {
            Serial.println("  ✅ Отличная автономность!");
        }
    }
    
    Serial.println(String('=', 50));
}

String profileToString(PowerProfile profile) {
    switch (profile) {
        case PowerProfile::MAX_PERFORMANCE: return "Максимальная производительность";
        case PowerProfile::HIGH_PERFORMANCE: return "Высокая производительность";
        case PowerProfile::BALANCED_MOBILE: return "Сбалансированный";
        case PowerProfile::LOW_POWER_ACTIVE: return "Низкое потребление";
        case PowerProfile::ULTRA_LOW_POWER: return "Ультранизкое потребление";
        case PowerProfile::ADAPTIVE_BATTERY_LIFE: return "Адаптивная батарея";
        default: return "Неизвестный";
    }
}

String wakeupReasonToString(WakeupReason reason) {
    switch (reason) {
        case WakeupReason::RTC_ALARM_SECONDS: return "Таймер RTC";
        case WakeupReason::EXTERNAL_PIN_2: return "Внешний пин";
        case WakeupReason::TOUCH_SENSOR_GLOBAL: return "Сенсор касания";
        case WakeupReason::WATCHDOG_TIMEOUT_INTERRUPT: return "Watchdog";
        case WakeupReason::THERMAL_WARNING_HIGH: return "Высокая температура";
        case WakeupReason::BATTERY_LOW: return "Низкий заряд";
        default: return "Неизвестная";
    }
}
