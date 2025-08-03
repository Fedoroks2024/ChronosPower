/*
 * ChronosPower v2.5 - Базовый пример использования
 * 
 * Этот пример показывает основные возможности библиотеки:
 * - Инициализация с профилем энергопотребления
 * - Переключение между режимами
 * - Использование режимов сна
 * - Мониторинг энергопотребления
 */

#include <ChronosPower.h>

using namespace ChronosPower;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("ChronosPower - Базовый пример");
    Serial.println("Версия: " + String(CPower().getVersion()));
    
    // Инициализация с балансированным профилем
    Status result = CPower().begin(PowerProfile::BALANCED_MOBILE);
    if (result == Status::OK) {
        Serial.println("Библиотека инициализирована успешно");
    } else {
        Serial.println("Ошибка инициализации");
        return;
    }
    
    // Показать текущие настройки
    showCurrentSettings();
}

void loop() {
    Serial.println("\n=== Демонстрация режимов ===");
    
    // Тест различных профилей
    testPowerProfiles();
    
    // Тест управления периферией
    testPeripheralControl();
    
    // Тест режимов сна
    testSleepModes();
    
    // Мониторинг энергопотребления
    monitorEnergy();
    
    delay(5000);
}

void showCurrentSettings() {
    Serial.println("\n--- Текущие настройки ---");
    Serial.println("Профиль: " + String(profileToString(CPower().getProfile())));
    Serial.println("Частота CPU: " + String(CPower().getCpuFrequency()) + " Гц");
    
    EnergyMetrics energy = CPower().getEnergyMetrics();
    Serial.println("Потребление: " + String(energy.currentConsumptionMicroAmps) + " мкА");
    
    ThermalMetrics thermal = CPower().getThermalMetrics();
    Serial.println("Температура: " + String(thermal.currentTemperatureCelsius) + "°C");
}

void testPowerProfiles() {
    Serial.println("\n--- Тест профилей энергопотребления ---");
    
    PowerProfile profiles[] = {
        PowerProfile::MAX_PERFORMANCE,
        PowerProfile::BALANCED_MOBILE,
        PowerProfile::LOW_POWER_ACTIVE,
        PowerProfile::ULTRA_LOW_POWER
    };
    
    const char* profileNames[] = {
        "Максимальная производительность",
        "Сбалансированный",
        "Низкое потребление",
        "Ультранизкое потребление"
    };
    
    for (int i = 0; i < 4; i++) {
        Serial.println("Переключение на: " + String(profileNames[i]));
        
        Status result = CPower().setProfile(profiles[i]);
        if (result == Status::OK) {
            delay(500); // Время на стабилизацию
            
            EnergyMetrics energy = CPower().getEnergyMetrics();
            Serial.println("  Частота: " + String(CPower().getCpuFrequency()) + " Гц");
            Serial.println("  Потребление: " + String(energy.currentConsumptionMicroAmps) + " мкА");
        } else {
            Serial.println("  Ошибка переключения");
        }
        
        delay(1000);
    }
}

void testPeripheralControl() {
    Serial.println("\n--- Тест управления периферией ---");
    
    // Включение ADC
    Serial.println("Включение ADC...");
    CPower().control(Peripheral::ADC_0_12BIT, true);
    Serial.println("ADC включен: " + String(isPeripheralEnabled(Peripheral::ADC_0_12BIT) ? "Да" : "Нет"));
    
    delay(1000);
    
    // Выключение ADC
    Serial.println("Выключение ADC...");
    CPower().control(Peripheral::ADC_0_12BIT, false);
    Serial.println("ADC включен: " + String(isPeripheralEnabled(Peripheral::ADC_0_12BIT) ? "Да" : "Нет"));
    
    delay(1000);
    
    // Управление доменом коммуникации
    Serial.println("Выключение домена коммуникации...");
    CPower().controlDomain(PowerDomain::COMMUNICATIONS, false);
    
    delay(1000);
    
    Serial.println("Включение домена коммуникации...");
    CPower().controlDomain(PowerDomain::COMMUNICATIONS, true);
}

