/*
 * ChronosPower - Продвинутые возможности
 * 
 * Этот пример демонстрирует:
 * - Пользовательские настройки частоты
 * - Работу с группами периферийных устройств
 * - Сложные политики пробуждения
 * - Мониторинг и логирование
 */

#include <ChronosPower.h>

using namespace ChronosPower;

// Переменные для демонстрации
uint32_t measurementCount = 0;
uint32_t totalEnergyConsumption = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("ChronosPower - Продвинутые возможности");
    Serial.println("=====================================");
    
    // Инициализация с пользовательскими настройками
    CPower().begin(PowerProfile::CUSTOM);
    
    // Установка пользовательской частоты
    Serial.println("Установка частоты 100 МГц...");
    Status result = CPower().setCpuFrequency(100000000);
    if (result == Status::OK) {
        Serial.println("Частота установлена успешно");
    } else {
        Serial.println("Ошибка установки частоты");
    }
    
    // Настройка периферии для работы с датчиками
    setupSensorPeripherals();
    
    Serial.println("Инициализация завершена\n");
}

void loop() {
    // Демонстрация различных возможностей
    demonstrateFrequencyScaling();
    delay(2000);
    
    demonstratePeripheralGroups();
    delay(2000);
    
    demonstrateAdvancedSleep();
    delay(2000);
    
    demonstrateEnergyMonitoring();
    delay(2000);
    
    demonstrateThermalManagement();
    delay(5000);
}

void setupSensorPeripherals() {
    Serial.println("--- Настройка периферии для датчиков ---");
    
    // Включаем необходимые модули
    CPower().enablePeripheral(Peripheral::ADC_0);
    CPower().enablePeripheral(Peripheral::I2C_0);
    CPower().enablePeripheral(Peripheral::TIMER_0);
    
    Serial.println("Включены: ADC, I2C, Timer");
    
    // Выключаем ненужные модули для экономии энергии
    CPower().disablePeripheral(Peripheral::SPI_0);
    CPower().disablePeripheral(Peripheral::UART_1);
    
    Serial.println("Выключены: SPI, UART1");
}

void demonstrateFrequencyScaling() {
    Serial.println("--- Демонстрация масштабирования частоты ---");
    
    uint32_t frequencies[] = {240000000, 160000000, 80000000, 40000000, 20000000};
    const char* freqNames[] = {"240 МГц", "160 МГц", "80 МГц", "40 МГц", "20 МГц"};
    
    for (int i = 0; i < 5; i++) {
        Serial.println("Переключение на " + String(freqNames[i]));
        
        uint32_t startTime = millis();
        Status result = CPower().setCpuFrequency(frequencies[i]);
        uint32_t switchTime = millis() - startTime;
        
        if (result == Status::OK) {
            // Измерение производительности
            uint32_t perfStart = micros();
            performCalculations();
            uint32_t perfTime = micros() - perfStart;
            
            EnergyMetrics energy = CPower().getEnergyMetrics();
            
            Serial.println("  Время переключения: " + String(switchTime) + " мс");
            Serial.println("  Время вычислений: " + String(perfTime) + " мкс");
            Serial.println("  Потребление: " + String(energy.currentConsumptionMicroAmps) + " мкА");
        } else {
            Serial.println("  Ошибка переключения частоты");
        }
        
        delay(1000);
    }
    
    // Возврат к балансированному режиму
    CPower().setProfile(PowerProfile::BALANCED);
    Serial.println("Возврат к балансированному режиму");
}

void performCalculations() {
    // Простые вычисления для тестирования производительности
    volatile float result = 0;
    for (int i = 0; i < 1000; i++) {
        result += sin(i * 0.1) * cos(i * 0.1);
    }
}

void demonstratePeripheralGroups() {
    Serial.println("--- Управление группами периферии ---");
    
    // Создание группы для работы с коммуникацией
    Serial.println("Группа коммуникации:");
    CPower().enablePeripheral(Peripheral::UART_0);
    CPower().enablePeripheral(Peripheral::I2C_0);
    CPower().enablePeripheral(Peripheral::SPI_0);
    
    Serial.println("  UART: " + String(CPower().isPeripheralEnabled(Peripheral::UART_0) ? "Вкл" : "Выкл"));
    Serial.println("  I2C: " + String(CPower().isPeripheralEnabled(Peripheral::I2C_0) ? "Вкл" : "Выкл"));
    Serial.println("  SPI: " + String(CPower().isPeripheralEnabled(Peripheral::SPI_0) ? "Вкл" : "Выкл"));
    
    delay(2000);
    
    // Выключение всей группы
    Serial.println("Выключение группы коммуникации:");
    CPower().disablePeripheral(Peripheral::UART_0);
    CPower().disablePeripheral(Peripheral::I2C_0);
    CPower().disablePeripheral(Peripheral::SPI_0);
    
    Serial.println("  UART: " + String(CPower().isPeripheralEnabled(Peripheral::UART_0) ? "Вкл" : "Выкл"));
    Serial.println("  I2C: " + String(CPower().isPeripheralEnabled(Peripheral::I2C_0) ? "Вкл" : "Выкл"));
    Serial.println("  SPI: " + String(CPower().isPeripheralEnabled(Peripheral::SPI_0) ? "Вкл" : "Выкл"));
}

