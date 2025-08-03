<p align="center">
  <img src="https://raw.githubusercontent.com/Fedoroks2024/ChronosPower/da14c2717107c76b75b65c717742a41bf1bbf89f/ChronosPower.svg" alt="Логотип ChronosPower" width="300"/>
</p>

__ChronosPower__ — это многофункциональный, кросс-платформенный фреймворк для управления энергопотреблением встраиваемых систем нового поколения. Он предоставляет единый, высокоуровневый API с возможностями искусственного интеллекта для контроля над низкоуровневыми аспектами питания на широком спектре архитектур — от классических 8-битных микроконтроллеров до современных многоядерных ARM-процессоров.

### Оглавление

*   [Ключевые Концепции](#ключевые-концепции)
*   [Что нового в версии 2.5](#что-нового-в-версии-25)
*   [Установка](#установка)
*   [Справочник API](#справочник-api)
    *   [Глава 1: Основные Команды](#глава-1-основные-команды)
    *   [Глава 2: Управление Системой](#глава-2-управление-системой)
    *   [Глава 3: Диагностика и События](#глава-3-диагностика-и-события)
    *   [Глава 4: Структуры Конфигурации](#глава-4-структуры-конфигурации)
    *   [Глава 5: Интеллектуальные Возможности](#глава-5-интеллектуальные-возможности)
*   [Большая Книга Примеров](#большая-книга-примеров)
    *   [Раздел 1: Основы Сна (Примеры 1-20)](#раздел-1-основы-сна-примеры-1-20)
    *   [Раздел 2: Управление Частотой и Профилями (Примеры 21-40)](#раздел-2-управление-частотой-и-профилями-примеры-21-40)
    *   [Раздел 3: Ручное Управление Периферией (Примеры 41-70)](#раздел-3-ручное-управление-периферией-примеры-41-70)
    *   [Раздел 4: Диагностика и Информация (Примеры 71-100)](#раздел-4-диагностика-и-информация-примеры-71-100)
    *   [Раздел 5: Продвинутые Сценарии Пробуждения (Примеры 101-150)](#раздел-5-продвинутые-сценарии-пробуждения-примеры-101-150)
    *   [Раздел 6: Домены и Массовое Управление (Примеры 151-180)](#раздел-6-домены-и-массовое-управление-примеры-151-180)
    *   [Раздел 7: Диагностика и Конечные Автоматы (Примеры 181-230)](#раздел-7-диагностика-и-конечные-автома  ы-примеры-181-230)
    *   [Раздел 8: Специализированные Политики (Примеры 231-260)](#раздел-8-специализированные-политики-примеры-231-260)
    *   [Раздел 9: Платформо-зависимые Техники (Примеры 261-301)](#раздел-9-платформо-зависимые-техники-примеры-261-301)
    *   [Раздел 10: Интеллектуальные Возможности v2.5 (Примеры 302-400)](#раздел-10-интеллектуальные-возможности-v25-примеры-302-400)
*   [Часто Задаваемые Вопросы (FAQ)](#часто-задаваемые-вопросы-faq)

### Ключевые Концепции

1.  **Профили Питания (`PowerProfile`)**: Предопределенные наборы настроек для быстрого переключения между режимами. Включая новые интеллектуальные адаптивные профили: `ADAPTIVE_PERFORMANCE`, `ADAPTIVE_BATTERY_LIFE`, `ADAPTIVE_THERMAL`.

2.  **Политики Питания (`PowerPolicy`)**: Детальная конфигурация, позволяющая точно указать частоту CPU, активные периферийные устройства, уровни напряжения, режимы работы Flash-памяти и энергетические бюджеты.

3.  **Политика Пробуждения (`WakeupPolicy`)**: Единая структура для настройки всех возможных источников пробуждения с интеллектуальными возможностями и предсказанием оптимальной длительности сна.

4.  **Абстракция Периферии (`Peripheral`)**: Глобальный перечислитель для 500+ периферийных блоков, позволяющий управлять его тактованием и питанием через единый API.

5.  **Интеллектуальное Управление**: Алгоритмы машинного обучения для адаптивного масштабирования, предсказания нагрузки и автоматической оптимизации энергопотребления.

6.  **Кросс-платформенная Совместимость**: Ядро библиотеки использует директивы препроцессора для автоматического выбора корректной низкоуровневой реализации для целевой архитектуры.

### Что нового в версии 2.5

Версия 2.5 представляет собой революционный скачок в области интеллектуального управления питанием, внедряя возможности искусственного интеллекта и машинного обучения в встраиваемые системы.

*   **Интеллектуальное адаптивное масштабирование**: Алгоритмы на основе ИИ, которые изучают поведение вашего приложения и автоматически регулируют частоту CPU и состояния периферии на основе паттернов нагрузки.

*   **Энергетическое бюджетирование**: Установка дневных/часовых энергетических бюджетов с предиктивным масштабированием. Система автоматически снижает энергопотребление при приближении к лимитам бюджета.

*   **Продвинутое тепловое управление**: Многозонный тепловой мониторинг с интеллектуальным троттлингом. Автоматически снижает производительность при превышении температурных порогов.

*   **Fluent интерфейс**: Цепочка вызовов конфигурации для элегантного, читаемого кода, который делает сложные настройки управления питанием простыми.

*   **Всеобъемлющий мониторинг**: Метрики энергии, температуры и производительности в реальном времени с поддержкой callback для реактивного управления питанием.

*   **Предсказание нагрузки**: Предсказание нагрузки и оптимизация на основе машинного обучения, которое изучает паттерны использования вашего приложения со временем.

*   **Контроль 500+ периферийных устройств**: Детальный контроль над отдельными периферийными устройствами с поддержкой создания пользовательских групп периферии (расширение с 200+ в v2.0).

*   **Управление конфигурациями**: Сохранение, загрузка и переключение между различными конфигурациями питания для разных режимов работы.

*   **Платформенный интеллект**: Автоматическое обнаружение платформы и оптимизация с платформо-специфичными возможностями и оптимизациями.

*   **Поддержка сбора энергии**: Оптимизация энергопотребления для систем сбора солнечной, кинетической и RF энергии с предиктивной доступностью энергии.

*   **Интеграция безопасности**: Энергосберегающие функции безопасности с обнаружением вмешательства и автоматическими режимами реагирования на безопасность.

### Установка

1.  Скачайте ZIP-архив с библиотекой.
2.  В Arduino IDE выберите `Скетч` -> `Подключить библиотеку` -> `Добавить .ZIP библиотеку...`.
3.  Выберите скачанный ZIP-файл.
4.  Подключите библиотеку в вашем скетче: `#include <ChronosPower.h>`.

### Справочник API

Этот раздел содержит детальное описание всех публичных функций, структур и перечислений, доступных в `ChronosPower v2.5`.

#### **Глава 1: Основные Команды**

*   `Status begin(PowerProfile initialProfile = PowerProfile::ADAPTIVE_BATTERY_LIFE)`: Инициализация менеджера питания с интеллектуальным профилем по умолчанию.
*   `Status setProfile(PowerProfile profile)`: Установка предопределенного профиля питания.
*   `Status sleep(const WakeupPolicy& policy)`: Отправка системы в режим сна с интеллектуальной оптимизацией.
*   `[[noreturn]] void hibernate(const WakeupPolicy& policy)`: Отправка системы в режим гибернации.
*   `[[noreturn]] void shutdown()`: Полное отключение питания.

#### **Глава 2: Управление Системой**

*   `Status setCpuFrequency(uint32_t frequencyHz)`: Установка тактовой частоты процессора.
*   `uint32_t getCpuFrequency() const`: Получение текущей тактовой частоты.
*   `Status control(Peripheral p, bool enable)`: Включение/отключение одного периферийного блока.
*   `Status massControl(const uint64_t masks[], uint8_t maskCount, bool enable)`: Массовое управление периферией.
*   `Status setCustomPolicy(const PowerPolicy& policy)`: Применение пользовательской политики.
*   `Status createPeripheralGroup(const std::string& name, const std::vector<Peripheral>& peripherals)`: Создание группы периферийных устройств.
*   `Status enablePeripheralGroup(const std::string& name)`: Включение группы периферии.
*   `Status disablePeripheralGroup(const std::string& name)`: Отключение группы периферии.

#### **Глава 3: Диагностика и События**

*   `WakeupReason getWakeupReason()`: Получение причины последнего пробуждения.
*   `ResetReason getResetReason()`: Получение причины последней перезагрузки.
*   `uint32_t getSleepDurationMillis() const`: Получение длительности последнего сна.
*   `Status registerEventCallback(ChronosCallback callback)`: Регистрация callback на события.
*   `Status registerEnergyCallback(EnergyCallback callback)`: Регистрация callback на изменения энергии.
*   `Status registerThermalCallback(ThermalCallback callback)`: Регистрация callback на тепловые события.
*   `Status getInternalTemperature(float& temp)`: Измерение температуры кристалла.
*   `Status getCoreVoltage(float& voltage)`: Измерение напряжения ядра.

#### **Глава 4: Структуры Конфигурации**

*   `PowerPolicy`: Комплексная политика с поддержкой энергетических бюджетов и тепловых политик.
*   `WakeupPolicy`: Расширенная политика пробуждения с интеллектуальными возможностями.
*   `EnergyBudget`: Структура для настройки энергетических бюджетов.
*   `ThermalPolicy`: Структура для настройки тепловых политик.
*   `AdaptiveScalingConfig`: Конфигурация адаптивного масштабирования.

#### **Глава 5: Интеллектуальные Возможности**

*   `Status enableAdaptiveScaling(const AdaptiveScalingConfig& config)`: Включение адаптивного масштабирования.
*   `Status setEnergyBudget(const EnergyBudget& budget)`: Установка энергетического бюджета.
*   `EnergyMetrics getEnergyMetrics() const`: Получение метрик энергопотребления.
*   `ThermalMetrics getThermalMetrics() const`: Получение тепловых метрик.
*   `PerformanceMetrics getPerformanceMetrics() const`: Получение метрик производительности.
*   `Status optimizeForBatteryLife()`: Автоматическая оптимизация для максимального времени работы батареи.
*   `Status optimizeForPerformance()`: Автоматическая оптимизация для максимальной производительности.
*   `Status optimizeForThermal()`: Автоматическая оптимизация для теплового управления.
*   `Status saveConfiguration(const std::string& name)`: Сохранение текущей конфигурации.
*   `Status loadConfiguration(const std::string& name)`: Загрузка сохраненной конфигурации.

*   **Fluent интерфейс**:
    *   `ChronosPowerManager& withProfile(PowerProfile profile)`: Установка профиля в цепочке.
    *   `ChronosPowerManager& withFrequency(uint32_t frequencyHz)`: Установка частоты в цепочке.
    *   `ChronosPowerManager& withPeripheral(Peripheral p, bool enable = true)`: Управление периферией в цепочке.
    *   `ChronosPowerManager& withEnergyBudget(float dailyBudgetWh)`: Установка энергетического бюджета в цепочке.
    *   `ChronosPowerManager& withThermalLimit(float maxTempCelsius)`: Установка теплового лимита в цепочке.
    *   `ChronosPowerManager& withAdaptiveScaling(bool enable = true)`: Включение адаптивного масштабирования в цепочке.
    *   `Status apply()`: Применение всех настроенных параметров.

### Большая Книга Примеров

#### **Раздел 1: Основы Сна (Примеры 1-20)**

**1. Интеллектуальный сон с адаптивной оптимизацией**
\`\`\`cpp
ChronosPower::WakeupPolicy p1;
p1.wdt.timeoutMillis = 2000;
p1.enableIntelligentWakeup = true;
CPower().sleep(p1);
\`\`\`

**2. Сон до нажатия кнопки на пине 2**
\`\`\`cpp
pinMode(2, INPUT_PULLUP);
ChronosPower::WakeupPolicy p2;
p2.extInterrupt.pinMask = (1ULL << 2);
p2.extInterrupt.triggerMode = FALLING;
CPower().sleep(p2);
\`\`\`

**3. Сон до отпускания кнопки на пине 3**
\`\`\`cpp
pinMode(3, INPUT_PULLUP);
ChronosPower::WakeupPolicy p3;
p3.extInterrupt.pinMask = (1ULL << 3);
p3.extInterrupt.triggerMode = RISING;
CPower().sleep(p3);
\`\`\`

**4. Сон до появления высокого уровня на пине 4**
\`\`\`cpp
pinMode(4, INPUT);
ChronosPower::WakeupPolicy p4;
p4.extInterrupt.pinMask = (1ULL << 4);
p4.extInterrupt.triggerMode = RISING;
CPower().sleep(p4);
\`\`\`

**5. Сон до появления низкого уровня на пине 5**
\`\`\`cpp
pinMode(5, INPUT_PULLUP);
ChronosPower::WakeupPolicy p5;
p5.extInterrupt.pinMask = (1ULL << 5);
p5.extInterrupt.triggerMode = LOW;
CPower().sleep(p5);
\`\`\`

**6. Сон на 1 минуту с помощью RTC**
\`\`\`cpp
ChronosPower::WakeupPolicy p6;
p6.rtc.timeoutSeconds = 60;
CPower().sleep(p6);
\`\`\`

**7. Гибернация на 2 часа**
\`\`\`cpp
ChronosPower::WakeupPolicy p7;
p7.rtc.timeoutSeconds = 2 * 3600;
CPower().hibernate(p7);
\`\`\`

**8. Гибернация до нажатия кнопки на пине 12**
\`\`\`cpp
pinMode(12, INPUT_PULLUP);
ChronosPower::WakeupPolicy p8;
p8.extInterrupt.pinMask = (1ULL << 12);
p8.extInterrupt.triggerMode = FALLING;
CPower().hibernate(p8);
\`\`\`

**9. Сон до касания (ESP32)**
\`\`\`cpp
ChronosPower::WakeupPolicy p9;
p9.touchThreshold = 40;
CPower().sleep(p9);
\`\`\`

**10. Комбинированный сон: 10 секунд ИЛИ кнопка на пине 2**
\`\`\`cpp
ChronosPower::WakeupPolicy p10;
p10.wdt.timeoutMillis = 10000;
p10.wdt.generateInterrupt = true;
pinMode(2, INPUT_PULLUP);
p10.extInterrupt.pinMask = (1ULL << 2);
p10.extInterrupt.triggerMode = FALLING;
CPower().sleep(p10);
\`\`\`

**11. Сон на 20 секунд на Arduino Uno (цикл WDT)**
\`\`\`cpp
ChronosPower::WakeupPolicy p11_8s;
p11_8s.wdt.timeoutMillis = 8000;
p11_8s.wdt.generateInterrupt = true;
ChronosPower::WakeupPolicy p11_4s;
p11_4s.wdt.timeoutMillis = 4000;
p11_4s.wdt.generateInterrupt = true;
CPower().sleep(p11_8s); 
CPower().sleep(p11_8s); 
CPower().sleep(p11_4s);
\`\`\`

**12. Сон до получения любого символа в Serial**
\`\`\`cpp
ChronosPower::WakeupPolicy p12;
p12.uartWakeup.portIndex = 1;
CPower().sleep(p12);
\`\`\`

**13. Пробуждение по любой из трех кнопок**
\`\`\`cpp
pinMode(2, INPUT_PULLUP); 
pinMode(3, INPUT_PULLUP); 
pinMode(4, INPUT_PULLUP);
ChronosPower::WakeupPolicy p13;
p13.extInterrupt.pinMask = (1ULL << 2) | (1ULL << 3) | (1ULL << 4);
p13.extInterrupt.triggerMode = FALLING;
CPower().sleep(p13);
\`\`\`

**14. Сон с предварительным отключением АЦП и SPI**
\`\`\`cpp
CPower().control(Peripheral::ADC_0_12BIT, false);
CPower().control(Peripheral::SPI_0, false);
ChronosPower::WakeupPolicy p14;
p14.wdt.timeoutMillis = 5000;
CPower().sleep(p14);
\`\`\`

**15. Гибернация до тех пор, пока на пине 13 не появится высокий уровень**
\`\`\`cpp
pinMode(13, INPUT);
ChronosPower::WakeupPolicy p15;
p15.extInterrupt.pinMask = (1ULL << 13);
p15.extInterrupt.triggerMode = HIGH;
CPower().hibernate(p15);
\`\`\`

**16. Полностью выключить устройство**
\`\`\`cpp
CPower().shutdown();
\`\`\`

**17. Сон с очень низкой чувствительностью к касанию**
\`\`\`cpp
ChronosPower::WakeupPolicy p17;
p17.touchThreshold = 80;
CPower().sleep(p17);
\`\`\`

**18. Сон на 1 час на Arduino (очень длинный цикл WDT)**
\`\`\`cpp
ChronosPower::WakeupPolicy p18;
p18.wdt.timeoutMillis = 8000;
for (int i = 0; i < 450; ++i) { 
    CPower().sleep(p18); 
}
\`\`\`

**19. Комбинированный сон: RTC на 5 минут ИЛИ касание**
\`\`\`cpp
ChronosPower::WakeupPolicy p19;
p19.rtc.timeoutSeconds = 5 * 60;
p19.touchThreshold = 40;
CPower().sleep(p19);
\`\`\`

**20. Пробуждение по изменению логического уровня (CHANGE)**
\`\`\`cpp
pinMode(7, INPUT_PULLUP);
ChronosPower::WakeupPolicy p20;
p20.extInterrupt.pinMask = (1ULL << 7);
p20.extInterrupt.triggerMode = CHANGE;
CPower().sleep(p20);
\`\`\`

#### **Раздел 2: Управление Частотой и Профилями (Примеры 21-40)**

**21. Использование адаптивного профиля производительности**
\`\`\`cpp
CPower().setProfile(PowerProfile::ADAPTIVE_PERFORMANCE);
\`\`\`

**22. Использование адаптивного профиля батареи**
\`\`\`cpp
CPower().setProfile(PowerProfile::ADAPTIVE_BATTERY_LIFE);
\`\`\`

**23. Использование адаптивного теплового профиля**
\`\`\`cpp
CPower().setProfile(PowerProfile::ADAPTIVE_THERMAL);
\`\`\`

**24. Fluent интерфейс для конфигурации**
\`\`\`cpp
CPower()
    .withProfile(PowerProfile::BALANCED_MOBILE)
    .withFrequency(80000000)
    .withEnergyBudget(0.5) // 0.5 Вт·ч в день
    .withAdaptiveScaling(true)
    .apply();
\`\`\`

**25. Замедлить AVR процессор до 8 МГц**
\`\`\`cpp
CPower().setCpuFrequency(8000000);
\`\`\`

**26. Замедлить AVR процессор до 4 МГц**
\`\`\`cpp
CPower().setCpuFrequency(4000000);
\`\`\`

**27. Замедлить AVR процессор до 1 МГц**
\`\`\`cpp
CPower().setCpuFrequency(1000000);
\`\`\`

**28. Установить частоту ESP32 на 240 МГц**
\`\`\`cpp
CPower().setCpuFrequency(240000000);
\`\`\`

**29. Установить частоту ESP32 на 80 МГц**
\`\`\`cpp
CPower().setCpuFrequency(80000000);
\`\`\`

**30. Установить частоту ESP32 на 40 МГц**
\`\`\`cpp
CPower().setCpuFrequency(40000000);
\`\`\`

**31. Динамическое изменение частоты в цикле**
\`\`\`cpp
CPower().setCpuFrequency(240000000);
// ... делаем что-то быстро ...
CPower().setCpuFrequency(40000000);
// ... ждем ...
\`\`\`

**32. Инициализация сразу в режиме LOW_POWER_ACTIVE**
\`\`\`cpp
CPower().begin(PowerProfile::LOW_POWER_ACTIVE);
\`\`\`

**33. Получение текущей частоты процессора**
\`\`\`cpp
uint32_t currentFreq = CPower().getCpuFrequency();
Serial.println(currentFreq);
\`\`\`

**34. Сценарий: быстрый замер, потом сон**
\`\`\`cpp
CPower().setProfile(PowerProfile::HIGH_PERFORMANCE);
// takeFastMeasurement();
CPower().setProfile(PowerProfile::DEEP_SLEEP_PREP);
ChronosPower::WakeupPolicy p34;
p34.wdt.timeoutMillis = 5000;
CPower().sleep(p34);
\`\`\`

**35. Возврат к стандартной частоте Arduino Uno**
\`\`\`cpp
CPower().setCpuFrequency(16000000);
\`\`\`

**36. Установка очень низкой частоты для RP2040 (Pico)**
\`\`\`cpp
CPower().setCpuFrequency(20000000); // 20 MHz
\`\`\`

**37. Установка максимальной частоты для RP2040 (Pico)**
\`\`\`cpp
CPower().setCpuFrequency(133000000);
\`\`\`

**38. Создание и применение своей политики**
\`\`\`cpp
ChronosPower::PowerPolicy customPolicy;
customPolicy.cpuFrequencyHz = 5000000;
customPolicy.peripheralMask[0] = (1ULL << (int)Peripheral::UART_0);
CPower().setCustomPolicy(customPolicy);
\`\`\`

**39. Переключение с кастомной политики обратно на стандартный профиль**
\`\`\`cpp
CPower().setProfile(PowerProfile::BALANCED_MOBILE);
\`\`\`

**40. Проверка текущего профиля**
\`\`\`cpp
if (CPower().getProfile() == PowerProfile::MAX_PERFORMANCE) { 
    Serial.println("Максимальная производительность активна");
}
\`\`\`

#### **Раздел 3: Ручное Управление Периферией (Примеры 41-70)**

**41. Отключить весь блок АЦП**
\`\`\`cpp
CPower().control(Peripheral::ADC_0_12BIT, false);
\`\`\`

**42. Отключить весь блок I2C**
\`\`\`cpp
CPower().control(Peripheral::I2C_0, false);
\`\`\`

**43. Отключить весь блок SPI**
\`\`\`cpp
CPower().control(Peripheral::SPI_0, false);
\`\`\`

**44. Отключить USB**
\`\`\`cpp
CPower().control(Peripheral::USB_DEVICE, false);
\`\`\`

**45. Отключить Wi-Fi на ESP32**
\`\`\`cpp
CPower().control(Peripheral::WIFI_MAC_2_4GHZ, false);
\`\`\`

**46. Отключить Bluetooth на ESP32**
\`\`\`cpp
CPower().control(Peripheral::BLUETOOTH_CONTROLLER, false);
\`\`\`

**47. Отключить Brown-Out-Detector (рискованно)**
\`\`\`cpp
CPower().control(Peripheral::BROWN_OUT_DETECTOR, false);
\`\`\`

**48. Отключить внутренний датчик температуры**
\`\`\`cpp
CPower().control(Peripheral::INTERNAL_TEMP_SENSOR, false);
\`\`\`

**49. Отключить аппаратный генератор случайных чисел**
\`\`\`cpp
CPower().control(Peripheral::TRNG_UNIT, false);
\`\`\`

**50. Отключить основной таймер Arduino (millis/delay перестанут работать)**
\`\`\`cpp
CPower().control(Peripheral::TIMER_0_8BIT_BASIC, false);
\`\`\`

**51. Включить таймер обратно**
\`\`\`cpp
CPower().control(Peripheral::TIMER_0_8BIT_BASIC, true);
\`\`\`

**52. Отключить все таймеры (0, 1, 2)**
\`\`\`cpp
CPower().control(Peripheral::TIMER_0_8BIT_BASIC, false);
CPower().control(Peripheral::TIMER_1_16BIT_GENERAL, false);
CPower().control(Peripheral::TIMER_2_8BIT_ASYNC, false);
\`\`\`

**53. Отключить всю периферию порта GPIOA**
\`\`\`cpp
CPower().control(Peripheral::GPIO_PORT_A, false);
\`\`\`

**54. Отключить Flash-кэш (замедлит выполнение)**
\`\`\`cpp
CPower().control(Peripheral::FLASH_CACHE_DATA, false);
\`\`\`

**55. Включить его обратно**
\`\`\`cpp
CPower().control(Peripheral::FLASH_CACHE_DATA, true);
\`\`\`

**56. Отключить контроллер SD-карты**
\`\`\`cpp
CPower().control(Peripheral::SDIO_0, false);
\`\`\`

**57. Отключить интерфейс камеры**
\`\`\`cpp
CPower().control(Peripheral::CAMERA_INTERFACE_DCMI, false);
\`\`\`

**58. Отключить Ethernet**
\`\`\`cpp
CPower().control(Peripheral::ETHERNET_MAC, false);
\`\`\`

**59. Отключить ШИМ-модуль**
\`\`\`cpp
CPower().control(Peripheral::PWM_MODULE_0, false);
\`\`\`

**60. Отключить DMA**
\`\`\`cpp
CPower().control(Peripheral::DMA_CONTROLLER_0, false);
\`\`\`

**61. Комбинации: оставить только UART и I2C**
\`\`\`cpp
CPower().setProfile(PowerProfile::ULTRA_LOW_POWER);
CPower().control(Peripheral::UART_0, true);
CPower().control(Peripheral::I2C_0, true);
\`\`\`

**62. Отключить второй UART**
\`\`\`cpp
CPower().control(Peripheral::UART_1, false);
\`\`\`

**63. Отключить второй SPI**
\`\`\`cpp
CPower().control(Peripheral::SPI_1, false);
\`\`\`

**64. Отключить второй I2C**
\`\`\`cpp
CPower().control(Peripheral::I2C_1, false);
\`\`\`

**65. Отключить все аналоговые компараторы**
\`\`\`cpp
CPower().control(Peripheral::ANALOG_COMPARATOR_0, false);
\`\`\`

**66. Отключить DAC**
\`\`\`cpp
CPower().control(Peripheral::DAC_0_8BIT, false);
\`\`\`

**67. Отключить второй ADC**
\`\`\`cpp
CPower().control(Peripheral::ADC_1_12BIT, false);
\`\`\`

**68. Отключить RTC**
\`\`\`cpp
CPower().control(Peripheral::RTC_INTERNAL, false);
\`\`\`

**69. Отключить Watchdog**
\`\`\`cpp
CPower().control(Peripheral::WATCHDOG_SYSTEM, false);
\`\`\`

**70. Включить все обратно**
\`\`\`cpp
CPower().setProfile(PowerProfile::MAX_PERFORMANCE);
\`\`\`

#### **Раздел 4: Диагностика и Информация (Примеры 71-100)**

**71. Вывести причину пробуждения в Serial**
\`\`\`cpp
Serial.println((int)CPower().getWakeupReason());
\`\`\`

**72. Среагировать на пробуждение от таймера**
\`\`\`cpp
if (CPower().getWakeupReason() == WakeupReason::WATCHDOG_TIMEOUT_INTERRUPT) {
    Serial.println("Проснулись от таймера");
}
\`\`\`

**73. Среагировать на пробуждение от кнопки на пине 2**
\`\`\`cpp
if (CPower().getWakeupReason() == WakeupReason::EXTERNAL_PIN_2) {
    Serial.println("Проснулись от кнопки на пине 2");
}
\`\`\`

**74. Среагировать на пробуждение от RTC**
\`\`\`cpp
if (CPower().getWakeupReason() == WakeupReason::RTC_ALARM_SECONDS) {
    Serial.println("Проснулись от RTC");
}
\`\`\`

**75. Вывести длительность последнего сна**
\`\`\`cpp
Serial.println(CPower().getSleepDurationMillis());
\`\`\`

**76. Использовать колбэк для логирования**
\`\`\`cpp
void loggerCallback(WakeupReason reason) { 
    Serial.println((int)reason); 
}
CPower().registerEventCallback(loggerCallback);
\`\`\`

**77. Измерить и вывести температуру чипа**
\`\`\`cpp
float t;
if(CPower().getInternalTemperature(t) == Status::OK) { 
    Serial.println(t); 
}
\`\`\`

**78. Измерить и вывести напряжение ядра**
\`\`\`cpp
float v;
if(CPower().getCoreVoltage(v) == Status::OK) { 
    Serial.println(v); 
}
\`\`\`

**79. Очистить флаг причины пробуждения**
\`\`\`cpp
CPower().clearWakeupReason();
\`\`\`

**80. Синхронизировать millis() после сна (обычно авто)**
\`\`\`cpp
CPower().synchronizeSystemTime();
\`\`\`

**81. Включить светодиод, если проснулись от кнопки**
\`\`\`cpp
if (CPower().getWakeupReason() == WakeupReason::EXTERNAL_PIN_0) {
    digitalWrite(LED_BUILTIN, HIGH);
}
\`\`\`

**82. Проверить причину сброса системы**
\`\`\`cpp
ResetReason reset = CPower().getResetReason();
if (reset == ResetReason::WATCHDOG_RESET) {
    Serial.println("Система была сброшена watchdog");
}
\`\`\`

**83. Проверить сброс от просадки питания**
\`\`\`cpp
if (CPower().getResetReason() == ResetReason::BROWN_OUT_RESET) {
    Serial.println("Сброс от просадки питания");
}
\`\`\`

**84. Проверить нормальный сброс**
\`\`\`cpp
if (CPower().getResetReason() == ResetReason::POWER_ON_RESET) {
    Serial.println("Нормальное включение");
}
\`\`\`

**85. Проверить внешний сброс**
\`\`\`cpp
if (CPower().getResetReason() == ResetReason::EXTERNAL_RESET) {
    Serial.println("Внешний сброс");
}
\`\`\`

**86. Проверить программный сброс**
\`\`\`cpp
if (CPower().getResetReason() == ResetReason::SOFTWARE_RESET) {
    Serial.println("Программный сброс");
}
\`\`\`

**87. Получить состояние Flash памяти**
\`\`\`cpp
FlashState state;
if (CPower().getFlashState(state) == Status::OK) {
    Serial.println("Flash состояние: " + String((int)state));
}
\`\`\`

**88. Проверить статус последней операции**
\`\`\`cpp
Status lastStatus = CPower().getLastOperationStatus();
if (lastStatus != Status::OK) {
    Serial.println("Ошибка: " + String((int)lastStatus));
}
\`\`\`

**89. Получить время работы системы**
\`\`\`cpp
uint32_t uptime = CPower().getUptimeSeconds();
Serial.println("Время работы: " + String(uptime) + " секунд");
\`\`\`

**90. Получить общее время сна**
\`\`\`cpp
uint32_t sleepTime = CPower().getTotalSleepTime();
Serial.println("Общее время сна: " + String(sleepTime) + " мс");
\`\`\`

**91. Проверить количество пробуждений**
\`\`\`cpp
uint16_t wakeups = CPower().getWakeupCount();
Serial.println("Количество пробуждений: " + String(wakeups));
\`\`\`

**92. Получить версию библиотеки**
\`\`\`cpp
String version = CPower().getLibraryVersion();
Serial.println("Версия ChronosPower: " + version);
\`\`\`

**93. Проверить поддержку платформы**
\`\`\`cpp
if (CPower().isPlatformSupported()) {
    Serial.println("Платформа полностью поддерживается");
}
\`\`\`

**94. Получить информацию о платформе**
\`\`\`cpp
String platform = CPower().getPlatformName();
Serial.println("Платформа: " + platform);
\`\`\`

**95. Проверить доступность функции**
\`\`\`cpp
if (CPower().isFeatureSupported(Feature::ADAPTIVE_SCALING)) {
    Serial.println("Адаптивное масштабирование поддерживается");
}
\`\`\`

**96. Получить максимальную частоту CPU**
\`\`\`cpp
uint32_t maxFreq = CPower().getMaxCpuFrequency();
Serial.println("Максимальная частота: " + String(maxFreq) + " Гц");
\`\`\`

**97. Получить минимальную частоту CPU**
\`\`\`cpp
uint32_t minFreq = CPower().getMinCpuFrequency();
Serial.println("Минимальная частота: " + String(minFreq) + " Гц");
\`\`\`

**98. Проверить доступность периферии**
\`\`\`cpp
if (CPower().isPeripheralAvailable(Peripheral::WIFI_MAC_2_4GHZ)) {
    Serial.println("WiFi доступен");
}
\`\`\`

**99. Получить список доступных профилей**
\`\`\`cpp
std::vector<PowerProfile> profiles = CPower().getAvailableProfiles();
Serial.println("Доступно профилей: " + String(profiles.size()));
\`\`\`

**100. Полная диагностика системы**
\`\`\`cpp
CPower().printSystemDiagnostics();
\`\`\`

#### **Раздел 5: Продвинутые Сценарии Пробуждения (Примеры 101-150)**

**101. Пробуждение по адресу 0x7E на шине UART0**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.uartWakeup.portIndex = 1; 
p.uartWakeup.wakeupOnAddressMatch = true; 
p.uartWakeup.address = 0x7E; 
CPower().sleep(p);
\`\`\`

**102. Пробуждение по стартовому биту на UART2**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.uartWakeup.portIndex = 3; 
p.uartWakeup.wakeupOnStartBit = true; 
CPower().sleep(p);
\`\`\`

**103. Пробуждение от аналогового компаратора 0**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.enableAnalogComparatorWakeup = true; 
CPower().sleep(p);
\`\`\`

**104. Сон с подавлением дребезга кнопки (50 мс)**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.extInterrupt.pinMask = (1ULL << 2); 
p.extInterrupt.triggerMode = FALLING; 
p.extInterrupt.debounceMicros = 50000; 
CPower().sleep(p);
\`\`\`

**105. Пробуждение по Wi-Fi пакету (ESP32)**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.enableWifiWakeup = true; 
CPower().sleep(p);
\`\`\`

**106. Пробуждение по Bluetooth соединению (ESP32)**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.enableBluetoothWakeup = true; 
CPower().sleep(p);
\`\`\`

**107. RTC-будильник на точное время (15:30:00)**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.rtc.alarmHour = 15; 
p.rtc.alarmMinute = 30; 
p.rtc.alarmSecond = 0; 
CPower().sleep(p);
\`\`\`

**108. RTC-будильник на конкретную дату**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.rtc.alarmHour=9; 
p.rtc.matchDate=true; 
p.rtc.alarmDay=1; 
p.rtc.alarmMonth=1; 
CPower().sleep(p);
\`\`\`

**109. Сон с оконным WDT (сброс в окне 50%-100%)**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.wdt.timeoutMillis=4000; 
p.wdt.windowedMode=true; 
p.wdt.windowPercent=50; 
p.wdt.generateInterrupt=false;
CPower().sleep(p);
\`\`\`

**110. Комбинация: Кнопка на пине 4 ИЛИ RTC через 30 сек**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.rtc.timeoutSeconds=30; 
p.extInterrupt.pinMask=(1ULL<<4); 
p.extInterrupt.triggerMode=FALLING; 
CPower().sleep(p);
\`\`\`

**111. Комбинация: Касание ИЛИ WDT на 15 секунд**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.wdt.timeoutMillis=15000; 
p.touchThreshold=40; 
CPower().sleep(p);
\`\`\`

**112. Комбинация: UART ИЛИ Кнопка на пине 5**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.uartWakeup.portIndex=1; 
p.extInterrupt.pinMask=(1ULL<<5); 
p.extInterrupt.triggerMode=RISING; 
CPower().sleep(p);
\`\`\`

**113. Пробуждение от ULP сопроцессора (ESP32)**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.enableUlpWakeup = true;
CPower().sleep(p);
\`\`\`

**114. Пробуждение по превышению порога АЦП**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.adcWakeup.enabled = true;
p.adcWakeup.channel = 0;
p.adcWakeup.threshold = 2048; // 12-bit ADC
p.adcWakeup.triggerMode = ADC_ABOVE_THRESHOLD;
CPower().sleep(p);
\`\`\`

**115. Гибернация до низкого уровня на пине с таймаутом в 1 час**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.rtc.timeoutSeconds=3600; 
p.extInterrupt.pinMask=(1ULL<<8); 
p.extInterrupt.triggerMode=LOW; 
CPower().hibernate(p);
\`\`\`

**116. Пробуждение от изменения на любом пине порта**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.extInterrupt.pinMask = 0xFF; // Пины 0-7
p.extInterrupt.triggerMode = CHANGE;
CPower().sleep(p);
\`\`\`

**117. Пробуждение от таймера с микросекундной точностью**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.highResTimer.timeoutMicros = 500000; // 500 мс
p.highResTimer.enabled = true;
CPower().sleep(p);
\`\`\`

**118. Пробуждение от CAN сообщения**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.canWakeup.enabled = true;
p.canWakeup.filterId = 0x123;
CPower().sleep(p);
\`\`\`

**119. Пробуждение от SPI транзакции**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.spiWakeup.enabled = true;
p.spiWakeup.portIndex = 0;
CPower().sleep(p);
\`\`\`

**120. Пробуждение от I2C адреса**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.i2cWakeup.enabled = true;
p.i2cWakeup.slaveAddress = 0x48;
CPower().sleep(p);
\`\`\`

**121. Пробуждение от USB подключения**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.usbWakeup.enabled = true;
p.usbWakeup.wakeupOnConnect = true;
CPower().sleep(p);
\`\`\`

**122. Пробуждение от Ethernet пакета**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.ethernetWakeup.enabled = true;
p.ethernetWakeup.magicPacket = true;
CPower().sleep(p);
\`\`\`

**123. Пробуждение от изменения магнитного поля**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.magneticWakeup.enabled = true;
p.magneticWakeup.sensitivity = 50;
CPower().sleep(p);
\`\`\`

**124. Пробуждение от акселерометра**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.motionWakeup.enabled = true;
p.motionWakeup.threshold = 100; // mg
CPower().sleep(p);
\`\`\`

**125. Пробуждение от звука (микрофон)**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.audioWakeup.enabled = true;
p.audioWakeup.volumeThreshold = 60; // dB
CPower().sleep(p);
\`\`\`

**126. Пробуждение от изменения освещенности**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.lightWakeup.enabled = true;
p.lightWakeup.luxThreshold = 100;
p.lightWakeup.triggerMode = LIGHT_INCREASE;
CPower().sleep(p);
\`\`\`

**127. Пробуждение от GPS сигнала**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.gpsWakeup.enabled = true;
p.gpsWakeup.wakeupOnFix = true;
CPower().sleep(p);
\`\`\`

**128. Пробуждение от NFC поля**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.nfcWakeup.enabled = true;
p.nfcWakeup.fieldDetection = true;
CPower().sleep(p);
\`\`\`

**129. Пробуждение от изменения температуры**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.temperatureWakeup.enabled = true;
p.temperatureWakeup.threshold = 25.0f; // °C
p.temperatureWakeup.triggerMode = TEMP_ABOVE;
CPower().sleep(p);
\`\`\`

**130. Пробуждение от изменения влажности**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.humidityWakeup.enabled = true;
p.humidityWakeup.threshold = 80.0f; // %
p.humidityWakeup.triggerMode = HUMIDITY_ABOVE;
CPower().sleep(p);
\`\`\`

**131. Пробуждение от изменения давления**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.pressureWakeup.enabled = true;
p.pressureWakeup.threshold = 1013.25f; // hPa
p.pressureWakeup.triggerMode = PRESSURE_BELOW;
CPower().sleep(p);
\`\`\`

**132. Пробуждение от RFID тега**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.rfidWakeup.enabled = true;
p.rfidWakeup.tagId = 0x12345678;
CPower().sleep(p);
\`\`\`

**133. Пробуждение от ИК сигнала**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.irWakeup.enabled = true;
p.irWakeup.protocol = IR_NEC;
p.irWakeup.command = 0x45;
CPower().sleep(p);
\`\`\`

**134. Пробуждение от радиосигнала**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.radioWakeup.enabled = true;
p.radioWakeup.frequency = 433920000; // 433.92 MHz
p.radioWakeup.rssiThreshold = -80; // dBm
CPower().sleep(p);
\`\`\`

**135. Пробуждение от вибрации**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.vibrationWakeup.enabled = true;
p.vibrationWakeup.sensitivity = 75;
CPower().sleep(p);
\`\`\`

**136. Пробуждение от изменения емкости**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.capacitiveWakeup.enabled = true;
p.capacitiveWakeup.channel = 2;
p.capacitiveWakeup.threshold = 1000;
CPower().sleep(p);
\`\`\`

**137. Пробуждение от ультразвука**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.ultrasonicWakeup.enabled = true;
p.ultrasonicWakeup.distanceThreshold = 50; // cm
p.ultrasonicWakeup.triggerMode = DISTANCE_BELOW;
CPower().sleep(p);
\`\`\`

**138. Пробуждение от изменения напряжения**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.voltageWakeup.enabled = true;
p.voltageWakeup.channel = 1;
p.voltageWakeup.threshold = 3.3f; // V
p.voltageWakeup.triggerMode = VOLTAGE_BELOW;
CPower().sleep(p);
\`\`\`

**139. Пробуждение от изменения тока**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.currentWakeup.enabled = true;
p.currentWakeup.threshold = 100.0f; // mA
p.currentWakeup.triggerMode = CURRENT_ABOVE;
CPower().sleep(p);
\`\`\`

**140. Пробуждение от изменения мощности**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.powerWakeup.enabled = true;
p.powerWakeup.threshold = 1.0f; // W
p.powerWakeup.triggerMode = POWER_ABOVE;
CPower().sleep(p);
\`\`\`

**141. Пробуждение от изменения частоты**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.frequencyWakeup.enabled = true;
p.frequencyWakeup.targetFrequency = 50.0f; // Hz
p.frequencyWakeup.tolerance = 1.0f; // Hz
CPower().sleep(p);
\`\`\`

**142. Пробуждение от цифрового паттерна**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.patternWakeup.enabled = true;
p.patternWakeup.pattern = 0b10101010;
p.patternWakeup.mask = 0xFF;
CPower().sleep(p);
\`\`\`

**143. Пробуждение от изменения скорости**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.speedWakeup.enabled = true;
p.speedWakeup.threshold = 10.0f; // km/h
p.speedWakeup.triggerMode = SPEED_ABOVE;
CPower().sleep(p);
\`\`\`

**144. Пробуждение от изменения угла**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.angleWakeup.enabled = true;
p.angleWakeup.threshold = 45.0f; // degrees
p.angleWakeup.axis = ANGLE_X;
CPower().sleep(p);
\`\`\`

**145. Пробуждение от изменения координат GPS**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.locationWakeup.enabled = true;
p.locationWakeup.latitude = 55.7558;
p.locationWakeup.longitude = 37.6176;
p.locationWakeup.radius = 100; // meters
CPower().sleep(p);
\`\`\`

**146. Пробуждение от изменения времени**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.timeWakeup.enabled = true;
p.timeWakeup.hour = 8;
p.timeWakeup.minute = 0;
p.timeWakeup.dayOfWeek = MONDAY;
CPower().sleep(p);
\`\`\`

**147. Пробуждение от календарного события**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.calendarWakeup.enabled = true;
p.calendarWakeup.day = 25;
p.calendarWakeup.month = 12;
p.calendarWakeup.year = 2024;
CPower().sleep(p);
\`\`\`

**148. Пробуждение от изменения фазы луны**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.lunarWakeup.enabled = true;
p.lunarWakeup.phase = FULL_MOON;
CPower().sleep(p);
\`\`\`

**149. Пробуждение от солнечного события**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.solarWakeup.enabled = true;
p.solarWakeup.event = SUNRISE;
p.solarWakeup.latitude = 55.7558;
p.solarWakeup.longitude = 37.6176;
CPower().sleep(p);
\`\`\`

**150. Комплексное пробуждение с множественными условиями**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.rtc.timeoutSeconds = 3600; // Максимум 1 час
p.extInterrupt.pinMask = (1ULL << 2);
p.extInterrupt.triggerMode = FALLING;
p.touchThreshold = 40;
p.temperatureWakeup.enabled = true;
p.temperatureWakeup.threshold = 30.0f;
p.motionWakeup.enabled = true;
p.motionWakeup.threshold = 50;
p.enableIntelligentWakeup = true;
CPower().sleep(p);
\`\`\`

#### **Раздел 6: Домены и Массовое Управление (Примеры 151-180)**

**151. Отключить все таймеры и ШИМ**
\`\`\`cpp
CPower().setClockGatingForDomain(PowerDomain::TIMERS_AND_PWM, false);
\`\`\`

**152. Включить все коммуникационные интерфейсы**
\`\`\`cpp
CPower().setClockGatingForDomain(PowerDomain::COMMUNICATIONS, true);
\`\`\`

**153. Отключить все модули безопасности (AES, SHA, TRNG)**
\`\`\`cpp
CPower().setClockGatingForDomain(PowerDomain::SECURITY_MODULES, false);
\`\`\`

**154. Отключить всё, кроме базовой системы и GPIO**
\`\`\`cpp
CPower().setClockGatingForDomain(PowerDomain::ALL, false); 
CPower().setClockGatingForDomain(PowerDomain::GPIO_PORTS, true);
\`\`\`

**155. Включение только UART1 и SPI2 через маску**
\`\`\`cpp
ChronosPower::PowerPolicy p; 
CPower().getCustomPolicy(p); 
for(int i=0;i<8;++i) p.peripheralMask[i]=0;
p.peripheralMask[0] |= (1ULL << (int)Peripheral::UART_1) | (1ULL << (int)Peripheral::SPI_2); 
CPower().setCustomPolicy(p);
\`\`\`

**156. Отключение всех банков SRAM, кроме первого**
\`\`\`cpp
CPower().control(Peripheral::SRAM_BANK_1, false); 
CPower().control(Peripheral::SRAM_BANK_2, false);
CPower().control(Peripheral::SRAM_BANK_3, false);
\`\`\`

**157. Массовое отключение всех GPIO портов через `massControl`**
\`\`\`cpp
uint64_t mask[8]={0}; 
mask[1] |= (1ULL<<((int)Peripheral::GPIO_PORT_A%64)) | (1ULL<<((int)Peripheral::GPIO_PORT_B%64));
mask[1] |= (1ULL<<((int)Peripheral::GPIO_PORT_C%64)) | (1ULL<<((int)Peripheral::GPIO_PORT_D%64));
CPower().massControl(mask, 8, false);
\`\`\`

**158. Динамическое создание маски из массива**
\`\`\`cpp
Peripheral list[] = {Peripheral::I2C_1, Peripheral::ADC_1_12BIT, Peripheral::DMA_CHANNEL_3}; 
uint64_t mask[8]={0};
for(Peripheral item : list) { 
    mask[(int)item/64] |= (1ULL<<(int)item%64); 
}
CPower().massControl(mask, 8, true);
\`\`\`

**159. Переключение с "MAX_PERFORMANCE" на "Только UART"**
\`\`\`cpp
CPower().setProfile(PowerProfile::MAX_PERFORMANCE); 
delay(1000);
CPower().setClockGatingForDomain(PowerDomain::ALL, false); 
CPower().control(Peripheral::UART_0, true);
\`\`\`

**160. Создание инвертированной маски: отключить только USB**
\`\`\`cpp
ChronosPower::PowerPolicy p; 
CPower().getCustomPolicy(p);
p.peripheralMask[(int)Peripheral::USB_DEVICE/64] &= ~(1ULL<<((int)Peripheral::USB_DEVICE%64)); 
CPower().setCustomPolicy(p);
\`\`\`

**161. Отключить все аналоговые блоки**
\`\`\`cpp
CPower().setClockGatingForDomain(PowerDomain::ANALOG_SUBSYSTEM, false);
\`\`\`

**162. Оставить только мультимедийные блоки**
\`\`\`cpp
CPower().setClockGatingForDomain(PowerDomain::ALL, false);
CPower().control(Peripheral::CAMERA_INTERFACE_DCMI, true); 
CPower().control(Peripheral::JPEG_CODEC, true); 
CPower().control(Peripheral::LCD_CONTROLLER_SPI, true);
\`\`\`

**163. Управление доменом памяти**
\`\`\`cpp
CPower().setClockGatingForDomain(PowerDomain::MEMORY_SYSTEM, false);
CPower().control(Peripheral::SRAM_BANK_0, true); // Оставить только основной банк
\`\`\`

**164. Управление радио доменом**
\`\`\`cpp
CPower().setClockGatingForDomain(PowerDomain::RADIO_SUBSYSTEM, false);
\`\`\`

**165. Управление доменом процессора**
\`\`\`cpp
CPower().setClockGatingForDomain(PowerDomain::CORE_LOGIC, true);
\`\`\`

**166. Создание группы сенсоров**
\`\`\`cpp
std::vector<Peripheral> sensors = {
    Peripheral::ADC_0_12BIT,
    Peripheral::ADC_1_12BIT,
    Peripheral::INTERNAL_TEMP_SENSOR,
    Peripheral::ANALOG_COMPARATOR_0
};
CPower().createPeripheralGroup("sensors", sensors);
\`\`\`

**167. Создание группы коммуникации**
\`\`\`cpp
std::vector<Peripheral> comm = {
    Peripheral::UART_0,
    Peripheral::UART_1,
    Peripheral::SPI_0,
    Peripheral::I2C_0,
    Peripheral::I2C_1
};
CPower().createPeripheralGroup("communication", comm);
\`\`\`

**168. Создание группы таймеров**
\`\`\`cpp
std::vector<Peripheral> timers = {
    Peripheral::TIMER_0_8BIT_BASIC,
    Peripheral::TIMER_1_16BIT_GENERAL,
    Peripheral::TIMER_2_8BIT_ASYNC,
    Peripheral::RTC_INTERNAL
};
CPower().createPeripheralGroup("timers", timers);
\`\`\`

**169. Использование групп для быстрого переключения**
\`\`\`cpp
// Режим измерения
CPower().disablePeripheralGroup("communication");
CPower().enablePeripheralGroup("sensors");

// Режим передачи данных
CPower().disablePeripheralGroup("sensors");
CPower().enablePeripheralGroup("communication");
\`\`\`

**170. Создание группы безопасности**
\`\`\`cpp
std::vector<Peripheral> security = {
    Peripheral::AES_ACCELERATOR_128,
    Peripheral::TRNG_UNIT,
    Peripheral::TAMPER_DETECTION
};
CPower().createPeripheralGroup("security", security);
\`\`\`

**171. Создание группы мультимедиа**
\`\`\`cpp
std::vector<Peripheral> multimedia = {
    Peripheral::CAMERA_INTERFACE_DCMI,
    Peripheral::JPEG_CODEC,
    Peripheral::LCD_CONTROLLER_SPI,
    Peripheral::AUDIO_CODEC_I2S
};
CPower().createPeripheralGroup("multimedia", multimedia);
\`\`\`

**172. Управление группами через условия**
\`\`\`cpp
if (batteryLevel < 20) {
    CPower().disablePeripheralGroup("multimedia");
    CPower().disablePeripheralGroup("security");
    CPower().enablePeripheralGroup("sensors");
} else {
    CPower().enablePeripheralGroup("multimedia");
    CPower().enablePeripheralGroup("security");
}
\`\`\`

**173. Удаление группы периферии**
\`\`\`cpp
CPower().removePeripheralGroup("multimedia");
\`\`\`

**174. Получение списка групп**
\`\`\`cpp
std::vector<std::string> groups = CPower().getPeripheralGroups();
for (const auto& group : groups) {
    Serial.println("Группа: " + group);
}
\`\`\`

**175. Проверка состояния группы**
\`\`\`cpp
if (CPower().isPeripheralGroupEnabled("sensors")) {
    Serial.println("Сенсоры включены");
}
\`\`\`

**176. Массовое управление через домены с исключениями**
\`\`\`cpp
CPower().setClockGatingForDomain(PowerDomain::ALL, false);
// Исключения
CPower().control(Peripheral::SYSTEM_CONTROLLER, true);
CPower().control(Peripheral::CPU_CORE_0, true);
CPower().control(Peripheral::SRAM_BANK_0, true);
\`\`\`

**177. Создание иерархии доменов**
\`\`\`cpp
// Отключить весь домен связи
CPower().setClockGatingForDomain(PowerDomain::COMMUNICATIONS, false);
// Но включить только UART для отладки
CPower().control(Peripheral::UART_0, true);
\`\`\`

**178. Управление питанием по приоритетам**
\`\`\`cpp
// Критически важные (всегда включены)
std::vector<Peripheral> critical = {
    Peripheral::SYSTEM_CONTROLLER,
    Peripheral::CPU_CORE_0,
    Peripheral::WATCHDOG_SYSTEM
};
CPower().createPeripheralGroup("critical", critical);

// Важные (включены при нормальном пи  ании)
std::vector<Peripheral> important = {
    Peripheral::UART_0,
    Peripheral::RTC_INTERNAL,
    Peripheral::FLASH_CONTROLLER
};
CPower().createPeripheralGroup("important", important);

// Опциональные (включены только при хорошем питании)
std::vector<Peripheral> optional = {
    Peripheral::WIFI_MAC_2_4GHZ,
    Peripheral::BLUETOOTH_CONTROLLER,
    Peripheral::USB_DEVICE
};
CPower().createPeripheralGroup("optional", optional);
\`\`\`

**179. Адаптивное управление доменами**
\`\`\`cpp
float batteryVoltage = CPower().getBatteryVoltage();
if (batteryVoltage > 3.7f) {
    CPower().enablePeripheralGroup("optional");
} else if (batteryVoltage > 3.3f) {
    CPower().disablePeripheralGroup("optional");
    CPower().enablePeripheralGroup("important");
} else {
    CPower().disablePeripheralGroup("optional");
    CPower().disablePeripheralGroup("important");
    CPower().enablePeripheralGroup("critical");
}
\`\`\`

**180. Сохранение и восстановление состояния доменов**
\`\`\`cpp
// Сохранить текущее состояние
CPower().savePeripheralState("backup");

// Изменить конфигурацию для специальной задачи
CPower().setClockGatingForDomain(PowerDomain::ALL, false);
CPower().enablePeripheralGroup("sensors");

// Выполнить задачу
performSpecialTask();

// Восстановить исходное состояние
CPower().restorePeripheralState("backup");
\`\`\`

#### **Раздел 7: Диагностика и Конечные Автоматы (Примеры 181-230)**

**181. Вывод точной причины перезагрузки**
\`\`\`cpp
Serial.println((int)CPower().getResetReason());
\`\`\`

**182. Реакция на сбой сторожевого таймера**
\`\`\`cpp
if (CPower().getResetReason()==ResetReason::WATCHDOG_RESET) { 
    Serial.println("Система была сброшена watchdog - возможна ошибка в коде");
    // Сохранить информацию об ошибке в EEPROM
}
\`\`\`

**183. Реакция на просадку питания**
\`\`\`cpp
if (CPower().getResetReason()==ResetReason::BROWN_OUT_RESET) { 
    CPower().setProfile(PowerProfile::LOW_POWER_ACTIVE);
    Serial.println("Обнаружена просадка питания - переход в режим экономии");
}
\`\`\`

**184. Логирование длительности каждого цикла сна**
\`\`\`cpp
long sleep_ms = CPower().getSleepDurationMillis(); 
Serial.println("Длительность сна: " + String(sleep_ms) + " мс");
\`\`\`

**185. Простой конечный автомат (FSM)**
\`\`\`cpp
enum State { IDLE, WORKING, SLEEPING }; 
State currentState = IDLE; 

void updateStateMachine() {
    switch(currentState) {
        case IDLE:
            if(CPower().getWakeupReason() == WakeupReason::EXTERNAL_PIN_0) { 
                currentState = WORKING;
                Serial.println("Переход в состояние WORKING");
            }
            break;
        case WORKING:
            // Выполнить работу
            performWork();
            currentState = SLEEPING;
            break;
        case SLEEPING:
            ChronosPower::WakeupPolicy p;
            p.wdt.timeoutMillis = 5000;
            CPower().sleep(p);
            currentState = IDLE;
            break;
    }
}
\`\`\`

**186. Очистка флага пробуждения**
\`\`\`cpp
if (CPower().getWakeupReason()==WakeupReason::RTC_ALARM_SECONDS) { 
    Serial.println("Обработка RTC пробуждения");
    CPower().clearWakeupReason(); 
}
\`\`\`

**187. Использование колбэка для отладки**
\`\`\`cpp
void debugCallback(WakeupReason r) { 
    Serial.println("Пробуждение: " + String((int)r) + " в " + String(millis()) + " мс"); 
} 
CPower().registerEventCallback(debugCallback);
\`\`\`

**188. Проверка состояния Flash перед записью**
\`\`\`cpp
FlashState s; 
if (CPower().getFlashState(s)==Status::OK && s==FlashState::ACTIVE) { 
    Serial.println("Flash готов для записи");
    // Выполнить запись
} else {
    Serial.println("Flash не готов");
}
\`\`\`

**189. Полный обработчик причин сброса**
\`\`\`cpp
void handleResetReason() {
    switch(CPower().getResetReason()) {
        case ResetReason::POWER_ON_RESET:
            Serial.println("Нормальное включение");
            break;
        case ResetReason::WATCHDOG_RESET:
            Serial.println("Сброс от watchdog - проверить код на зависания");
            break;
        case ResetReason::BROWN_OUT_RESET:
            Serial.println("Сброс от просадки питания");
            CPower().setProfile(PowerProfile::LOW_POWER_ACTIVE);
            break;
        case ResetReason::EXTERNAL_RESET:
            Serial.println("Внешний сброс");
            break;
        case ResetReason::SOFTWARE_RESET:
            Serial.println("Программный сброс");
            break;
        case ResetReason::LOCKUP_RESET:
            Serial.println("Сброс от блокировки процессора");
            break;
        case ResetReason::CLOCK_FAILURE_SYSTEM:
            Serial.println("Сбой системного тактирования");
            break;
        default:
            Serial.println("Неизвестная причина сброса");
            break;
    }
}
\`\`\`

**190. Счетчик разных типов пробуждений**
\`\`\`cpp
int wdt_wakes=0, btn_wakes=0, rtc_wakes=0; 
WakeupReason r = CPower().getWakeupReason();
switch(r) {
    case WakeupReason::WATCHDOG_TIMEOUT_INTERRUPT: 
        wdt_wakes++; 
        break;
    case WakeupReason::EXTERNAL_PIN_0: 
        btn_wakes++; 
        break;
    case WakeupReason::RTC_ALARM_SECONDS:
        rtc_wakes++;
        break;
}
Serial.println("Статистика: WDT=" + String(wdt_wakes) + 
               ", BTN=" + String(btn_wakes) + 
               ", RTC=" + String(rtc_wakes));
\`\`\`

**191. Система с тремя состояниями: Сбор, Передача, Сон**
\`\`\`cpp
enum SystemState { COLLECT_DATA, TRANSMIT_DATA, DEEP_SLEEP };
SystemState state = DEEP_SLEEP;

void runStateMachine() {
    switch(state) {
        case DEEP_SLEEP:
            if (CPower().getWakeupReason() == WakeupReason::RTC_ALARM_SECONDS) {
                state = COLLECT_DATA;
                Serial.println("Время сбора данных");
            } else if (CPower().getWakeupReason() == WakeupReason::EXTERNAL_PIN_0) {
                state = TRANSMIT_DATA;
                Serial.println("Запрос передачи данных");
            }
            break;
            
        case COLLECT_DATA:
            collectSensorData();
            state = DEEP_SLEEP;
            Serial.println("Данные собраны, возврат в сон");
            break;
            
        case TRANSMIT_DATA:
            transmitStoredData();
            state = DEEP_SLEEP;
            Serial.println("Данные переданы, возврат в сон");
            break;
    }
}
\`\`\`

**192. Проверка сбоя тактирования**
\`\`\`cpp
if (CPower().getResetReason() == ResetReason::CLOCK_FAILURE_SYSTEM) { 
    Serial.println("Сбой тактирования - переход на внутренний генератор");
    CPower().setClockSource(ClockSource::INTERNAL_RC);
}
\`\`\`

**193. Система логирования событий**
\`\`\`cpp
struct LogEntry {
    unsigned long timestamp;
    WakeupReason reason;
    uint32_t sleepDuration;
};

LogEntry eventLog[100];
int logIndex = 0;

void logWakeupEvent() {
    if (logIndex < 100) {
        eventLog[logIndex].timestamp = millis();
        eventLog[logIndex].reason = CPower().getWakeupReason();
        eventLog[logIndex].sleepDuration = CPower().getSleepDurationMillis();
        logIndex++;
    }
}

void printEventLog() {
    for (int i = 0; i < logIndex; i++) {
        Serial.println("Событие " + String(i) + ": время=" + String(eventLog[i].timestamp) + 
                      ", причина=" + String((int)eventLog[i].reason) + 
                      ", сон=" + String(eventLog[i].sleepDuration) + "мс");
    }
}
\`\`\`

**194. Автоматическое восстановление после ошибок**
\`\`\`cpp
void handleSystemErrors() {
    ResetReason reset = CPower().getResetReason();
    
    if (reset == ResetReason::WATCHDOG_RESET || reset == ResetReason::LOCKUP_RESET) {
        Serial.println("Обнаружена критическая ошибка - активация безопасного режима");
        
        // Переход в безопасный режим
        CPower().setProfile(PowerProfile::ULTRA_LOW_POWER);
        
        // Отключить все несущественные периферийные устройства
        CPower().setClockGatingForDomain(PowerDomain::ALL, false);
        CPower().control(Peripheral::UART_0, true); // Только для отладки
        CPower().control(Peripheral::WATCHDOG_SYSTEM, true); // Для безопасности
        
        // Увеличить интервал watchdog
        CPower().setWatchdogTimeout(10000); // 10 секунд
    }
}
\`\`\`

**195. Мониторинг производительности системы**
\`\`\`cpp
struct PerformanceStats {
    unsigned long totalUptime;
    unsigned long totalSleepTime;
    uint16_t wakeupCount;
    float averageSleepDuration;
    float systemEfficiency;
};

PerformanceStats getSystemStats() {
    PerformanceStats stats;
    stats.totalUptime = CPower().getTotalUptime();
    stats.totalSleepTime = CPower().getTotalSleepTime();
    stats.wakeupCount = CPower().getWakeupCount();
    
    if (stats.wakeupCount > 0) {
        stats.averageSleepDuration = (float)stats.totalSleepTime / stats.wakeupCount;
    }
    
    stats.systemEfficiency = (float)stats.totalSleepTime / 
                           (stats.totalSleepTime + stats.totalUptime) * 100.0f;
    
    return stats;
}
\`\`\`

**196. Адаптивная система управления ошибками**
\`\`\`cpp
int errorCount = 0;
unsigned long lastErrorTime = 0;

void handleError(Status error) {
    errorCount++;
    lastErrorTime = millis();
    
    Serial.println("Ошибка #" + String(errorCount) + ": " + String((int)error));
    
    if (errorCount > 5 && (millis() - lastErrorTime) < 60000) {
        Serial.println("Слишком много ошибок - переход в аварийный режим");
        
        // Аварийный режим
        CPower().setProfile(PowerProfile::EMERGENCY_POWER);
        
        // Отправить сигнал бедствия
        sendDistressSignal();
        
        // Глубокий сон на длительное время
        ChronosPower::WakeupPolicy emergency;
        emergency.rtc.timeoutSeconds = 3600; // 1 час
        CPower().hibernate(emergency);
    }
}
\`\`\`

**197. Система предиктивной диагностики**
\`\`\`cpp
void predictiveHealthCheck() {
    float temperature;
    float voltage;
    
    if (CPower().getInternalTemperature(temperature) == Status::OK) {
        if (temperature > 80.0f) {
            Serial.println("ПРЕДУПРЕЖДЕНИЕ: Критическая температура!");
            CPower().optimizeForThermal();
        } else if (temperature > 70.0f) {
            Serial.println("Высокая температура - снижение производительности");
            CPower().setCpuFrequency(CPower().getCpuFrequency() * 0.8f);
        }
    }
    
    if (CPower().getCoreVoltage(voltage) == Status::OK) {
        if (voltage < 3.0f) {
            Serial.println("КРИТИЧЕСКИ низкое напряжение!");
            CPower().setProfile(PowerProfile::EMERGENCY_POWER);
        } else if (voltage < 3.3f) {
            Serial.println("Низкое напряжение - режим экономии");
            CPower().setProfile(PowerProfile::ULTRA_LOW_POWER);
        }
    }
}
\`\`\`

**198. Система самодиагностики**
\`\`\`cpp
bool performSelfDiagnostic() {
    Serial.println("Запуск самодиагностики...");
    
    // Проверка CPU
    uint32_t expectedFreq = 16000000;
    uint32_t actualFreq = CPower().getCpuFrequency();
    if (abs((int)(actualFreq - expectedFreq)) > 1000000) {
        Serial.println("ОШИБКА: Неправильная частота CPU");
        return false;
    }
    
    // Проверка памяти
    if (!CPower().isPeripheralAvailable(Peripheral::SRAM_BANK_0)) {
        Serial.println("ОШИБКА: Проблемы с памятью");
        return false;
    }
    
    // Проверка периферии
    if (!CPower().isPeripheralAvailable(Peripheral::UART_0)) {
        Serial.println("ПРЕДУПРЕЖДЕНИЕ: UART недоступен");
    }
    
    // Проверка питания
    float voltage;
    if (CPower().getCoreVoltage(voltage) == Status::OK) {
        if (voltage < 2.7f || voltage > 5.5f) {
            Serial.println("ОШИБКА: Напряжение вне допустимых пределов");
            return false;
        }
    }
    
    Serial.println("Самодиагностика пройдена успешно");
    return true;
}
\`\`\`

**199. Система мониторинга жизненных показателей**
\`\`\`cpp
struct VitalSigns {
    float temperature;
    float voltage;
    uint32_t frequency;
    uint16_t freeMemory;
    bool systemHealthy;
};

VitalSigns getVitalSigns() {
    VitalSigns signs;
    
    CPower().getInternalTemperature(signs.temperature);
    CPower().getCoreVoltage(signs.voltage);
    signs.frequency = CPower().getCpuFrequency();
    signs.freeMemory = CPower().getFreeMemory();
    
    // Оценка общего состояния
    signs.systemHealthy = (signs.temperature < 75.0f) && 
                         (signs.voltage > 3.0f) && 
                         (signs.freeMemory > 512);
    
    return signs;
}

void monitorVitalSigns() {
    VitalSigns signs = getVitalSigns();
    
    Serial.println("=== Жизненные показатели ===");
    Serial.println("Температура: " + String(signs.temperature) + "°C");
    Serial.println("Напряжение: " + String(signs.voltage) + "V");
    Serial.println("Частота: " + String(signs.frequency) + " Гц");
    Serial.println("Свободная память: " + String(signs.freeMemory) + " байт");
    Serial.println("Состояние: " + String(signs.systemHealthy ? "Здоров" : "Требует внимания"));
    
    if (!signs.systemHealthy) {
        Serial.println("ВНИМАНИЕ: Система требует обслуживания!");
    }
}
\`\`\`

**200. Интеллектуальная система восстановления**
\`\`\`cpp
void intelligentRecovery() {
    static int recoveryAttempts = 0;
    const int maxRecoveryAttempts = 3;
    
    if (CPower().getResetReason() != ResetReason::POWER_ON_RESET) {
        recoveryAttempts++;
        
        Serial.println("Попытка восстановления #" + String(recoveryAttempts));
        
        if (recoveryAttempts <= maxRecoveryAttempts) {
            // Постепенное снижение производительности для стабилизации
            switch(recoveryAttempts) {
                case 1:
                    CPower().setProfile(PowerProfile::BALANCED_MOBILE);
                    Serial.println("Переход в сбалансированный режим");
                    break;
                case 2:
                    CPower().setProfile(PowerProfile::LOW_POWER_ACTIVE);
                    Serial.println("Переход в режим низкого энергопотребления");
                    break;
                case 3:
                    CPower().setProfile(PowerProfile::ULTRA_LOW_POWER);
                    Serial.println("Переход в ультра-низкий режим");
                    break;
            }
        } else {
            Serial.println("Максимальное количество попыток восстановления превышено");
            Serial.println("Переход в аварийный режим");
            
            // Аварийный режим с минимальной функциональностью
            CPower().setProfile(PowerProfile::EMERGENCY_POWER);
            
            // Сохранить диагностическую информацию
            saveDiagnosticInfo();
            
            // Длительный сон для "остывания" системы
            ChronosPower::WakeupPolicy emergency;
            emergency.rtc.timeoutSeconds = 1800; // 30 минут
            CPower().hibernate(emergency);
        }
    } else {
        // Нормальное включение - сбросить счетчик
        recoveryAttempts = 0;
        Serial.println("Нормальное включение системы");
    }
}
\`\`\`

**201-230. Дополнительные примеры конечных автоматов, систем мониторинга и диагностики...**

#### **Раздел 8: Специализированные Политики (Примеры 231-260)**

**231. Использование профиля для аудио**
\`\`\`cpp
CPower().setProfile(PowerProfile::AUDIO_PROCESSING);
\`\`\`

**232. Использование профиля для ML**
\`\`\`cpp
CPower().setProfile(PowerProfile::MACHINE_LEARNING_INFERENCE);
\`\`\`

**233. Создание своей политики для LoRa-узла**
\`\`\`cpp
PowerPolicy loraPolicy; 
loraPolicy.cpuFrequencyHz=10000000; 
loraPolicy.flashDeepPowerDown=true;
loraPolicy.peripheralMask[0] |= (1ULL<<(int)Peripheral::UART_0)|(1ULL<<(int)Peripheral::SPI_0)|(1ULL<<(int)Peripheral::RTC_INTERNAL);
CPower().setCustomPolicy(loraPolicy);
\`\`\`

**234. Политика для USB HID устройства**
\`\`\`cpp
PowerPolicy hidPolicy; 
hidPolicy.cpuFrequencyHz=48000000;
hidPolicy.peripheralMask[0] |= (1ULL<<(int)Peripheral::USB_DEVICE)|(1ULL<<(int)Peripheral::GPIO_PORT_B);
CPower().setCustomPolicy(hidPolicy);
\`\`\`

**235. Динамическое изменение политики: день/ночь**
\`\`\`cpp
bool isDayTime() {
    // Логика определения времени суток
    int hour = getCurrentHour();
    return (hour >= 6 && hour < 22);
}

if (isDayTime()) {
    CPower().setProfile(PowerProfile::BALANCED_MOBILE);
    Serial.println("Дневной режим активирован");
} else {
    CPower().setProfile(PowerProfile::ULTRA_LOW_POWER);
    Serial.println("Ночной режим активирован");
}
\`\`\`

**236. Политика для высокоскоростного сбора данных (DAQ)**
\`\`\`cpp
PowerPolicy daqPolicy; 
daqPolicy.cpuFrequencyHz=240000000;
daqPolicy.peripheralMask[0] |= (1ULL<<(int)Peripheral::ADC_2_16BIT)|(1ULL<<(int)Peripheral::DMA_CONTROLLER_0)|(1ULL<<(int)Peripheral::TIMER_6_16BIT_ADVANCED);
CPower().setCustomPolicy(daqPolicy);
\`\`\`

**237. Политика для "тихого" режима: без радио**
\`\`\`cpp
PowerPolicy silentPolicy; 
CPower().getCustomPolicy(silentPolicy);
silentPolicy.peripheralMask[(int)Peripheral::WIFI_MAC_2_4GHZ/64] &= ~(1ULL<<((int)Peripheral::WIFI_MAC_2_4GHZ%64));
silentPolicy.peripheralMask[(int)Peripheral::BLUETOOTH_CONTROLLER/64] &= ~(1ULL<<((int)Peripheral::BLUETOOTH_CONTROLLER%64));
CPower().setCustomPolicy(silentPolicy);
\`\`\`

**238. Политика для ЧПУ-контроллера (CNC)**
\`\`\`cpp
PowerPolicy cncPolicy; 
cncPolicy.cpuFrequencyHz=120000000;
cncPolicy.peripheralMask[0] |= (1ULL<<(int)Peripheral::PWM_MODULE_2_MOTOR_CONTROL)|(1ULL<<(int)Peripheral::QUADRATURE_ENCODER_0);
CPower().setCustomPolicy(cncPolicy);
\`\`\`

**239. Политика для IoT сенсорного узла**
\`\`\`cpp
PowerPolicy iotPolicy;
iotPolicy.cpuFrequencyHz = 8000000; // Низкая частота для экономии
iotPolicy.flashDeepPowerDown = true;
iotPolicy.voltageScaleLevel = 3; // Минимальное напряжение
iotPolicy.enableDynamicVoltageScaling = true;

// Включить только необходимые периферийные устройства
for(int i = 0; i < 8; i++) iotPolicy.peripheralMask[i] = 0;
iotPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::SYSTEM_CONTROLLER);
iotPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::RTC_INTERNAL);
iotPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ADC_0_12BIT);
iotPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::I2C_0);

CPower().setCustomPolicy(iotPolicy);
\`\`\`

**240. Политика для носимого устройства**
\`\`\`cpp
PowerPolicy wearablePolicy;
wearablePolicy.cpuFrequencyHz = 32000000;
wearablePolicy.enableDynamicVoltageScaling = true;
wearablePolicy.flashDeepPowerDown = false; // Для быстрого отклика

// Периферия для носимых устройств
wearablePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::BLUETOOTH_CONTROLLER);
wearablePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ADC_0_12BIT); // Для сенсоров
wearablePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::I2C_0); // Для акселерометра
wearablePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::PWM_MODULE_0); // Для вибромотора

CPower().setCustomPolicy(wearablePolicy);
\`\`\`

**241. Политика для дрона**
\`\`\`cpp
PowerPolicy dronePolicy;
dronePolicy.cpuFrequencyHz = 168000000; // Высокая частота для управления
dronePolicy.enableDynamicVoltageScaling = false; // Стабильность важнее экономии

// Периферия для дрона
dronePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::PWM_MODULE_0); // Моторы
dronePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::PWM_MODULE_1);
dronePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::PWM_MODULE_2);
dronePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::PWM_MODULE_3);
dronePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::I2C_0); // IMU
dronePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::SPI_0); // GPS
dronePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::UART_0); // Телеметрия

CPower().setCustomPolicy(dronePolicy);
\`\`\`

**242. Политика для умного дома**
\`\`\`cpp
PowerPolicy smartHomePolicy;
smartHomePolicy.cpuFrequencyHz = 80000000;
smartHomePolicy.enableDynamicVoltageScaling = true;

// Периферия для умного дома
smartHomePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::WIFI_MAC_2_4GHZ);
smartHomePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ETHERNET_MAC);
smartHomePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::I2C_0); // Сенсоры
smartHomePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::SPI_0); // SD карта
smartHomePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::PWM_MODULE_0); // Диммеры

CPower().setCustomPolicy(smartHomePolicy);
\`\`\`

**243. Политика для медицинского устройства**
\`\`\`cpp
PowerPolicy medicalPolicy;
medicalPolicy.cpuFrequencyHz = 64000000;
medicalPolicy.enableDynamicVoltageScaling = false; // Стабильность критична
medicalPolicy.flashDeepPowerDown = false; // Быстрый доступ к данным

// Высокая надежность
medicalPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::WATCHDOG_SYSTEM);
medicalPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::BROWN_OUT_DETECTOR);
medicalPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ADC_0_12BIT); // Высокоточные измерения
medicalPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ADC_1_12BIT);
medicalPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::UART_0); // Связь с ПК

CPower().setCustomPolicy(medicalPolicy);
\`\`\`

**244. Политика для автомобильной системы**
\`\`\`cpp
PowerPolicy automotivePolicy;
automotivePolicy.cpuFrequencyHz = 100000000;
automotivePolicy.enableDynamicVoltageScaling = true;
automotivePolicy.voltageScaleLevel = 1; // Средний уровень для стабильности

// Автомобильная периферия
automotivePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::CAN_CONTROLLER_0);
automotivePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::CAN_CONTROLLER_1);
automotivePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ADC_0_12BIT); // Датчики
automotivePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::PWM_MODULE_0); // Управление
automotivePolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::UART_0); // OBD-II

CPower().setCustomPolicy(automotivePolicy);
\`\`\`

**245. Политика для промышленного контроллера**
\`\`\`cpp
PowerPolicy industrialPolicy;
industrialPolicy.cpuFrequencyHz = 120000000;
industrialPolicy.enableDynamicVoltageScaling = false; // Стабильность в промышленности
industrialPolicy.flashDeepPowerDown = false;

// Промышленная периферия
industrialPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ETHERNET_MAC);
industrialPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::RS485_CONTROLLER);
industrialPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::MODBUS_CONTROLLER);
industrialPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ADC_0_12BIT);
industrialPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::DAC_0_8BIT);

CPower().setCustomPolicy(industrialPolicy);
\`\`\`

**246. Политика для игрового контроллера**
\`\`\`cpp
PowerPolicy gamingPolicy;
gamingPolicy.cpuFrequencyHz = 180000000; // Высокая частота для отзывчивости
gamingPolicy.enableDynamicVoltageScaling = true;

// Игровая периферия
gamingPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::USB_DEVICE); // HID
gamingPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::BLUETOOTH_CONTROLLER);
gamingPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ADC_0_12BIT); // Аналоговые стики
gamingPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::PWM_MODULE_0); // Вибрация
gamingPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::I2C_0); // Акселерометр

CPower().setCustomPolicy(gamingPolicy);
\`\`\`

**247. Политика для метеостанции**
\`\`\`cpp
PowerPolicy weatherPolicy;
weatherPolicy.cpuFrequencyHz = 16000000; // Низкая частота для экономии
weatherPolicy.enableDynamicVoltageScaling = true;
weatherPolicy.flashDeepPowerDown = true;

// Метеорологическая периферия
weatherPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ADC_0_12BIT); // Датчики
weatherPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::I2C_0); // Барометр, влажность
weatherPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::SPI_0); // SD карта
weatherPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::WIFI_MAC_2_4GHZ); // Передача данных
weatherPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::RTC_INTERNAL); // Временные метки

CPower().setCustomPolicy(weatherPolicy);
\`\`\`

**248. Политика для системы безопасности**
\`\`\`cpp
PowerPolicy securityPolicy;
securityPolicy.cpuFrequencyHz = 100000000;
securityPolicy.enableDynamicVoltageScaling = true;

// Периферия безопасности
securityPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::CAMERA_INTERFACE_DCMI);
securityPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::WIFI_MAC_2_4GHZ);
securityPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ETHERNET_MAC);
securityPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::AES_ACCELERATOR_128); // Шифрование
securityPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::MOTION_DETECTOR); // PIR
securityPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::AUDIO_CODEC_I2S); // Микрофон

CPower().setCustomPolicy(securityPolicy);
\`\`\`

**249. Политика для робота**
\`\`\`cpp
PowerPolicy robotPolicy;
robotPolicy.cpuFrequencyHz = 200000000; // Высокая частота для обработки
robotPolicy.enableDynamicVoltageScaling = false; // Стабильность для моторов

// Робототехническая периферия
robotPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::PWM_MODULE_0); // Сервоприводы
robotPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::PWM_MODULE_1);
robotPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::PWM_MODULE_2);
robotPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::QUADRATURE_ENCODER_0); // Энкодеры
robotPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::QUADRATURE_ENCODER_1);
robotPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::I2C_0); // Сенсоры
robotPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::UART_0); // Связь
robotPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ADC_0_12BIT); // Датчики тока

CPower().setCustomPolicy(robotPolicy);
\`\`\`

**250. Политика для научного инструмента**
\`\`\`cpp
PowerPolicy scientificPolicy;
scientificPolicy.cpuFrequencyHz = 168000000;
scientificPolicy.enableDynamicVoltageScaling = false; // Точность измерений
scientificPolicy.voltageScaleLevel = 0; // Максимальная стабильность

// Научная периферия
scientificPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ADC_0_16BIT); // Высокоточный АЦП
scientificPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ADC_1_16BIT);
scientificPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::DAC_0_16BIT); // Высокоточный ЦАП
scientificPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::DMA_CONTROLLER_0); // Быстрая передача
scientificPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::TIMER_PRECISION_64BIT); // Точное время
scientificPolicy.peripheralMask[0] |= (1ULL << (int)Peripheral::ETHERNET_MAC); // Передача данных

CPower().setCustomPolicy(scientificPolicy);
\`\`\`

**251-260. Дополнительные специализированные политики для различных применений...**

#### **Раздел 9: Платформо-зависимые Техники (Примеры 261-301)**

**261. ESP32: Пробуждение от ULP-сопроцессор  **
\`\`\`cpp
// Загрузка программы ULP
ChronosPower::WakeupPolicy p; 
p.enableUlpWakeup=true; 
CPower().sleep(p);
\`\`\`

**262. ESP32: Пробуждение по касанию с изоляцией пинов**
\`\`\`cpp
// rtc_gpio_isolate(GPIO_NUM_12);
ChronosPower::WakeupPolicy p; 
p.touchThreshold=35; 
CPower().sleep(p);
\`\`\`

**263. AVR: Точный сон на 30 секунд (цикл WDT)**
\`\`\`cpp
ChronosPower::WakeupPolicy p8s, p4s, p2s; 
p8s.wdt.timeoutMillis=8000; 
p4s.wdt.timeoutMillis=4000; 
p2s.wdt.timeoutMillis=2000;
CPower().sleep(p8s); 
CPower().sleep(p8s); 
CPower().sleep(p8s); 
CPower().sleep(p4s); 
CPower().sleep(p2s);
\`\`\`

**264. RP2040: Пробуждение от RTC-будильника**
\`\`\`cpp
ChronosPower::WakeupPolicy p; 
p.rtc.timeoutSeconds=120; 
CPower().hibernate(p);
\`\`\`

**265. STM32: Использование LPUART для сна**
\`\`\`cpp
CPower().control(Peripheral::UART_LP_0, true); 
ChronosPower::WakeupPolicy p; 
p.uartWakeup.portIndex=5; 
CPower().sleep(p);
\`\`\`

**266. ESP32: Отключение RTC периферии перед сном**
\`\`\`cpp
// esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
ChronosPower::WakeupPolicy p;
p.wdt.timeoutMillis = 5000;
CPower().sleep(p);
\`\`\`

**267. AVR: Использование Pin Change Interrupt для пробуждения**
\`\`\`cpp
// Настройка Pin Change Interrupt для пина PB0 (Arduino D8)
// PCICR |= (1 << PCIE0); // Enable PortB Pin Change Interrupts
// PCMSK0 |= (1 << PCINT0); // Enable on Pin PB0
ChronosPower::WakeupPolicy p;
p.extInterrupt.pinMask = (1ULL << 8);
p.extInterrupt.triggerMode = CHANGE;
CPower().sleep(p);
\`\`\`

**268. AVR: Использование разных режимов сна**
\`\`\`cpp
// Режим IDLE - CPU останавливается, периферия работает
CPower().setProfile(PowerProfile::IDLE_CPU_ONLY);
ChronosPower::WakeupPolicy idle;
idle.wdt.timeoutMillis = 1000;
CPower().sleep(idle);

// Режим ADC Noise Reduction - для точных измерений АЦП
CPower().setProfile(PowerProfile::ADC_NOISE_REDUCTION);
ChronosPower::WakeupPolicy adc;
adc.adcWakeup.enabled = true;
CPower().sleep(adc);

// Режим Power-save - асинхронный таймер работает
CPower().setProfile(PowerProfile::POWER_SAVE_ASYNC_TIMER);
ChronosPower::WakeupPolicy powersave;
powersave.rtc.timeoutSeconds = 10;
CPower().sleep(powersave);
\`\`\`

**269. AVR: Отключение BOD непосредственно перед сном**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.wdt.timeoutMillis = 5000;
p.disableBrownOutDetector = true; // Отключить BOD для экономии
CPower().sleep(p);
\`\`\`

**270. RP2040: Пробуждение от спящего режима через GPIO**
\`\`\`cpp
// Настройка GPIO для пробуждения из dormant режима
ChronosPower::WakeupPolicy p;
p.extInterrupt.pinMask = (1ULL << 2);
p.extInterrupt.triggerMode = LOW;
p.enableDormantMode = true; // Специальный режим RP2040
CPower().sleep(p);
\`\`\`

**271. STM32: Вход в режим STANDBY**
\`\`\`cpp
// Режим STANDBY с сохранением только RTC
ChronosPower::WakeupPolicy p;
p.rtc.timeoutSeconds = 3600;
p.enableStandbyMode = true; // STM32 STANDBY
p.wakeupPin = 0; // WKUP pin
CPower().hibernate(p);
\`\`\`

**272. STM32: Вход в режим STOP с сохранением SRAM**
\`\`\`cpp
// Режим STOP - SRAM сохраняется, низкое энергопотребление
ChronosPower::WakeupPolicy p;
p.extInterrupt.pinMask = (1ULL << 0);
p.extInterrupt.triggerMode = RISING;
p.enableStopMode = true; // STM32 STOP
p.regulatorLowPower = true; // Регулятор в режиме низкого энергопотребления
CPower().sleep(p);
\`\`\`

**273. ESP32: Калибровка RTC таймера для точного сна**
\`\`\`cpp
// Калибровка медленного тактового генератора RTC
CPower().calibrateRtcClock();

ChronosPower::WakeupPolicy p;
p.rtc.timeoutSeconds = 60;
p.rtc.useCalibration = true; // Использовать калибровку
CPower().sleep(p);
\`\`\`

**274. ESP32: Пробуждение по нескольким пинам (логическое ИЛИ)**
\`\`\`cpp
// Пробуждение при HIGH на любом из указанных пинов
ChronosPower::WakeupPolicy p;
p.extInterrupt.pinMask = (1ULL << 2) | (1ULL << 4) | (1ULL << 15);
p.extInterrupt.triggerMode = HIGH;
p.extInterrupt.logicMode = LOGIC_OR; // Любой пин
CPower().sleep(p);
\`\`\`

**275. ESP32: Пробуждение по нескольким пинам (логическое И)**
\`\`\`cpp
// Пробуждение только когда ВСЕ пины в состоянии LOW
ChronosPower::WakeupPolicy p;
p.extInterrupt.pinMask = (1ULL << 2) | (1ULL << 4);
p.extInterrupt.triggerMode = LOW;
p.extInterrupt.logicMode = LOGIC_AND; // Все пины
CPower().sleep(p);
\`\`\`

**276. RP2040: Запуск кода на втором ядре перед сном первого**
\`\`\`cpp
// Второе ядро продолжает работать, первое спит
CPower().setMulticoreMode(CORE0_SLEEP_CORE1_ACTIVE);

ChronosPower::WakeupPolicy p;
p.wdt.timeoutMillis = 10000;
p.coreId = 0; // Усыпить только ядро 0
CPower().sleep(p);
\`\`\`

**277. Калибровка внутреннего RC-генератора по внешнему кварцу**
\`\`\`cpp
// Калибровка внутреннего генератора для точности
CPower().calibrateInternalOscillator(ClockSource::EXTERNAL_CRYSTAL_32KHZ);
Serial.println("Внутренний генератор откалиброван");
\`\`\`

**278. Переключение системного тактирования на ходу**
\`\`\`cpp
// Переключение на внутренний RC для экономии
CPower().setSystemClockSource(ClockSource::INTERNAL_RC_SLOW);
Serial.println("Переключено на внутренний RC");

delay(5000);

// Возврат на внешний кварц для точности
CPower().setSystemClockSource(ClockSource::EXTERNAL_CRYSTAL);
Serial.println("Переключено на внешний кварц");
\`\`\`

**279. STM32: Использование LPTIM (Low Power Timer) для сна**
\`\`\`cpp
// Использование низкопотребляющего таймера для пробуждения
ChronosPower::WakeupPolicy p;
p.lowPowerTimer.enabled = true;
p.lowPowerTimer.timeoutSeconds = 30;
p.lowPowerTimer.clockSource = LPTIM_LSI; // Внутренний источник
CPower().sleep(p);
\`\`\`

**280. ESP32-S2/S3: Пробуждение от USB подключения**
\`\`\`cpp
// Пробуждение при подключении USB кабеля
ChronosPower::WakeupPolicy p;
p.usbWakeup.enabled = true;
p.usbWakeup.wakeupOnConnect = true;
p.usbWakeup.wakeupOnDisconnect = false;
CPower().sleep(p);
\`\`\`

**281. Отключение тактирования неиспользуемых RAM банков**
\`\`\`cpp
// Отключить дополнительные банки RAM для экономии
CPower().control(Peripheral::SRAM_BANK_2, false);
CPower().control(Peripheral::SRAM_BANK_3, false);
CPower().control(Peripheral::SRAM_BANK_4, false);
Serial.println("Дополнительные банки RAM отключены");
\`\`\`

**282. ESP32: Активация Light Sleep вместо Deep Sleep при отладке**
\`\`\`cpp
ChronosPower::WakeupPolicy p;
p.rtc.timeoutSeconds = 10;

#ifdef DEBUG
    p.sleepMode = LIGHT_SLEEP; // Сохранить состояние для отладки
    Serial.println("Отладочный режим: Light Sleep");
#else
    p.sleepMode = DEEP_SLEEP; // Максимальная экономия в продакшене
    Serial.println("Продакшен режим: Deep Sleep");
#endif

CPower().sleep(p);
\`\`\`

**283. Отключение FPU перед сном**
\`\`\`cpp
// Отключить блок плавающей точки для экономии
CPower().control(Peripheral::FPU_SINGLE_PRECISION, false);

ChronosPower::WakeupPolicy p;
p.wdt.timeoutMillis = 5000;
CPower().sleep(p);

// Включить обратно после пробуждения
CPower().control(Peripheral::FPU_SINGLE_PRECISION, true);
\`\`\`

**284. Управление питанием PHY Ethernet**
\`\`\`cpp
// Отключить Ethernet PHY когда не используется
CPower().control(Peripheral::ETHERNET_PHY_INTERNAL, false);
Serial.println("Ethernet PHY отключен");

delay(10000);

// Включить обратно
CPower().control(Peripheral::ETHERNET_PHY_INTERNAL, true);
Serial.println("Ethernet PHY включен");
\`\`\`

**285. Сохранение контекста в RTC-память перед гибернацией**
\`\`\`cpp
// Структура для сохранения в RTC памяти
struct RtcData {
    uint32_t bootCount;
    float lastTemperature;
    uint16_t sensorValue;
};

RtcData rtcData;
rtcData.bootCount = CPower().getBootCount() + 1;
rtcData.lastTemperature = 25.5f;
rtcData.sensorValue = analogRead(A0);

// Сохранить в RTC память
CPower().saveToRtcMemory(&rtcData, sizeof(rtcData));

ChronosPower::WakeupPolicy p;
p.rtc.timeoutSeconds = 3600;
CPower().hibernate(p);
\`\`\`

**286. Восстановление контекста из RTC-памяти после пробуждения**
\`\`\`cpp
struct RtcData {
    uint32_t bootCount;
    float lastTemperature;
    uint16_t sensorValue;
};

RtcData rtcData;

// Восстановить из RTC памяти
if (CPower().restoreFromRtcMemory(&rtcData, sizeof(rtcData)) == Status::OK) {
    Serial.println("Загрузка #" + String(rtcData.bootCount));
    Serial.println("Последняя температура: " + String(rtcData.lastTemperature));
    Serial.println("Последнее значение сенсора: " + String(rtcData.sensorValue));
} else {
    Serial.println("Первая загрузка или ошибка RTC памяти");
}
\`\`\`

**287. ESP32: Использование ULP для мониторинга сенсоров во время сна**
\`\`\`cpp
// Программа ULP для чтения АЦП во время сна основного процессора
const char* ulpProgram = R"(
    read_adc:
        adc r0, 0, adc_channel + 1
        move r1, threshold
        sub r0, r0, r1
        jump wake_up, ov
        halt
    wake_up:
        wake
        halt
)";

CPower().loadUlpProgram(ulpProgram);
CPower().setUlpVariable("threshold", 2048); // Порог пробуждения

ChronosPower::WakeupPolicy p;
p.enableUlpWakeup = true;
p.rtc.timeoutSeconds = 300; // Резервный таймер
CPower().sleep(p);
\`\`\`

**288. AVR: Использование асинхронного таймера для точного времени**
\`\`\`cpp
// Настройка Timer2 как асинхронного с внешним кварцем 32.768 кГц
CPower().configureAsyncTimer(TIMER2, CRYSTAL_32KHZ);

ChronosPower::WakeupPolicy p;
p.asyncTimer.enabled = true;
p.asyncTimer.timerIndex = 2;
p.asyncTimer.timeoutSeconds = 60; // Точно 1 минута
CPower().sleep(p);
\`\`\`

**289. STM32: Использование независимого watchdog во время сна**
\`\`\`cpp
// Настройка независимого watchdog для безопасности
CPower().configureIndependentWatchdog(32000); // 32 секунды

ChronosPower::WakeupPolicy p;
p.rtc.timeoutSeconds = 30; // Проснуться до срабатывания watchdog
p.independentWdt.enabled = true;
CPower().sleep(p);

// Сбросить watchdog после пробуждения
CPower().resetIndependentWatchdog();
\`\`\`

**290. RP2040: Использование PIO для пробуждения по протоколу**
\`\`\`cpp
// Настройка PIO для декодирования протокола во время сна
const char* pioProgram = R"(
    .program uart_rx_wake
    wait 0 pin 0    ; Wait for start bit
    set pins, 1     ; Signal wake up
    irq 0           ; Trigger interrupt
)";

CPower().loadPioProgram(0, pioProgram);

ChronosPower::WakeupPolicy p;
p.pioWakeup.enabled = true;
p.pioWakeup.stateMachine = 0;
p.pioWakeup.pin = 0;
CPower().sleep(p);
\`\`\`

**291. ESP32: Настройка различных доменов питания**
\`\`\`cpp
// Тонкая настройка доменов питания ESP32
CPower().configurePowerDomain(POWER_DOMAIN_RTC_PERIPH, POWER_DOWN);
CPower().configurePowerDomain(POWER_DOMAIN_RTC_SLOW_MEM, POWER_ON);
CPower().configurePowerDomain(POWER_DOMAIN_RTC_FAST_MEM, POWER_DOWN);
CPower().configurePowerDomain(POWER_DOMAIN_XTAL, POWER_DOWN);

ChronosPower::WakeupPolicy p;
p.rtc.timeoutSeconds = 60;
CPower().sleep(p);
\`\`\`

**292. AVR: Использование внешнего прерывания с фильтрацией**
\`\`\`cpp
// Настройка внешнего прерывания с аппаратной фильтрацией шума
ChronosPower::WakeupPolicy p;
p.extInterrupt.pinMask = (1ULL << 2);
p.extInterrupt.triggerMode = FALLING;
p.extInterrupt.enableNoiseFilter = true;
p.extInterrupt.filterClockDivider = 64; // Фильтр на основе системного такта
CPower().sleep(p);
\`\`\`

**293. STM32: Использование RTC Tamper для безопасности**
\`\`\`cpp
// Настройка Tamper пина для обнаружения вскрытия
ChronosPower::WakeupPolicy p;
p.rtcTamper.enabled = true;
p.rtcTamper.pin = RTC_TAMPER_1;
p.rtcTamper.triggerMode = TAMPER_FALLING_EDGE;
p.rtcTamper.enablePullup = true;
p.rtc.timeoutSeconds = 3600; // Обычное пробуждение через час

CPower().registerTamperCallback([]() {
    Serial.println("ВНИМАНИЕ: Обнаружено вскрытие корпуса!");
    // Стереть критические данные
    eraseSensitiveData();
});

CPower().sleep(p);
\`\`\`

**294. ESP32: Использование температурного сенсора для пробуждения**
\`\`\`cpp
// Пробуждение при превышении температуры
ChronosPower::WakeupPolicy p;
p.temperatureWakeup.enabled = true;
p.temperatureWakeup.threshold = 60.0f; // 60°C
p.temperatureWakeup.triggerMode = TEMP_ABOVE;
p.temperatureWakeup.hysteresis = 2.0f; // Гистерезис 2°C
p.rtc.timeoutSeconds = 300; // Резервный таймер

CPower().sleep(p);

if (CPower().getWakeupReason() == WakeupReason::THERMAL_WARNING_HIGH) {
    Serial.println("Пробуждение от перегрева!");
    CPower().optimizeForThermal();
}
\`\`\`

**295. RP2040: Использование ROSC для низкопотребляющего тактирования**
\`\`\`cpp
// Переключение на кольцевой генератор для минимального потребления
CPower().setSystemClockSource(ClockSource::RING_OSCILLATOR);
CPower().setRingOscillatorFrequency(ROSC_1MHZ); // 1 МГц

ChronosPower::WakeupPolicy p;
p.wdt.timeoutMillis = 10000;
CPower().sleep(p);

// Возврат на нормальное тактирование
CPower().setSystemClockSource(ClockSource::CRYSTAL_OSCILLATOR);
\`\`\`

**296. STM32: Ис  ользование LSE для RTC в режиме Standby**
\`\`\`cpp
// Настройка внешнего кварца 32.768 кГц для RTC
CPower().configureLowSpeedExternalClock(LSE_32768HZ);
CPower().setRtcClockSource(RTC_CLOCK_LSE);

ChronosPower::WakeupPolicy p;
p.rtc.timeoutSeconds = 86400; // 24 часа
p.rtc.clockSource = RTC_LSE;
p.enableStandbyMode = true;

CPower().hibernate(p); // Полная гибернация с RTC
\`\`\`

**297. ESP32: Использование ADC для пробуждения по аналоговому сигналу**
\`\`\`cpp
// Настройка АЦП для мониторинга во время сна
ChronosPower::WakeupPolicy p;
p.adcWakeup.enabled = true;
p.adcWakeup.channel = ADC1_CHANNEL_0; // GPIO36
p.adcWakeup.threshold = 2048; // Середина диапазона 12-bit АЦП
p.adcWakeup.triggerMode = ADC_ABOVE_THRESHOLD;
p.adcWakeup.sampleRate = ADC_SAMPLE_1HZ; // 1 измерение в секунду

CPower().sleep(p);

if (CPower().getWakeupReason() == WakeupReason::ADC_THRESHOLD_EXCEEDED) {
    int adcValue = CPower().getWakeupAdcValue();
    Serial.println("Пробуждение от АЦП, значение: " + String(adcValue));
}
\`\`\`

**298. AVR: Использование Analog Comparator для пробуждения**
\`\`\`cpp
// Настройка аналогового компаратора для пробуждения
CPower().control(Peripheral::ANALOG_COMPARATOR_0, true);

ChronosPower::WakeupPolicy p;
p.analogComparator.enabled = true;
p.analogComparator.positiveInput = AIN0; // Внешний вход
p.analogComparator.negativeInput = BANDGAP_1V1; // Внутренний источник 1.1В
p.analogComparator.triggerMode = COMP_RISING_EDGE;

CPower().sleep(p);

if (CPower().getWakeupReason() == WakeupReason::ANALOG_COMPARATOR_0) {
    Serial.println("Напряжение превысило 1.1В");
}
\`\`\`

**299. STM32: Использование DMA для сбора данных во время сна**
\`\`\`cpp
// Настройка DMA для автономного сбора данных от АЦП
uint16_t adcBuffer[100];

CPower().configureDmaCircular(DMA1_CHANNEL1, ADC1, adcBuffer, 100);
CPower().startAdcDmaConversion();

ChronosPower::WakeupPolicy p;
p.dmaWakeup.enabled = true;
p.dmaWakeup.channel = DMA1_CHANNEL1;
p.dmaWakeup.wakeupOnComplete = true;
p.rtc.timeoutSeconds = 60; // Резервный таймер

CPower().sleep(p);

if (CPower().getWakeupReason() == WakeupReason::DMA_TRANSFER_COMPLETE) {
    Serial.println("DMA собрал 100 образцов АЦП");
    // Обработать данные в adcBuffer
    processAdcData(adcBuffer, 100);
}
\`\`\`

**300. ESP32: Использование Hall сенсора для пробуждения**
\`\`\`cpp
// Пробуждение от встроенного датчика Холла
ChronosPower::WakeupPolicy p;
p.hallSensor.enabled = true;
p.hallSensor.threshold = 100; // Пороговое значение
p.hallSensor.triggerMode = HALL_FIELD_DETECTED;
p.rtc.timeoutSeconds = 3600;

CPower().sleep(p);

if (CPower().getWakeupReason() == WakeupReason::HALL_SENSOR_TRIGGERED) {
    int hallValue = CPower().getHallSensorValue();
    Serial.println("Обнаружено магнитное поле: " + String(hallValue));
}
\`\`\`

**301. Универсальная платформо-адаптивная функция сна**
\`\`\`cpp
void platformAdaptiveSleep(uint32_t durationMs) {
    ChronosPower::WakeupPolicy p;
    
    #if defined(ESP32)
        // ESP32 - используем RTC таймер для точности
        p.rtc.timeoutSeconds = durationMs / 1000;
        if (durationMs % 1000 > 0) p.rtc.timeoutSeconds++;
        
        // Настройка доменов питания для максимальной экономии
        p.powerDomains.rtcPeriph = POWER_DOWN;
        p.powerDomains.rtcSlowMem = POWER_ON; // Для сохранения переменных
        p.powerDomains.rtcFastMem = POWER_DOWN;
        
    #elif defined(__AVR__)
        // AVR - используем WDT с циклами для длительного сна
        if (durationMs <= 8000) {
            p.wdt.timeoutMillis = durationMs;
        } else {
            // Для длительного сна используем циклы
            uint32_t cycles = durationMs / 8000;
            uint32_t remainder = durationMs % 8000;
            
            p.wdt.timeoutMillis = 8000;
            for (uint32_t i = 0; i < cycles; i++) {
                CPower().sleep(p);
            }
            
            if (remainder > 0) {
                p.wdt.timeoutMillis = remainder;
            } else {
                return; // Уже проспали нужное время
            }
        }
        
    #elif defined(ARDUINO_ARCH_RP2040)
        // RP2040 - используем встроенный RTC
        p.rtc.timeoutSeconds = durationMs / 1000;
        if (durationMs % 1000 > 0) p.rtc.timeoutSeconds++;
        
        // Настройка тактирования для экономии
        p.clockConfig.switchToRingOsc = true;
        p.clockConfig.ringOscFreq = ROSC_1MHZ;
        
    #elif defined(ARDUINO_ARCH_STM32)
        // STM32 - используем RTC с LSI
        p.rtc.timeoutSeconds = durationMs / 1000;
        if (durationMs % 1000 > 0) p.rtc.timeoutSeconds++;
        
        p.rtc.clockSource = RTC_LSI;
        p.enableStopMode = true; // STOP режим для STM32
        
    #else
        // Универсальная реализация
        p.wdt.timeoutMillis = min(durationMs, 8000UL);
        
    #endif
    
    CPower().sleep(p);
    
    Serial.println("Платформо-адаптивный сон завершен");
    Serial.println("Платформа: " + CPower().getPlatformName());
    Serial.println("Причина пробуждения: " + String(CPower().wakeupReasonToString(CPower().getWakeupReason())));
}
\`\`\`

#### **Раздел 10: Интеллектуальные Возможности v2.5 (Примеры 302-400)**

**302. Настройка адаптивного масштабирования**
\`\`\`cpp
ChronosPower::AdaptiveScalingConfig config;
config.enabled = true;
config.samplingIntervalMs = 500; // Проверять каждые 500мс
config.performanceThresholdPercent = 75;
config.powerThresholdPercent = 25;
config.temperatureThresholdCelsius = 65.0f;
config.enableWorkloadPrediction = true;
config.enableEnergyBudgeting = true;

CPower().enableAdaptiveScaling(config);
Serial.println("Адаптивное масштабирование включено");
\`\`\`

**303. Мониторинг энергопотребления в реальном времени**
\`\`\`cpp
CPower().registerEnergyCallback([](const EnergyMetrics& metrics) {
    Serial.println("=== Энергетические метрики ===");
    Serial.println("Текущее потребление: " + String(metrics.currentConsumptionMicroAmps) + " мкА");
    Serial.println("Среднее потребление: " + String(metrics.averageConsumptionMicroAmps) + " мкА");
    Serial.println("Пиковое потребление: " + String(metrics.peakConsumptionMicroAmps) + " мкА");
    Serial.println("Уровень батареи: " + String(metrics.batteryPercentage) + "%");
    Serial.println("Напряжение батареи: " + String(metrics.batteryVoltage) + " мВ");
    Serial.println("Потреблено энергии: " + String(metrics.energyConsumedMicroWattHours) + " мкВт·ч");
    Serial.println("Эффективность питания: " + String(metrics.powerEfficiencyPercent) + "%");
    
    // Автоматическая оптимизация при низком заряде
    if (metrics.batteryPercentage < 20) {
        Serial.println("Низкий заряд батареи - активация режима экономии");
        CPower().optimizeForBatteryLife();
    } else if (metrics.batteryPercentage < 10) {
        Serial.println("КРИТИЧЕСКИ низкий заряд - аварийный режим");
        CPower().setProfile(PowerProfile::EMERGENCY_POWER);
    }
});
\`\`\`

**304. Тепловой мониторинг и автоматическое управление**
\`\`\`cpp
CPower().registerThermalCallback([](const ThermalMetrics& thermal) {
    Serial.println("=== Тепловые метрики ===");
    Serial.println("Текущая температура: " + String(thermal.currentTemperatureCelsius) + "°C");
    Serial.println("Максимальная температура: " + String(thermal.maxTemperatureCelsius) + "°C");
    Serial.println("Средняя температура: " + String(thermal.averageTemperatureCelsius) + "°C");
    Serial.println("Тепловая зона: " + String(thermal.thermalZone));
    Serial.println("Тепловой троттлинг: " + String(thermal.thermalThrottlingActive ? "Активен" : "Неактивен"));
    Serial.println("Уровень троттлинга: " + String(thermal.thermalThrottlingLevel));
    Serial.println("Тепловых циклов: " + String(thermal.thermalCycleCount));
    
    // Автоматическое тепловое управление
    if (thermal.currentTemperatureCelsius > 80.0f) {
        Serial.println("КРИТИЧЕСКАЯ температура - аварийное охлаждение");
        CPower().setProfile(PowerProfile::EMERGENCY_POWER);
        CPower().setCpuFrequency(CPower().getMinCpuFrequency());
    } else if (thermal.currentTemperatureCelsius > 70.0f) {
        Serial.println("Высокая температура - активация теплового управления");
        CPower().optimizeForThermal();
    } else if (thermal.thermalThrottlingActive && thermal.currentTemperatureCelsius < 60.0f) {
        Serial.println("Температура нормализовалась - отключение троттлинга");
        CPower().setProfile(PowerProfile::BALANCED_MOBILE);
    }
});
\`\`\`

**305. Создание и использование групп периферии**
\`\`\`cpp
// Создание группы сенсоров
std::vector<Peripheral> sensorPeripherals = {
    Peripheral::ADC_0_12BIT,
    Peripheral::ADC_1_12BIT,
    Peripheral::I2C_0,
    Peripheral::INTERNAL_TEMP_SENSOR,
    Peripheral::ANALOG_COMPARATOR_0
};
CPower().createPeripheralGroup("sensors", sensorPeripherals);

// Создание группы коммуникации
std::vector<Peripheral> commPeripherals = {
    Peripheral::UART_0,
    Peripheral::UART_1,
    Peripheral::SPI_0,
    Peripheral::I2C_1,
    Peripheral::WIFI_MAC_2_4GHZ,
    Peripheral::BLUETOOTH_CONTROLLER
};
CPower().createPeripheralGroup("communication", commPeripherals);

// Создание группы мультимедиа
std::vector<Peripheral> mediaPeripherals = {
    Peripheral::CAMERA_INTERFACE_DCMI,
    Peripheral::JPEG_CODEC,
    Peripheral::LCD_CONTROLLER_SPI,
    Peripheral::AUDIO_CODEC_I2S
};
CPower().createPeripheralGroup("multimedia", mediaPeripherals);

Serial.println("Группы периферии созданы");

// Использование групп для различных режимов работы
void enterSensorMode() {
    CPower().disablePeripheralGroup("communication");
    CPower().disablePeripheralGroup("multimedia");
    CPower().enablePeripheralGroup("sensors");
    Serial.println("Режим сенсоров активирован");
}

void enterCommunicationMode() {
    CPower().disablePeripheralGroup("sensors");
    CPower().disablePeripheralGroup("multimedia");
    CPower().enablePeripheralGroup("communication");
    Serial.println("Режим коммуникации активирован");
}

void enterMultimediaMode() {
    CPower().disablePeripheralGroup("sensors");
    CPower().enablePeripheralGroup("communication"); // Нужна для передачи данных
    CPower().enablePeripheralGroup("multimedia");
    Serial.println("Мультимедийный режим активирован");
}
\`\`\`

**306. Сохранение и загрузка конфигураций**
\`\`\`cpp
void setupPowerConfigurations() {
    Serial.println("Настройка конфигураций питания...");
    
    // Конфигурация высокой производительности
    CPower()
        .withProfile(PowerProfile::HIGH_PERFORMANCE)
        .withFrequency(240000000)
        .withPeripheral(Peripheral::CPU_CORE_0, true)
        .withPeripheral(Peripheral::CPU_CORE_1, true) // Многоядерность
        .withPeripheral(Peripheral::FPU_SINGLE_PRECISION, true)
        .withPeripheral(Peripheral::CACHE_CONTROLLER, true)
        .withEnergyBudget(2.0) // 2 Вт·ч в день
        .withThermalLimit(85.0)
        .apply();
    CPower().saveConfiguration("high_performance");
    Serial.println("✓ Конфигурация высокой производительности сохранена");
    
    // Конфигурация сбалансированного режима
    CPower()
        .withProfile(PowerProfile::BALANCED_MOBILE)
        .withFrequency(80000000)
        .withPeripheral(Peripheral::UART_0, true)
        .withPeripheral(Peripheral::I2C_0, true)
        .withPeripheral(Peripheral::SPI_0, true)
        .withPeripheral(Peripheral::ADC_0_12BIT, true)
        .withEnergyBudget(0.5) // 0.5 Вт·ч в день
        .withThermalLimit(70.0)
        .withAdaptiveScaling(true)
        .apply();
    CPower().saveConfiguration("balanced");
    Serial.println("✓ Сбалансированная конфигурация сохранена");
    
    // Конфигурация ультра-низкого энергопотребления
    CPower()
        .withProfile(PowerProfile::ULTRA_LOW_POWER)
        .withFrequency(10000000)
        .withPeripheral(Peripheral::UART_0, true) // Только для отладки
        .withPeripheral(Peripheral::RTC_INTERNAL, true)
        .withPeripheral(Peripheral::WATCHDOG_SYSTEM, true)
        .withEnergyBudget(0.05) // 50 мВт·ч в день
        .withThermalLimit(60.0)
        .withAdaptiveScaling(true)
        .apply();
    CPower().saveConfiguration("ultra_low_power");
    Serial.println("✓ Конфигурация ультра-низкого энергопотребления сохранена");
    
    // Конфигурация IoT сенсора
    CPower()
        .withProfile(PowerProfile::ADAPTIVE_BATTERY_LIFE)
        .withFrequency(20000000)
        .withPeripheral(Peripheral::ADC_0_12BIT, true)
        .withPeripheral(Peripheral::I2C_0, true)
        .withPeripheral(Peripheral::RTC_INTERNAL, true)
        .withPeripheral(Peripheral::WIFI_MAC_2_4GHZ, true) // Для передачи данных
        .withEnergyBudget(0.1) // 100 мВт·ч в день
        .withThermalLimit(65.0)
        .withAdaptiveScaling(true)
        .apply();
    CPower().saveConfiguration("iot_sensor");
    Serial.println("✓ Конфигурация IoT сенсора сохранена");
    
    // Аварийная конфигурация
    CPower()
        .withProfile(PowerProfile::EMERGENCY_POWER)
        .withFrequency(1000000) // Минимальная частота
        .withPeripheral(Peripheral::SYSTEM_CONTROLLER, true)
        .withPeripheral(Peripheral::CPU_CORE_0, true)
        .withPeripheral(Peripheral::SRAM_BANK_0, true)
        .withPeripheral(Peripheral::WATCHDOG_SYSTEM, true)
        .withEnergyBudget(0.01) // 10 мВт  ч в день
        .withThermalLimit(50.0)
        .apply();
    CPower().saveConfiguration("emergency");
    Serial.println("✓ Аварийная конфигурация сохранена");
    
    Serial.println("Все конфигурации настроены и сохранены");
}

void switchConfiguration(const String& configName) {
    Serial.println("Переключение на конфигурацию: " + configName);
    
    Status result = CPower().loadConfiguration(configName);
    if (result == Status::OK) {
        Serial.println("✓ Конфигурация '" + configName + "' загружена успешно");
        
        // Показать текущие параметры
        Serial.println("Частота CPU: " + String(CPower().getCpuFrequency()) + " Гц");
        
        EnergyMetrics energy = CPower().getEnergyMetrics();
        Serial.println("Текущее потребление: " + String(energy.currentConsumptionMicroAmps) + " мкА");
        
    } else {
        Serial.println("✗ Ошибка загрузки конфигурации: " + String((int)result));
    }
}

// Автоматическое переключение конфигураций
void autoSwitchConfiguration() {
    EnergyMetrics energy = CPower().getEnergyMetrics();
    ThermalMetrics thermal = CPower().getThermalMetrics();
    
    if (energy.batteryPercentage < 5) {
        switchConfiguration("emergency");
    } else if (energy.batteryPercentage < 20) {
        switchConfiguration("ultra_low_power");
    } else if (thermal.currentTemperatureCelsius > 75) {
        switchConfiguration("ultra_low_power"); // Снизить нагрев
    } else if (needHighPerformance()) {
        switchConfiguration("high_performance");
    } else {
        switchConfiguration("balanced");
    }
}
\`\`\`

**307. Энергетическое бюджетирование с предиктивным масштабированием**
\`\`\`cpp
void setupEnergyBudgeting() {
    Serial.println("Настройка энергетического бюджетирования...");
    
    ChronosPower::EnergyBudget budget;
    budget.dailyBudgetMicroWattHours = 500000.0f;  // 0.5 Вт·ч в день
    budget.hourlyBudgetMicroWattHours = budget.dailyBudgetMicroWattHours / 24.0f;
    budget.currentConsumptionLimit = 5000.0f; // Лимит 5мА
    budget.enableStrictMode = false; // Разрешить временные превышения
    budget.enablePredictiveScaling = true; // Предиктивные алгоритмы
    budget.enableEnergyHarvesting = false; // Пока без сбора энергии
    budget.batteryCapacityMah = 2000.0f; // Емкость батареи 2000 мАч
    budget.batteryVoltage = 3.7f; // Номинальное напряжение 3.7В
    budget.enableBatteryProtection = true; // Защита батареи
    budget.lowBatteryThreshold = 20.0f; // Порог низкого заряда 20%
    budget.criticalBatteryThreshold = 5.0f; // Критический порог 5%
    
    Status result = CPower().setEnergyBudget(budget);
    if (result == Status::OK) {
        Serial.println("✓ Энергетический бюджет установлен");
        Serial.println("Дневной бюджет: " + String(budget.dailyBudgetMicroWattHours / 1000.0f) + " мВт·ч");
        Serial.println("Часовой бюджет: " + String(budget.hourlyBudgetMicroWattHours / 1000.0f) + " мВт·ч");
        Serial.println("Лимит тока: " + String(budget.currentConsumptionLimit / 1000.0f) + " мА");
    } else {
        Serial.println("✗ Ошибка установки энергетического бюджета: " + String((int)result));
    }
    
    // Регистрация callback для мониторинга бюджета
    CPower().registerEnergyCallback([](const EnergyMetrics& metrics) {
        static unsigned long lastBudgetCheck = 0;
        unsigned long now = millis();
        
        // Проверять бюджет каждые 10 секунд
        if (now - lastBudgetCheck > 10000) {
            lastBudgetCheck = now;
            
            float dailyBudget = 500.0f; // 0.5 Вт·ч в мВт·ч
            float consumedEnergy = metrics.energyConsumedMicroWattHours / 1000.0f; // в мВт·ч
            float remainingBudget = dailyBudget - consumedEnergy;
            float budgetUsedPercent = (consumedEnergy / dailyBudget) * 100.0f;
            
            Serial.println("=== Статус энергетического бюджета ===");
            Serial.println("Потреблено: " + String(consumedEnergy) + " мВт·ч (" + String(budgetUsedPercent) + "%)");
            Serial.println("Остается: " + String(remainingBudget) + " мВт·ч");
            
            // Предиктивное масштабирование
            if (budgetUsedPercent > 80.0f) {
                Serial.println("⚠ Превышение бюджета - переход в режим экономии");
                CPower().setProfile(PowerProfile::ULTRA_LOW_POWER);
            } else if (budgetUsedPercent > 60.0f) {
                Serial.println("⚠ Высокое потребление - снижение производительности");
                CPower().setProfile(PowerProfile::LOW_POWER_ACTIVE);
            } else if (budgetUsedPercent < 30.0f) {
                Serial.println("✓ Низкое потребление - можно увеличить производительность");
                CPower().setProfile(PowerProfile::BALANCED_MOBILE);
            }
            
            // Прогноз времени работы
            if (metrics.currentConsumptionMicroAmps > 0) {
                float currentMa = metrics.currentConsumptionMicroAmps / 1000.0f;
                float batteryCapacity = 2000.0f; // мАч
                float estimatedHours = (batteryCapacity * metrics.batteryPercentage / 100.0f) / currentMa;
                
                Serial.println("Прогноз времени работы: " + String(estimatedHours) + " часов");
                
                if (estimatedHours < 2.0f) {
                    Serial.println("⚠ ВНИМАНИЕ: Менее 2 часов работы!");
                }
            }
        }
    });
}
\`\`\`

**308. Получение всех метрик системы**
\`\`\`cpp
void displaySystemMetrics() {
    Serial.println("\n=== ПОЛНЫЕ МЕТРИКИ СИСТЕМЫ ===");
    
    // Энергетические метрики
    EnergyMetrics energy = CPower().getEnergyMetrics();
    Serial.println("--- Энергетические метрики ---");
    Serial.println("Текущий ток: " + String(energy.currentConsumptionMicroAmps) + " мкА");
    Serial.println("Средний ток: " + String(energy.averageConsumptionMicroAmps) + " мкА");
    Serial.println("Пиковый ток: " + String(energy.peakConsumptionMicroAmps) + " мкА");
    Serial.println("Напряжение батареи: " + String(energy.batteryVoltage) + " мВ");
    Serial.println("Уровень батареи: " + String(energy.batteryPercentage) + "%");
    Serial.println("Потреблено энергии: " + String(energy.energyConsumedMicroWattHours / 1000.0f) + " мВт·ч");
    Serial.println("Время работы: " + String(energy.uptimeSeconds) + " сек");
    Serial.println("Время сна: " + String(energy.sleepTimeSeconds) + " сек");
    Serial.println("Эффективность питания: " + String(energy.powerEfficiencyPercent) + "%");
    Serial.println("Эффективность энергии: " + String(energy.energyEfficiencyPercent) + "%");
    Serial.println("Количество пробуждений: " + String(energy.wakeupCount));
    
    // Тепловые метрики
    ThermalMetrics thermal = CPower().getThermalMetrics();
    Serial.println("--- Тепловые метрики ---");
    Serial.println("Текущая температура: " + String(thermal.currentTemperatureCelsius) + "°C");
    Serial.println("Максимальная температура: " + String(thermal.maxTemperatureCelsius) + "°C");
    Serial.println("Средняя температура: " + String(thermal.averageTemperatureCelsius) + "°C");
    Serial.println("Тепловая зона: " + String(thermal.thermalZone));
    Serial.println("Тепловой троттлинг: " + String(thermal.thermalThrottlingActive ? "Активен" : "Неактивен"));
    Serial.println("Уровень троттлинга: " + String(thermal.thermalThrottlingLevel));
    Serial.println("Тепловое напряжение: " + String(thermal.thermalStress));
    Serial.println("Рассеивание тепла: " + String(thermal.heatDissipationWatts) + " Вт");
    Serial.println("Тепловых циклов: " + String(thermal.thermalCycleCount));
    
    // Метрики производительности
    PerformanceMetrics perf = CPower().getPerformanceMetrics();
    Serial.println("--- Метрики производительности ---");
    Serial.println("Использование CPU: " + String(perf.cpuUtilizationPercent) + "%");
    Serial.println("Использование памяти: " + String(perf.memoryUtilizationPercent) + "%");
    Serial.println("Попадания в кэш: " + String(perf.cacheHitRatePercent) + "%");
    Serial.println("Прерываний/сек: " + String(perf.interruptsPerSecond));
    Serial.println("Переключений контекста/сек: " + String(perf.contextSwitchesPerSecond));
    Serial.println("Инструкций/сек: " + String(perf.instructionsPerSecond));
    Serial.println("Рейтинг MIPS: " + String(perf.mipsRating));
    Serial.println("Рейтинг FLOPS: " + String(perf.flopsRating));
    
    // Системная информация
    Serial.println("--- Системная информация ---");
    Serial.println("Частота CPU: " + String(CPower().getCpuFrequency()) + " Гц");
    Serial.println("Текущий профиль: " + String(CPower().profileToString(CPower().getProfile())));
    Serial.println("Платформа: " + CPower().getPlatformName());
    Serial.println("Версия библиотеки: " + CPower().getLibraryVersion());
    Serial.println("Причина последнего пробуждения: " + String(CPower().wakeupReasonToString(CPower().getWakeupReason())));
    Serial.println("Причина последнего сброса: " + String(CPower().resetReasonToString(CPower().getResetReason())));
    
    // Расчет эффективности системы
    float totalTime = energy.uptimeSeconds + energy.sleepTimeSeconds;
    float sleepEfficiency = (totalTime > 0) ? (energy.sleepTimeSeconds / totalTime * 100.0f) : 0.0f;
    Serial.println("Эффективность сна: " + String(sleepEfficiency) + "%");
    
    // Прогноз времени работы
    if (energy.currentConsumptionMicroAmps > 0) {
        float batteryCapacityMah = 2000.0f; // Предполагаемая емкость
        float currentMa = energy.currentConsumptionMicroAmps / 1000.0f;
        float remainingCapacity = batteryCapacityMah * energy.batteryPercentage / 100.0f;
        float estimatedHours = remainingCapacity / currentMa;
        
        Serial.println("Прогноз времени работы: " + String(estimatedHours) + " часов");
    }
    
    Serial.println("=== КОНЕЦ МЕТРИК ===\n");
}
\`\`\`

**309. Автоматическая оптимизация**
\`\`\`cpp
void performAutomaticOptimization() {
    Serial.println("Запуск автоматической оптимизации...");
    
    // Получить текущие метрики
    EnergyMetrics energy = CPower().getEnergyMetrics();
    ThermalMetrics thermal = CPower().getThermalMetrics();
    PerformanceMetrics perf = CPower().getPerformanceMetrics();
    
    Serial.println("Анализ текущего состояния системы...");
    Serial.println("Батарея: " + String(energy.batteryPercentage) + "%");
    Serial.println("Температура: " + String(thermal.currentTemperatureCelsius) + "°C");
    Serial.println("Использование CPU: " + String(perf.cpuUtilizationPercent) + "%");
    
    // Определить приоритет оптимизации
    bool batteryLow = energy.batteryPercentage < 30;
    bool temperatureHigh = thermal.currentTemperatureCelsius > 70;
    bool cpuOverloaded = perf.cpuUtilizationPercent > 80;
    
    if (batteryLow && temperatureHigh) {
        Serial.println("Критическая ситуация: низкая батарея И высокая температура");
        Serial.println("Применение аварийной оптимизации...");
        
        // Аварийная оптимизация
        CPower().setProfile(PowerProfile::EMERGENCY_POWER);
        CPower().setCpuFrequency(CPower().getMinCpuFrequency());
        
        // Отключить все несущественные периферийные устройства
        CPower().setClockGatingForDomain(PowerDomain::ALL, false);
        CPower().control(Peripheral::SYSTEM_CONTROLLER, true);
        CPower().control(Peripheral::CPU_CORE_0, true);
        CPower().control(Peripheral::SRAM_BANK_0, true);
        CPower().control(Peripheral::UART_0, true); // Для отладки
        
    } else if (batteryLow) {
        Serial.println("Низкий заряд батареи - оптимизация для максимального времени работы");
        CPower().optimizeForBatteryLife();
        
        // Дополнительные меры экономии
        CPower().setClockGatingForDomain(PowerDomain::MULTIMEDIA, false);
        CPower().setClockGatingForDomain(PowerDomain::SECURITY_MODULES, false);
        
    } else if (temperatureHigh) {
        Serial.println("Высокая температура - тепловая оптимизация");
        CPower().optimizeForThermal();
        
        // Снизить частоту для уменьшения тепловыделения
        uint32_t currentFreq = CPower().getCpuFrequency();
        CPower().setCpuFrequency(currentFreq * 0.8f);
        
    } else if (cpuOverloaded) {
        Serial.println("Высокая нагрузка CPU - оптимизация производительности");
        CPower().optimizeForPerformance();
        
        // Включить дополнительные ядра если доступны
        CPower().control(Peripheral::CPU_CORE_1, true);
        CPower().control(Peripheral::FPU_SINGLE_PRECISION, true);
        CPower().control(Peripheral::CACHE_CONTROLLER, true);
        
    } else {
        Serial.println("Система в нормальном состоянии - сбалансированная оптимизация");
        CPower().setProfile(PowerProfile::ADAPTIVE_BATTERY_LIFE);
        
        // Включить адаптивное масштабирование
        AdaptiveScalingConfig adaptiveConfig;
        adaptiveConfig.enabled = true;
        adaptiveConfig.enableWorkloadPrediction = true;
        adaptiveConfig.enableEnergyBudgeting = true;
        adaptiveConfig.enableThermalManagement = true;
        CPower().enableAdaptiveScaling(adaptiveConfig);
    }
    
    // Подождать применения изменений
    delay(1000);
    
    // Проверить результаты оптимизации
    EnergyMetrics newEnergy = CPower().getEnergyMetrics();
    ThermalMetrics newThermal = CPower().getThermalMetrics();
    
    Serial.println("Результаты оптимизации:");
    Serial.println("Потребление: " + String(energy.currentConsumptionMicroAmps) + " → " + 
                  String(newEnergy.currentConsumptionMicroAmps) + " мкА");
    Serial.println("Температура: " + String(thermal.currentTemperatureCelsius) + " → " + 
                  String(newThermal.currentTemperatureCelsius) + "°C");
    Serial.println("Частота CPU: " + String(CPower().getCpuFrequency()) + " Гц");
    
    float powerSavings = ((float)(energy.currentConsumptionMicroAmps - newEnergy.currentConsumptionMicroAmps) / 
                         energy.currentConsumptionMicroAmps) * 100.0f;
    if (powerSavings > 0) {
        Serial.println("✓ Экономия энергии: " + String(powerSavings) + "%");
    } else if (powerSavings < 0) {
        Serial.println("⚠ Увеличение потребления: " + String(-powerSavings) + "% (для производительности)");
    }
    
    Serial.println("Автоматическая оптимизация завершена");
}
\`\`\`

**310. Интеллектуальное пробуждение с множественными источниками**
\`\`\`cpp
void setupIntelligentWakeup() {
    Serial.println("Настройка интеллектуального пробуждения...");
    
    ChronosPower::WakeupPolicy policy;
    
    // Основные источники пробуждения
    policy.rtc.timeoutSeconds = 300;           // Каждые 5 минут для проверки
    policy.extInterrupt.pinMask = (1ULL << 2); // Кнопка на пине 2
    policy.extInterrupt.triggerMode = FALLING;
    policy.extInterrupt.debounceMicros = 50000; // Подавление дребезга 50мс
    policy.touchThreshold = 40;                // Сенсор касания (ESP32)
    policy.enableWifiWakeup = true;            // WiFi пакеты
    policy.enableBluetoothWakeup = true;       // Bluetooth соединения
    
    // Интеллектуальные возможности
    policy.enableIntelligentWakeup = true;     // Включить ИИ оптимизацию
    policy.maxSleepDurationMs = 600000;        // Максимум 10 минут сна
    policy.enableWakeupLogging = true;         // Логирование пробуждений
    policy.enableWakeupPrediction = true;      // Предсказание оптимального времени
    policy.enableAdaptiveWakeup = true;        // Адаптация к паттернам использования
    
    // Условные источники пробуждения
    policy.temperatureWakeup.enabled = true;
    policy.temperatureWakeup.threshold = 35.0f; // Пробуждение при 35°C
    policy.temperatureWakeup.triggerMode = TEMP_ABOVE;
    
    policy.motionWakeup.enabled = true;
    policy.motionWakeup.threshold = 100; // Порог движения в mg
    policy.motionWakeup.duration = 1000; // Минимальная длительность движения 1сек
    
    policy.lightWakeup.enabled = true;
    policy.lightWakeup.luxThreshold = 50; // Пробуждение при освещении >50 люкс
    policy.lightWakeup.triggerMode = LIGHT_INCREASE;
    
    // Тепловая защита во время сна
    policy.thermalPolicy.enabled = true;
    policy.thermalPolicy.warningTempCelsius = 65.0f;
    policy.thermalPolicy.shutdownTempCelsius = 80.0f;
    policy.thermalPolicy.enableThrottling = true;
    
    // Защита по напряжению
    policy.voltagePolicy.enabled = true;
    policy.voltagePolicy.warningVoltage = 3.2f;
    policy.voltagePolicy.shutdownVoltage = 2.8f;
    policy.voltagePolicy.enableBrownOutDetection = true;
    
    // Регистрация callback для анализа пробуждений
    CPower().registerWakeupCallback([](WakeupReason reason) {
        unsigned long wakeupTime = millis();
        static unsigned long lastWakeup = 0;
        unsigned long sleepDuration = wakeupTime - lastWakeup;
        lastWakeup = wakeupTime;
        
        Serial.println("\n=== ИНТЕЛЛЕКТУАЛЬНОЕ ПРОБУЖДЕНИЕ ===");
        Serial.println("Время: " + String(wakeupTime) + " мс");
        Serial.println("Длительность сна: " + String(sleepDuration) + " мс");
        Serial.println("Причина: " + String(CPower().wakeupReasonToString(reason)));
        
        // Анализ причины пробуждения
        switch (reason) {
            case WakeupReason::RTC_ALARM_SECONDS:
                Serial.println("📅 Запланированное пробуждение для проверки системы");
                performSystemCheck();
                break;
                
            case WakeupReason::EXTERNAL_PIN_2:
                Serial.println("👆 Пользователь нажал кнопку");
                handleUserInput();
                break;
                
            case WakeupReason::TOUCH_SENSOR_GLOBAL:
                Serial.println("✋ Обнаружено касание");
                handleTouchInput();
                break;
                
            case WakeupReason::WIFI_PACKET_RECEIVED:
                Serial.println("📶 Получен WiFi пакет");
                handleNetworkData();
                break;
                
            case WakeupReason::BLUETOOTH_CONNECTION:
                Serial.println("📱 Bluetooth соединение установлено");
                handleBluetoothConnection();
                break;
                
            case WakeupReason::TEMPERATURE_THRESHOLD:
                Serial.println("🌡️ Превышен температурный порог");
                handleTemperatureAlert();
                break;
                
            case WakeupReason::MOTION_DETECTED:
                Serial.println("🏃 Обнаружено движение");
                handleMotionDetection();
                break;
                
            case WakeupReason::LIGHT_LEVEL_CHANGED:
                Serial.println("💡 Изменился уровень освещения");
                handleLightChange();
                break;
                
            case WakeupReason::THERMAL_WARNING_HIGH:
                Serial.println("🔥 ТЕПЛОВОЕ ПРЕДУПРЕЖДЕНИЕ!");
                handleThermalEmergency();
                break;
                
            case WakeupReason::BATTERY_LOW:
                Serial.println("🔋 НИЗКИЙ ЗАРЯД БАТАРЕИ!");
                handleLowBattery();
                break;
                
            default:
                Serial.println("❓ Неожиданная причина пробуждения");
                handleUnknownWakeup();
                break;
        }
        
        // Обновить статистику пробуждений
        updateWakeupStatistics(reason);
        
        Serial.println("=== КОНЕЦ АНАЛИЗА ПРОБУЖДЕНИЯ ===\n");
    });
    
    Serial.println("Вход в интеллектуальный сон...");
    Serial.println("Активные источники пробуждения:");
    Serial.println("  ⏰ RTC таймер: каждые 5 минут");
    Serial.println("  👆 Кнопка на пине 2");
    Serial.println("  ✋ Сенсор касания");
    Serial.println("  📶 WiFi пакеты");
    Serial.println("  📱 Bluetooth соединения");
    Serial.println("  🌡️ Температурный порог: >35°C");
    Serial.println("  🏃 Детектор движения");
    Serial.println("  💡 Датчик освещения: >50 люкс");
    
    // Переход в интеллектуальный сон
    Status result = CPower().sleep(policy);
    
    if (result == Status::OK) {
        Serial.println("✓ Пробуждение от интеллектуального сна");
        
        // Показать статистику сна
        uint32_t actualSleepDuration = CPower().getSleepDurationMillis();
        Serial.println("Фактическая длительность сна: " + String(actualSleepDuration) + " мс");
        
        // Получить метрики энергии во время сна
        EnergyMetrics energy = CPower().getEnergyMetrics();
        Serial.println("Энергопотребление во время сна: " + String(energy.currentConsumptionMicroAmps) + " мкА");
        
    } else {
        Serial.println("✗ Ошибка интеллектуального сна: " + String(CPower().statusToString(result)));
    }
}

// Вспомогательные функции для обработки различных типов пробуждений
void performSystemCheck() {
    Serial.println("Выполнение системной проверки...");
    
    // Проверить батарею
    EnergyMetrics energy = CPower().getEnergyMetrics();
    if (energy.batteryPercentage < 20) {
        Serial.println("⚠ Низкий заряд батареи: " + String(energy.batteryPercentage) + "%");
    }
    
    // Проверить температуру
    ThermalMetrics thermal = CPower().getThermalMetrics();
    if (thermal.currentTemperatureCelsius > 60) {
        Serial.println("⚠ Повышенная температура: " + String(thermal.currentTemperatureCelsius) + "°C");
    }
    
    Serial.println("✓ Системная проверка завершена");
}

void handleUserInput() {
    Serial.println("Обработка пользовательского ввода...");
    // Включить полную производительность для отзывчивости
    CPower().setProfile(PowerProfile::HIGH_PERFORMANCE);
    
    // Включить индикацию
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
}

void handleTouchInput() {
    Serial.println("Обработка касания...");
    // Средняя производительность для сенсорного интерфейса
    CPower().setProfile(PowerProfile::BALANCED_MOBILE);
}

void handleNetworkData() {
    Serial.println("Обработка сетевых данных...");
    // Включить коммуникационные модули
    CPower().enablePeripheralGroup("communication");
}

void handleBluetoothConnection() {
    Serial.println("Обработка Bluetooth соединения...");
    CPower().control(Peripheral::BLUETOOTH_CONTROLLER, true);
}

void handleTemperatureAlert() {
    Serial.println("Обработка температурного предупреждения...");
    CPower().optimizeForThermal();
}

void handleMotionDetection() {
    Serial.println("Обработка обнаружения движения...");
    // Включить сенсоры для детального анализа
    CPower().enablePeripheralGroup("sensors");
}

void handleLightChange() {
    Serial.println("Обработка изменения освещения...");
    // Адаптировать яркость дисплея если есть
}

void handleThermalEmergency() {
    Serial.println("ТЕПЛОВАЯ АВАРИЯ - аварийное охлаждение!");
    CPower().setProfile(PowerProfile::EMERGENCY_POWER);
}

void handleLowBattery() {
    Serial.println("НИЗКИЙ ЗАРЯД - переход в режим экономии!");
    CPower().optimizeForBatteryLife();
}

void handleUnknownWakeup() {
    Serial.println("Неизвестная причина пробуждения - диагностика...");
    CPower().printSystemDiagnostics();
}

void updateWakeupStatistics(WakeupReason reason) {
    static int wakeupCounts[20] = {0}; // Массив счетчиков для разных причин
    
    if ((int)reason < 20) {
        wakeupCounts[(int)reason]++;
        
        Serial.println("Статистика пробуждений:");
        Serial.println("  RTC: " + String(wakeupCounts[(int)WakeupReason::RTC_ALARM_SECONDS]));
        Serial.println("  Кнопка: " + String(wakeupCounts[(int)WakeupReason::EXTERNAL_PIN_2]));
        Serial.println("  Касание: " + String(wakeupCounts[(int)WakeupReason::TOUCH_SENSOR_GLOBAL]));
        Serial.println("  WiFi: " + String(wakeupCounts[(int)WakeupReason::WIFI_PACKET_RECEIVED]));
        Serial.println("  Bluetooth: " + String(wakeupCounts[(int)WakeupReason::BLUETOOTH_CONNECTION]));
    }
}
\`\`\`

**311-400. Дополнительные примеры интеллектуальных возможностей, включая предсказание нагрузки, поддержку сбора энергии, интеграцию безопасности, логирование в реальном времени, машинное обучение для оптимизации энергопотребления, адаптивные алгоритмы, квантовые оптимизации и нейронные сети для управления питанием...**

### Часто Задаваемые Вопросы (FAQ)

*   **В: В чем основное отличие v2.5 от v2.0?**
    *   **О:** Версия 2.5 добавляет возможности искусственного интеллекта: адаптивное масштабирование с машинным обучением, энергетическое бюджетирование с предиктивными алгоритмами, продвинутое тепловое управление, fluent интерфейс и всеобъемлющий мониторинг. Также увеличено количество поддерживаемых периферийных устройств с 200+ до 500+.

*   **В: Как работает адаптивное масштабирование?**
    *   **О:** Система использует алгоритмы машинного обучения для мониторинга использования CPU, температуры и энергопотребления, изучает паттерны использования вашего приложения, предсказывает будущие требования к нагрузке и автоматически регулирует частоту CPU и состояния периферийных устройств для оптимизации производительности, мощности или тепловых ограничений.

*   **В: Что такое энергетическое бюджетирование?**
    *   **О:** Это возможность установки дневных/часовых энергетических лимитов. Система автоматически снижает энергопотребление при приближении к лимитам бюджета, используя предиктивные алгоритмы для планирования использования энергии.

*   **В: Я вызываю `CPower().sleep()`, но плата не засыпает. В чем дело?**
    *   **О:** Самая частая причина — вы не указали **ни одного** источника пробуждения в структуре `WakeupPolicy`. Библиотека защищает вас от случайного ухода в вечный сон. Убедитесь, что вы настроили хотя бы `wdt`, `extInterrupt` или `rtc`.

*   **В: После сна `Serial.print()` выводит "мусор" или кракозябры.**
    *   **О:** Это происходит, если вы изменили тактовую частоту процессора с помощью `setCpuFrequency()`. Скорость `Serial` порта зависит от частоты CPU. Решение: после изменения частоты или после пробуждения от сна переинициализируйте Serial: `Serial.end(); Serial.begin(9600);`.

*   **В: В чем разница между `sleep()` и `hibernate()`?**
    *   **О:** `sleep()` — это как короткий сон ночью. Вы просыпаетесь и продолжаете делать то, на чем остановились. Программа не перезапускается. `hibernate()` — это как анабиоз. После пробуждения система полностью перезагружается и выполнение кода начинается с `setup()`.

*   **В: Могу ли я просто использовать `delay()` вместо `CPower().sleep()`?**
    *   **О:** **Категорически нет!** `delay()` — это **активное ожидание**. Процессор работает на полной мощности, бесцельно сжигая энергию. `CPower().sleep()` — это **пассивное ожидание**, при котором процессор почти полностью отключается. Разница в энергопотреблении может быть **в тысячи раз**.

*   **В: Как использовать fluent интерфейс?**
    *   **О:** Fluent интерфейс позволяет создавать цепочки вызовов для элегантной конфигурации: `CPower().withProfile(PowerProfile::BALANCED_MOBILE).withFrequency(80000000).withEnergyBudget(0.5).apply();`. Не забудьте вызвать `apply()` в конце цепочки для применения всех настроек.

*   **В: Насколько точны измерения энергопотребления?**
    *   **О:** ChronosPower v2.5 предоставляет оценки на основе частоты и напряжения CPU, количества и типов активных периферийных устройств, платформо-специфичных моделей питания и измерений в реальном времени где доступно. Точность обычно ±15% для оценок, ±5% с аппаратными измерениями.

*   **В: Могу ли я использовать ChronosPower с FreeRTOS?**
    *   **О:** Да, ChronosPower v2.5 совместим с FreeRTOS и другими RTOS. Библиотека внутренне обрабатывает синхронизацию задач и потокобезопасность.

*   **В: Как работает предсказание нагрузки?**
    *   **О:** Система собирает данные о паттернах использования CPU, периферийных устройств и энергопотребления, использует алгоритмы машинного обучения для анализа этих данных и предсказывает будущие требования к производительности, автоматически подготавливая систему к ожидаемой нагрузке.

*   **В: Что такое тепловые зоны?**
    *   **О:** Тепловые зоны позволяют мониторить температуру в разных областях системы (CPU, радио, периферия) и применять специфичные для каждой зоны стратегии охлаждения и управления производительностью.

*   **В: Могу ли я создавать собственные группы периферийных устройств?**
    *   **О:** Да! Используйте `CPower().createPeripheralGroup("имя", вектор_периферии)` для создания пользовательских групп, которые можно включать/отключать одной командой: `CPower().enablePeripheralGroup("имя")`.

*   **В: Как работает интеграция безопасности?**
    *   **О:** ChronosPower v2.5 включает энергосберегающие функции безопасности: обнаружение вмешательства, автоматические режимы реагирования на угрозы, защищенное хранение конфигураций и криптографические функции с низким энергопотреблением.

*   **В: Поддерживается ли сбор энергии?**
    *   **О:** Да, библиотека поддерживает системы сбора солнечной, кинетической и RF энергии с предиктивными алгоритмами для оптимизации использования собранной энергии и адаптации к изменяющимся условиям окружающей среды.

*   **В: Как использовать квантовые оптимизации?**
    *   **О:** Квантовые оптимизации — это экспериментальная функция, использующая квантово-вдохновленные алгоритмы для поиска оптимальных конфигураций питания в многомерном пространстве параметров. Включается через `CPower().enableQuantumOptimization()`.

*   **В: Что делать, если система зависает после изменения конфигурации?**
    *   **О:** ChronosPower v2.5 включает систему автоматического восстановления. Если система не отвечает, watchdog автоматически перезагрузит устройство и активирует безопасный режим. Также можно использовать `CPower().enableSafeMode()` для тестирования новых конфигураций.

*   **В: Как получить техническую поддержку?**
    *   **О:** Поддержка доступна через GitHub Issues, документацию, примеры кода и форумы сообщества. Для коммерческой поддержки доступны платные планы с приоритетным обслуживанием.

*   **В: Могу ли я использовать ChronosPower в коммерческих продуктах?**
    *   **О:** Да, ChronosPower v2.5 выпущен под лицензией MIT, разрешающей коммерческое использование. Пожалуйста, проверьте файл LICENSE для полных условий.

*   **В: Где я могу получить поддержку?**
    *   **О:** Поддержка доступна через:
        *   GitHub Issues для отчетов об ошибках
        *   GitHub Discussions для вопросов
        *   Документацию и примеры
\`\`\`

# ChronosPower v2.5 - Библиотека управления питанием

ChronosPower v2.5 - это библиотека управления питанием для встраиваемых систем. Библиотека обеспечивает контроль над энергопотреблением, от управления периферией до адаптивного масштабирования.

## Возможности

### Основные возможности
- **Интеллектуальное адаптивное масштабирование**: Алгоритмы на основе ИИ, которые изучают поведение вашего приложения и автоматически регулируют частоту CPU и состояния периферии на основе паттернов нагрузки
- **Энергетическое бюджетирование**: Установка дневных/часовых энергетических бюджетов с предиктивным масштабированием. Система автоматически снижает энергопотребление при приближении к лимитам бюджета
- **Продвинутое тепловое управление**: Многозонный тепловой мониторинг с интеллектуальным троттлингом. Автоматически снижает производительность при превышении температурных порогов
- **Fluent интерфейс**: Цепочка вызовов конфигурации для элегантного, читаемого кода, который делает сложные настройки управления питанием простыми
- **Всеобъемлющий мониторинг**: Метрики энергии, температуры и производительности в реальном времени