void testSleepModes() {
    Serial.println("\n--- Тест режимов сна ---");
    
    // Простой сон на 2 секунды
    Serial.println("Засыпаем на 2 секунды...");
    Serial.flush();
    
    uint32_t startTime = millis();
    sleepFor(2000);
    uint32_t endTime = millis();
    
    Serial.println("Проснулись! Время сна: " + String(endTime - startTime) + " мс");
    Serial.println("Причина пробуждения: " + String(wakeupReasonToString(CPower().getWakeupReason())));
    
    delay(1000);
    
    // Сон с настройкой политики пробуждения
    Serial.println("Сон с внешним пробуждением (пин 2)...");
    Serial.flush();
    
    WakeupPolicy policy;
    policy.rtcTimeoutSeconds = 5; // Максимум 5 секунд
    policy.extInterruptPinMask = (1ULL << 2); // Пин 2
    
    startTime = millis();
    CPower().sleep(policy);
    endTime = millis();
    
    Serial.println("Проснулись! Время сна: " + String(endTime - startTime) + " мс");
    Serial.println("Причина пробуждения: " + String(wakeupReasonToString(CPower().getWakeupReason())));
}

void monitorEnergy() {
    Serial.println("\n--- Мониторинг энергопотребления ---");
    
    EnergyMetrics energy = CPower().getEnergyMetrics();
    ThermalMetrics thermal = CPower().getThermalMetrics();
    
    Serial.println("Текущее потребление: " + String(energy.currentConsumptionMicroAmps) + " мкА");
    Serial.println("Среднее потребление: " + String(energy.averageConsumptionMicroAmps) + " мкА");
    Serial.println("Уровень батареи: " + String(energy.batteryPercentage) + "%");
    Serial.println("Время работы: " + String(energy.uptimeSeconds) + " сек");
    Serial.println("Время сна: " + String(energy.sleepTimeSeconds) + " сек");
    Serial.println("Температура: " + String(thermal.currentTemperatureCelsius) + "°C");
    Serial.println("Троттлинг: " + String(thermal.thermalThrottlingActive ? "Активен" : "Неактивен"));
    
    // Расчет эффективности сна
    uint32_t totalTime = energy.uptimeSeconds + energy.sleepTimeSeconds;
    if (totalTime > 0) {
        float sleepEfficiency = (float)energy.sleepTimeSeconds / totalTime * 100.0;
        Serial.println("Эффективность сна: " + String(sleepEfficiency, 1) + "%");
    }
    
    // Оценка времени работы от батареи
    if (energy.currentConsumptionMicroAmps > 0) {
        float batteryCapacityMah = 2000.0; // 2000 мАч батарея
        float currentConsumptionMah = energy.currentConsumptionMicroAmps / 1000.0;
        float estimatedHours = (batteryCapacityMah * energy.batteryPercentage / 100.0) / currentConsumptionMah;
        
        Serial.println("Оценка времени работы: " + String(estimatedHours, 1) + " часов");
    }
    
    // Рекомендации по оптимизации
    if (energy.currentConsumptionMicroAmps > 50000) {
        Serial.println("РЕКОМЕНДАЦИЯ: Высокое потребление, рассмотрите переход в режим LOW_POWER");
    } else if (energy.currentConsumptionMicroAmps < 5000) {
        Serial.println("ОТЛИЧНО: Низкое энергопотребление");
    }
}

// Вспомогательные функции
String profileToString(PowerProfile profile) {
    switch (profile) {
        case PowerProfile::MAX_PERFORMANCE: return "Максимальная производительность";
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
        case WakeupReason::EXTERNAL_PIN_2: return "Внешний пин 2";
        case WakeupReason::WATCHDOG_TIMEOUT_INTERRUPT: return "Watchdog";
        case WakeupReason::TOUCH_SENSOR_GLOBAL: return "Сенсор касания";
        case WakeupReason::THERMAL_WARNING_HIGH: return "Высокая температура";
        case WakeupReason::BATTERY_LOW: return "Низкий заряд батареи";
        default: return "Неизвестная";
    }
}

bool isPeripheralEnabled(Peripheral p) {
    // Упрощенная проверка - в реальной реализации это будет проверять регистры
    return true;
}
