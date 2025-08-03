/*
 * ChronosPower - Пример IoT датчика с низким энергопотреблением
 * 
 * Демонстрирует создание энергоэффективного IoT устройства:
 * - Периодическое считывание датчиков
 * - Глубокий сон между измерениями
 * - Оптимизация энергопотребления
 * - Передача данных по WiFi
 */

#include <ChronosPower.h>

using namespace ChronosPower;

// Конфигурация
const uint32_t MEASUREMENT_INTERVAL_MS = 60000; // 1 минута
const uint32_t DEEP_SLEEP_DURATION_MS = 55000;  // 55 секунд сна
const int SENSOR_PIN = A0;
const int LED_PIN = 2;

// Переменные
uint32_t measurementCounter = 0;
float batteryVoltage = 3.7;

void setup() {
    Serial.begin(115200);
    
    // Настройка пинов
    pinMode(SENSOR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    Serial.println("IoT Датчик с низким энергопотреблением");
    Serial.println("====================================");
    
    // Инициализация в режиме ультранизкого потребления
    CPower().begin(PowerProfile::ULTRA_LOW_POWER);
    
    // Отключение ненужной периферии
    optimizePeripherals();
    
    Serial.println("Инициализация завершена");
    showSystemInfo();
}

void loop() {
    measurementCounter++;
    
    Serial.println("\n--- Цикл измерения #" + String(measurementCounter) + " ---");
    
    // Включение необходимой периферии
    enableMeasurementPeripherals();
    
    // Выполнение измерений
    performMeasurements();
    
    // Передача данных (если необходимо)
    if (measurementCounter % 10 == 0) { // Каждые 10 измерений
        transmitData();
    }
    
    // Отключение периферии перед сном
    disableMeasurementPeripherals();
    
    // Показ статистики энергопотребления
    showEnergyStats();
    
    // Переход в глубокий сон
    enterDeepSleep();
}

void optimizePeripherals() {
    Serial.println("Оптимизация периферии...");
    
    // Отключаем все ненужные модули
    CPower().disablePeripheral(Peripheral::SPI_0);
    CPower().disablePeripheral(Peripheral::SPI_1);
    CPower().disablePeripheral(Peripheral::I2C_1);
    CPower().disablePeripheral(Peripheral::UART_1);
    CPower().disablePeripheral(Peripheral::UART_2);
    CPower().disablePeripheral(Peripheral::TIMER_1);
    CPower().disablePeripheral(Peripheral::TIMER_2);
    CPower().disablePeripheral(Peripheral::DAC_0);
    CPower().disablePeripheral(Peripheral::DAC_1);
    
    // Оставляем только необходимые
    CPower().enablePeripheral(Peripheral::ADC_0);
    CPower().enablePeripheral(Peripheral::RTC);
    CPower().enablePeripheral(Peripheral::UART_0); // Для Serial
    
    Serial.println("Периферия оптимизирована");
}

void enableMeasurementPeripherals() {
    // Включаем периферию только на время измерений
    CPower().enablePeripheral(Peripheral::ADC_0);
    CPower().enablePeripheral(Peripheral::I2C_0);
    
    // Небольшая задержка для стабилизации
    delay(10);
}

void disableMeasurementPeripherals() {
    // Отключаем периферию после измерений
    CPower().disablePeripheral(Peripheral::I2C_0);
    // ADC оставляем включенным для мониторинга батареи
}

void performMeasurements() {
    Serial.println("Выполнение измерений...");
    
    // Индикация активности
    digitalWrite(LED_PIN, HIGH);
    
    // Измерение с аналогового датчика
    int sensorValue = analogRead(SENSOR_PIN);
    float voltage = sensorValue * (3.3 / 1023.0);
    
    Serial.println("Значение датчика: " + String(sensorValue));
    Serial.println("Напряжение: " + String(voltage, 3) + " В");
    
    // Измерение напряжения батареи (симуляция)
    batteryVoltage -= 0.001; // Симуляция разряда
    if (batteryVoltage < 3.0) batteryVoltage = 3.7; // Сброс для демо
    
    Serial.println("Напряжение батареи: " + String(batteryVoltage, 3) + " В");
    
    // Выключение индикации
    digitalWrite(LED_PIN, LOW);
    
    Serial.println("Измерения завершены");
}

void transmitData() {
    Serial.println("Передача данных...");
    
    // Включение WiFi модуля
    #ifdef CHRONOS_PLATFORM_ESP32
    CPower().enablePeripheral(Peripheral::WIFI);
    delay(100); // Время на инициализацию WiFi
    #endif
    
    // Симуляция передачи данных
    Serial.println("Подключение к WiFi...");
    delay(2000); // Симуляция времени подключения
    
    Serial.println("Отправка данных на сервер...");
    delay(1000); // Симуляция передачи
    
    Serial.println("Данные переданы успешно");
    
    // Отключение WiFi для экономии энергии
    #ifdef CHRONOS_PLATFORM_ESP32
    CPower().disablePeripheral(Peripheral::WIFI);
    #endif
}

void showEnergyStats() {
    Serial.println("--- Статистика энергопотребления ---");
    
    EnergyMetrics energy = CPower().getEnergyMetrics();
    ThermalMetrics thermal = CPower().getThermalMetrics();
    
    Serial.println("Потребление: " + String(energy.currentConsumptionMicroAmps) + " мкА");
    Serial.println("Батарея: " + String(energy.batteryPercentage) + "%");
    Serial.println("Время работы: " + String(energy.uptimeSeconds) + " сек");
    Serial.println("Время сна: " + String(energy.sleepTimeSeconds) + " сек");
    Serial.println("Температура: " + String(thermal.currentTemperatureCelsius) + "°C");
    
    // Расчет эффективности сна
    uint32_t totalTime = energy.uptimeSeconds + energy.sleepTimeSeconds;
    if (totalTime > 0) {
        float sleepEfficiency = (float)energy.sleepTimeSeconds / totalTime * 100.0;
        Serial.println("Эффективность сна: " + String(sleepEfficiency, 1) + "%");
    }
    
    // Оценка времени работы
    if (energy.currentConsumptionMicroAmps > 0) {
        float batteryCapacityMah = 2000.0; // 2000 мАч батарея
        float avgCurrentMah = energy.currentConsumptionMicroAmps / 1000.0;
        float estimatedDays = (batteryCapacityMah * energy.batteryPercentage / 100.0) / (avgCurrentMah * 24.0);
        
        Serial.println("Оценка работы: " + String(estimatedDays, 1) + " дней");
    }
    
    // Предупреждения
    if (energy.batteryPercentage < 20) {
        Serial.println("ВНИМАНИЕ: Низкий заряд батареи!");
    }
    
    if (energy.currentConsumptionMicroAmps > 10000) {
        Serial.println("ВНИМАНИЕ: Высокое потребление энергии!");
    }
}

void enterDeepSleep() {
    Serial.println("Переход в глубокий сон на " + String(DEEP_SLEEP_DURATION_MS / 1000) + " сек...");
    Serial.flush();
    
    // Настройка политики пробуждения
    WakeupPolicy policy;
    policy.timerWakeupMs = DEEP_SLEEP_DURATION_MS;
    
    // Дополнительные источники пробуждения для экстренных случаев
    policy.extWakeupPinMask = (1ULL << 0); // Кнопка на пине 0
    
    #ifdef CHRONOS_PLATFORM_ESP32
    // Для ESP32 используем глубокий сон
    CPower().deepSleep(policy);
    #else
    // Для других платформ используем обычный сон
    CPower().sleep(policy);
    #endif
    
    // Эта строка выполнится только после пробуждения
    Serial.println("Пробуждение! Причина: " + wakeupReasonToString(CPower().getWakeupReason()));
}

void showSystemInfo() {
    Serial.println("--- Информация о системе ---");
    Serial.println("Версия библиотеки: " + String(CPower().getVersion()));
    Serial.println("Частота CPU: " + String(CPower().getCpuFrequency()) + " Гц");
    Serial.println("Профиль: Ультранизкое потребление");
    Serial.println("Интервал измерений: " + String(MEASUREMENT_INTERVAL_MS / 1000) + " сек");
    Serial.println("Время сна: " + String(DEEP_SLEEP_DURATION_MS / 1000) + " сек");
    
    #ifdef CHRONOS_PLATFORM_ESP32
    Serial.println("Платформа: ESP32");
    #elif defined(CHRONOS_PLATFORM_AVR)
    Serial.println("Платформа: Arduino AVR");
    #elif defined(CHRONOS_PLATFORM_RP2040)
    Serial.println("Платформа: Raspberry Pi Pico");
    #else
    Serial.println("Платформа: Неизвестная");
    #endif
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