void demonstrateAdvancedSleep() {
    Serial.println("--- Продвинутые режимы сна ---");
    
    // Сложная политика пробуждения
    WakeupPolicy policy;
    policy.timerWakeupMs = 10000; // Максимум 10 секунд
    policy.extWakeupPinMask = (1ULL << 2) | (1ULL << 3); // Пины 2 и 3
    policy.touchThreshold = 40; // Чувствительность касания
    
    #ifdef CHRONOS_PLATFORM_ESP32
    policy.enableWifiWakeup = true;
    policy.enableBluetoothWakeup = false;
    #endif
    
    Serial.println("Настройка сложной политики пробуждения:");
    Serial.println("  Таймер: 10 секунд");
    Serial.println("  Внешние пины: 2, 3");
    Serial.println("  Касание: порог 40");
    #ifdef CHRONOS_PLATFORM_ESP32
    Serial.println("  WiFi: включено");
    #endif
    
    Serial.println("Засыпаем...");
    Serial.flush();
    
    uint32_t sleepStart = millis();
    CPower().sleep(policy);
    uint32_t sleepDuration = millis() - sleepStart;
    
    Serial.println("Проснулись!");
    Serial.println("Время сна: " + String(sleepDuration) + " мс");
    Serial.println("Причина: " + wakeupReasonToString(CPower().getWakeupReason()));
}

void demonstrateEnergyMonitoring() {
    Serial.println("--- Мониторинг энергопотребления ---");
    
    measurementCount++;
    
    EnergyMetrics energy = CPower().getEnergyMetrics();
    totalEnergyConsumption += energy.currentConsumptionMicroAmps;
    
    Serial.println("Измерение #" + String(measurementCount));
    Serial.println("Текущее потребление: " + String(energy.currentConsumptionMicroAmps) + " мкА");
    Serial.println("Среднее потребление: " + String(totalEnergyConsumption / measurementCount) + " мкА");
    Serial.println("Уровень батареи: " + String(energy.batteryPercentage) + "%");
    Serial.println("Время работы: " + String(energy.uptimeSeconds) + " сек");
    
    // Расчет времени работы от батареи
    if (energy.currentConsumptionMicroAmps > 0) {
        // Предполагаем батарею 2000 мАч
        float batteryCapacityMah = 2000.0;
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

void demonstrateThermalManagement() {
    Serial.println("--- Тепловое управление ---");
    
    ThermalMetrics thermal = CPower().getThermalMetrics();
    
    Serial.println("Температура: " + String(thermal.currentTemperatureCelsius) + "°C");
    Serial.println("Тепловая зона: " + String(thermal.thermalZone));
    Serial.println("Троттлинг: " + String(thermal.thermalThrottlingActive ? "Активен" : "Неактивен"));
    
    // Симуляция нагрузки для повышения температуры
    Serial.println("Создание вычислительной нагрузки...");
    
    uint32_t startTime = millis();
    while (millis() - startTime < 3000) {
        performCalculations();
        
        // Проверка температуры каждые 500 мс
        if ((millis() - startTime) % 500 == 0) {
            thermal = CPower().getThermalMetrics();
            Serial.println("  Температура: " + String(thermal.currentTemperatureCelsius) + "°C");
            
            // Простая защита от перегрева
            if (thermal.currentTemperatureCelsius > 70) {
                Serial.println("  ВНИМАНИЕ: Высокая температура, снижение частоты");
                CPower().setProfile(PowerProfile::LOW_POWER);
                break;
            }
        }
    }
    
    Serial.println("Нагрузочный тест завершен");
    
    // Возврат к нормальному режиму
    CPower().setProfile(PowerProfile::BALANCED);
}

String wakeupReasonToString(WakeupReason reason) {
    switch (reason) {
        case WakeupReason::TIMER: return "Таймер";
        case WakeupReason::EXTERNAL_PIN: return "Внешний пин";
        case WakeupReason::TOUCHPAD: return "Сенсор касания";
        case WakeupReason::ULP: return "ULP процессор";
        case WakeupReason::WIFI: return "WiFi";
        case WakeupReason::BLUETOOTH: return "Bluetooth";
        case WakeupReason::UART: return "UART";
        default: return "Неизвестная";
    }
}
