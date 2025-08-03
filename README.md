<p align="center">
  <img src="https://raw.githubusercontent.com/Fedoroks2024/ChronosPower/68086992c50aa9aa06d053348726e9858f35f09b/ChronosPower.svg" alt="Логотип ChronosPower" width="300"/>
</p>

__ChronosPower__ — это многофункциональный, кросс-платформенный фреймворк для управления энергопотреблением встраиваемых систем. Он предоставляет единый, высокоуровневый API для контроля над низкоуровневыми аспектами питания на широком спектре архитектур — от классических 8-битных микроконтроллеров до современных многоядерных ARM-процессоров.

### Оглавление

*   [Ключевые Концепции](#ключевые-концепции)
*   [Что нового в версии 2.0](#что-нового-в-версии-20)
*   [Установка](#установка)
*   [Справочник API](#справочник-api)
    *   [Глава 1: Основные Команды](#глава-1-основные-команды)
    *   [Глава 2: Управление Системой](#глава-2-управление-системой)
    *   [Глава 3: Диагностика и События](#глава-3-диагностика-и-события)
    *   [Глава 4: Структуры Конфигурации](#глава-4-структуры-конфигурации)
*   [Большая Книга Примеров](#большая-книга-примеров)
    *   [Раздел 1: Основы Сна (Примеры 1-20)](#раздел-1-основы-сна-примеры-1-20)
    *   [Раздел 2: Управление Частотой и Профилями (Примеры 21-40)](#раздел-2-управление-частотой-и-профилями-примеры-21-40)
    *   [Раздел 3: Ручное Управление Периферией (Примеры 41-70)](#раздел-3-ручное-управление-периферией-примеры-41-70)
    *   [Раздел 4: Диагностика и Информация (Примеры 71-100)](#раздел-4-диагностика-и-информация-примеры-71-100)
    *   [Раздел 5: Продвинутые Сценарии Пробуждения (Примеры 101-150)](#раздел-5-продвинутые-сценарии-пробуждения-примеры-101-150)
    *   [Раздел 6: Домены и Массовое Управление (Примеры 151-180)](#раздел-6-домены-и-массовое-управление-примеры-151-180)
    *   [Раздел 7: Диагностика и Конечные Автоматы (Примеры 181-230)](#раздел-7-диагностика-и-конечные-автоматы-примеры-181-230)
    *   [Раздел 8: Специализированные Политики (Примеры 231-260)](#раздел-8-специализированные-политики-примеры-231-260)
    *   [Раздел 9: Платформо-зависимые Техники (Примеры 261-301)](#раздел-9-платформо-зависимые-техники-примеры-261-301)
*   [Часто Задаваемые Вопросы (FAQ)](#часто-задаваемые-вопросы-faq)

### Ключевые Концепции

1.  **Профили Питания (`PowerProfile`)**: Предопределенные наборы настроек для быстрого переключения между режимами. Например, `MAX_PERFORMANCE` для вычислений и `ULTRA_LOW_POWER_IDLE` для ожидания.
2.  **Политики Питания (`PowerPolicy`)**: Детальная конфигурация, позволяющая точно указать частоту CPU, активные периферийные устройства, уровни напряжения и режимы работы Flash-памяти.
3.  **Политика Пробуждения (`WakeupPolicy`)**: Единая структура для настройки всех возможных источников пробуждения: внешние прерывания, RTC, сторожевой таймер (WDT), сенсорные входы и т.д.
4.  **Абстракция Периферии (`Peripheral`)**: Глобальный перечислитель для каждого мыслимого периферийного блока, позволяющий управлять его тактованием и питанием через единый API.
5.  **Кросс-платформенная Совместимость**: Ядро библиотеки использует директивы препроцессора для автоматического выбора корректной низкоуровневой реализации для целевой архитектуры (AVR, ESP32, STM32, SAMD, RP2040 и др.).

### Что нового в версии 2.0

Версия 2.0 представляет собой монументальное расширение фреймворка, сфокусированное на максимальной гранулярности управления и поддержке самых современных и сложных микроконтроллеров.

*   **Гипер-расширение API**: Перечисления `Peripheral` и `WakeupReason` расширены до сотен новых значений, охватывая практически все существующие аппаратные блоки: от отдельных каналов DMA и банков памяти до специализированных мультимедийных и криптографических модулей.
*   **Управление доменами питания**: Введена концепция `PowerDomain` и функция `setClockGatingForDomain` для массового управления питанием целых подсистем микроконтроллера (например, всех таймеров или всех коммуникационных интерфейсов).
*   **Детализированная конфигурация**: Добавлены новые структуры (`ThermalPolicy`, `VoltagePolicy`, `UartWakeupConfig`) и расширены существующие для тончайшей настройки поведения системы при пробуждении и во время работы.
*   **Новые профили питания**: Добавлены узкоспециализированные профили, такие как `MACHINE_LEARNING_INFERENCE` и `AUDIO_PROCESSING`, для типовых задач.
*   **Расширенная диагностика**: Добавлены функции `getResetReason()` для получения точной причины перезагрузки (включая сбои тактирования, ошибки ПО и т.д.) и новые, более специфичные коды ошибок в `Status`.
*   **Управление состоянием**: Новая функция `getFlashState()` для контроля над состоянием Flash-памяти.
*   **Структурная готовность**: Размер маски периферии увеличен до 512 бит (`uint64_t[8]`), что обеспечивает запас для поддержки будущих, еще более сложных чипов.

### Установка

1.  Скачайте ZIP-архив с библиотекой.
2.  В Arduino IDE выберите `Скетч` -> `Подключить библиотеку` -> `Добавить .ZIP библиотеку...`.
3.  Выберите скачанный ZIP-файл.
4.  Подключите библиотеку в вашем скетче: `#include <ChronosPower.h>`.

### Справочник API

Этот раздел содержит детальное описание всех публичных функций, структур и перечислений, доступных в `ChronosPower`.

#### **Глава 1: Основные Команды**
*   `Status begin(PowerProfile initialProfile = PowerProfile::BALANCED_MOBILE)`: Инициализация менеджера питания.
*   `Status setProfile(PowerProfile profile)`: Установка предопределенного профиля питания.
*   `Status sleep(const WakeupPolicy& policy)`: Отправка системы в режим сна с последующим пробуждением.
*   `[[noreturn]] void hibernate(const WakeupPolicy& policy)`: Отправка системы в режим гибернации с последующей перезагрузкой.
*   `[[noreturn]] void shutdown()`: Полное отключение питания или вход в бесконечную гибернацию.

#### **Глава 2: Управление Системой**
*   `Status setCpuFrequency(uint32_t frequencyHz)`: Установка тактовой частоты процессора.
*   `uint32_t getCpuFrequency() const`: Получение текущей тактовой частоты.
*   `Status control(Peripheral p, bool enable)`: Включение/отключение одного периферийного блока.
*   `Status massControl(const uint64_t masks[], uint8_t maskCount, bool enable)`: Массовое управление периферией через битовые маски.
*   `Status setCustomPolicy(const PowerPolicy& policy)`: Применение пользовательской политики энергосбережения.
*   `Status getCustomPolicy(PowerPolicy& policy) const`: Получение текущей пользовательской политики.
*   `Status setClockGatingForDomain(PowerDomain domain, bool enable)`: Включение/отключение тактирования для целого домена периферии.

#### **Глава 3: Диагностика и События**
*   `WakeupReason getWakeupReason()`: Получение причины последнего пробуждения из сна.
*   `ResetReason getResetReason()`: Получение причины последней полной перезагрузки системы.
*   `uint32_t getSleepDurationMillis() const`: Получение длительности последнего сна.
*   `Status registerEventCallback(ChronosCallback callback)`: Регистрация функции обратного вызова на события питания.
*   `Status getInternalTemperature(float& temp)`: Измерение температуры кристалла.
*   `Status getCoreVoltage(float& voltage)`: Измерение напряжения ядра.
*   `Status getFlashState(FlashState& state)`: Получение информации о состоянии Flash-памяти.

#### **Глава 4: Структуры Конфигурации**
*   `PowerPolicy`: Комплексная политика, включающая `cpuFrequencyHz`, `voltageScaleLevel`, `flashDeepPowerDown` и массив `peripheralMask[8]` на 512 периферийных устройств.
*   `WakeupPolicy`: Комплексная политика пробуждения, включающая вложенные структуры для настройки каждого источника пробуждения.
*   `ExternalInterruptConfig`: Настройка внешних прерываний, включая маску пинов, режим и время подавления дребезга.
*   `RtcConfig`: Настройка пробуждения от RTC по таймауту или точному времени.
*   `WatchdogConfig`: Настройка сторожевого таймера, включая оконный режим.
*   `UartWakeupConfig`: Настройка пробуждения от UART по стартовому биту или совпадению адреса.
*   `ThermalPolicy`: Настройка порогов температуры для предупреждения и отключения.
*   `VoltagePolicy`: Настройка порогов напряжения для предупреждения и отключения.

### Большая Книга Примеров

#### **Раздел 1: Основы Сна (Примеры 1-20)**
**1. Сон на 2 секунды с помощью WDT**
```cpp
ChronosPower::WakeupPolicy p1;
p1.wdt.timeoutMillis = 2000;
p1.wdt.generateInterrupt = true;
CPower.sleep(p1);
```
**2. Сон до нажатия кнопки на пине 2**
```cpp
pinMode(2, INPUT_PULLUP);
ChronosPower::WakeupPolicy p2;
p2.extInterrupt.pinMask = (1ULL << 2);
p2.extInterrupt.triggerMode = FALLING;
CPower.sleep(p2);
```
**3. Сон до отпускания кнопки на пине 3**
```cpp
pinMode(3, INPUT_PULLUP);
ChronosPower::WakeupPolicy p3;
p3.extInterrupt.pinMask = (1ULL << 3);
p3.extInterrupt.triggerMode = RISING;
CPower.sleep(p3);
```
**4. Сон до появления высокого уровня на пине 4**
```cpp
pinMode(4, INPUT);
ChronosPower::WakeupPolicy p4;
p4.extInterrupt.pinMask = (1ULL << 4);
p4.extInterrupt.triggerMode = RISING;
CPower.sleep(p4);
```
**5. Сон до появления низкого уровня на пине 5**
```cpp
pinMode(5, INPUT_PULLUP);
ChronosPower::WakeupPolicy p5;
p5.extInterrupt.pinMask = (1ULL << 5);
p5.extInterrupt.triggerMode = LOW;
CPower.sleep(p5);
```
**6. Сон на 1 минуту с помощью RTC**
```cpp
ChronosPower::WakeupPolicy p6;
p6.rtc.timeoutSeconds = 60;
CPower.sleep(p6);
```
**7. Гибернация на 2 часа**
```cpp
ChronosPower::WakeupPolicy p7;
p7.rtc.timeoutSeconds = 2 * 3600;
CPower.hibernate(p7);
```
**8. Гибернация до нажатия кнопки на пине 12**
```cpp
pinMode(12, INPUT_PULLUP);
ChronosPower::WakeupPolicy p8;
p8.extInterrupt.pinMask = (1ULL << 12);
p8.extInterrupt.triggerMode = FALLING;
CPower.hibernate(p8);
```
**9. Сон до касания (ESP32)**
```cpp
ChronosPower::WakeupPolicy p9;
p9.touchThreshold = 40;
CPower.sleep(p9);
```
**10. Комбинированный сон: 10 секунд ИЛИ кнопка на пине 2**
```cpp
ChronosPower::WakeupPolicy p10;
p10.wdt.timeoutMillis = 10000;
p10.wdt.generateInterrupt = true;
pinMode(2, INPUT_PULLUP);
p10.extInterrupt.pinMask = (1ULL << 2);
p10.extInterrupt.triggerMode = FALLING;
CPower.sleep(p10);
```
**11. Сон на 20 секунд на Arduino Uno (цикл WDT)**
```cpp
ChronosPower::WakeupPolicy p11_8s;
p11_8s.wdt.timeoutMillis = 8000;
p11_8s.wdt.generateInterrupt = true;
ChronosPower::WakeupPolicy p11_4s;
p11_4s.wdt.timeoutMillis = 4000;
p11_4s.wdt.generateInterrupt = true;
CPower.sleep(p11_8s); CPower.sleep(p11_8s); CPower.sleep(p11_4s);
```
**12. Сон до получения любого символа в Serial**
```cpp
ChronosPower::WakeupPolicy p12;
p12.uartWakeup.portIndex = 1;
CPower.sleep(p12);
```
**13. Пробуждение по любой из трех кнопок**
```cpp
pinMode(2, INPUT_PULLUP); pinMode(3, INPUT_PULLUP); pinMode(4, INPUT_PULLUP);
ChronosPower::WakeupPolicy p13;
p13.extInterrupt.pinMask = (1ULL << 2) | (1ULL << 3) | (1ULL << 4);
p13.extInterrupt.triggerMode = FALLING;
CPower.sleep(p13);
```
**14. Сон с предварительным отключением АЦП и SPI**
```cpp
CPower.control(Peripheral::ADC_0_12BIT, false);
CPower.control(Peripheral::SPI_0, false);
// ... код для сна ...
```
**15. Гибернация до тех пор, пока на пине 13 не появится высокий уровень**
```cpp
pinMode(13, INPUT);
ChronosPower::WakeupPolicy p15;
p15.extInterrupt.pinMask = (1ULL << 13);
p15.extInterrupt.triggerMode = HIGH;
CPower.hibernate(p15);
```
**16. Полностью выключить устройство**
```cpp
CPower.shutdown();
```
**17. Сон с очень низкой чувствительностью к касанию**
```cpp
ChronosPower::WakeupPolicy p17;
p17.touchThreshold = 80;
CPower.sleep(p17);
```
**18. Сон на 1 час на Arduino (очень длинный цикл WDT)**
```cpp
ChronosPower::WakeupPolicy p18;
p18.wdt.timeoutMillis = 8000;
for (int i = 0; i < 450; ++i) { CPower.sleep(p18); }
```
**19. Комбинированный сон: RTC на 5 минут ИЛИ касание**
```cpp
ChronosPower::WakeupPolicy p19;
p19.rtc.timeoutSeconds = 5 * 60;
p19.touchThreshold = 40;
CPower.sleep(p19);
```
**20. Пробуждение по изменению логического уровня (CHANGE)**
```cpp
pinMode(7, INPUT_PULLUP);
ChronosPower::WakeupPolicy p20;
p20.extInterrupt.pinMask = (1ULL << 7);
p20.extInterrupt.triggerMode = CHANGE;
CPower.sleep(p20);
```

#### **Раздел 2: Управление Частотой и Профилями (Примеры 21-40)**
**21. Установить профиль максимальной производительности**
```cpp
CPower.setProfile(PowerProfile::MAX_PERFORMANCE);
```
**22. Установить сбалансированный профиль**
```cpp
CPower.setProfile(PowerProfile::BALANCED_MOBILE);
```
**23. Установить профиль для активной работы с низкой мощностью**
```cpp
CPower.setProfile(PowerProfile::LOW_POWER_ACTIVE);
```
**24. Установить профиль для максимальной экономии в режиме ожидания**
```cpp
CPower.setProfile(PowerProfile::ULTRA_LOW_POWER_IDLE);
```
**25. Замедлить AVR процессор до 8 МГц**
```cpp
CPower.setCpuFrequency(8000000);
```
**26. Замедлить AVR процессор до 4 МГц**
```cpp
CPower.setCpuFrequency(4000000);
```
**27. Замедлить AVR процессор до 1 МГц**
```cpp
CPower.setCpuFrequency(1000000);
```
**28. Установить частоту ESP32 на 240 МГц**
```cpp
CPower.setCpuFrequency(240000000);
```
**29. Установить частоту ESP32 на 80 МГц**
```cpp
CPower.setCpuFrequency(80000000);
```
**30. Установить частоту ESP32 на 40 МГц**
```cpp
CPower.setCpuFrequency(40000000);
```
**31. Динамическое изменение частоты в цикле**
```cpp
CPower.setCpuFrequency(240000000);
// ... делаем что-то быстро ...
CPower.setCpuFrequency(40000000);
// ... ждем ...
```
**32. Инициализация сразу в режиме LOW_POWER_ACTIVE**
```cpp
CPower.begin(PowerProfile::LOW_POWER_ACTIVE);
```
**33. Получение текущей частоты процессора**
```cpp
uint32_t currentFreq = CPower.getCpuFrequency();
Serial.println(currentFreq);
```
**34. Сценарий: быстрый замер, потом сон**
```cpp
CPower.setProfile(PowerProfile::HIGH_PERFORMANCE);
// takeFastMeasurement();
CPower.setProfile(PowerProfile::DEEP_SLEEP_PREP);
// CPower.sleep(p1);
```
**35. Возврат к стандартной частоте Arduino Uno**
```cpp
CPower.setCpuFrequency(16000000);
```
**36. Установка очень низкой частоты для RP2040 (Pico)**
```cpp
CPower.setCpuFrequency(20000000); // 20 MHz
```
**37. Установка максимальной частоты для RP2040 (Pico)**
```cpp
CPower.setCpuFrequency(133000000);
```
**38. Создание и применение своей политики**
```cpp
ChronosPower::PowerPolicy customPolicy;
customPolicy.cpuFrequencyHz = 5000000;
customPolicy.peripheralMask[0] = (1ULL << (int)Peripheral::UART_0);
CPower.setCustomPolicy(customPolicy);
```
**39. Переключение с кастомной политики обратно на стандартный профиль**
```cpp
CPower.setProfile(PowerProfile::BALANCED_MOBILE);
```
**40. Проверка текущего профиля**
```cpp
if (CPower.getProfile() == PowerProfile::MAX_PERFORMANCE) { /* ... */ }
```

#### **Раздел 3: Ручное Управление Периферией (Примеры 41-70)**
**41. Отключить весь блок АЦП**
```cpp
CPower.control(Peripheral::ADC_0_12BIT, false);
```
**42. Отключить весь блок I2C**
```cpp
CPower.control(Peripheral::I2C_0, false);
```
**43. Отключить весь блок SPI**
```cpp
CPower.control(Peripheral::SPI_0, false);
```
**44. Отключить USB**
```cpp
CPower.control(Peripheral::USB_DEVICE, false);
```
**45. Отключить Wi-Fi на ESP32**
```cpp
CPower.control(Peripheral::WIFI_MAC_2_4GHZ, false);
```
**46. Отключить Bluetooth на ESP32**
```cpp
CPower.control(Peripheral::BLUETOOTH_CONTROLLER, false);
```
**47. Отключить Brown-Out-Detector (рискованно)**
```cpp
CPower.control(Peripheral::BROWN_OUT_DETECTOR, false);
```
**48. Отключить внутренний датчик температуры**
```cpp
CPower.control(Peripheral::INTERNAL_TEMP_SENSOR, false);
```
**49. Отключить аппаратный генератор случайных чисел**
```cpp
CPower.control(Peripheral::TRNG_UNIT, false);
```
**50. Отключить основной таймер Arduino (millis/delay перестанут работать)**
```cpp
CPower.control(Peripheral::TIMER_0_8BIT_BASIC, false);
```
**51. Включить таймер обратно**
```cpp
CPower.control(Peripheral::TIMER_0_8BIT_BASIC, true);
```
**52. Отключить все таймеры (0, 1, 2)**
```cpp
CPower.control(Peripheral::TIMER_0_8BIT_BASIC, false);
CPower.control(Peripheral::TIMER_1_16BIT_GENERAL, false);
CPower.control(Peripheral::TIMER_2_8BIT_ASYNC, false);
```
**53. Отключить всю периферию порта GPIOA**
```cpp
CPower.control(Peripheral::GPIO_PORT_A, false);
```
**54. Отключить Flash-кэш (замедлит выполнение)**
```cpp
CPower.control(Peripheral::FLASH_CACHE_DATA, false);
```
**55. Включить его обратно**
```cpp
CPower.control(Peripheral::FLASH_CACHE_DATA, true);
```
**56. Отключить контроллер SD-карты**
```cpp
CPower.control(Peripheral::SDIO_0, false);
```
**57. Отключить интерфейс камеры**
```cpp
CPower.control(Peripheral::CAMERA_INTERFACE_DCMI, false);
```
**58. Отключить Ethernet**
```cpp
CPower.control(Peripheral::ETHERNET_MAC, false);
```
**59. Отключить ШИМ-модуль**
```cpp
CPower.control(Peripheral::PWM_MODULE_0, false);
```
**60. Отключить DMA**
```cpp
CPower.control(Peripheral::DMA_CONTROLLER_0, false);
```
**61-70. Комбинации: оставить только UART и I2C**
```cpp
CPower.setProfile(PowerProfile::ULTRA_LOW_POWER_IDLE);
CPower.control(Peripheral::UART_0, true);
CPower.control(Peripheral::I2C_0, true);
```

#### **Раздел 4: Диагностика и Информация (Примеры 71-100)**
**71. Вывести причину пробуждения в Serial**
```cpp
Serial.println((int)CPower.getWakeupReason());
```
**72. Среагировать на пробуждение от таймера**
```cpp
if (CPower.getWakeupReason() == WakeupReason::WATCHDOG_TIMEOUT_INTERRUPT) { /* ... */ }
```
**73. Среагировать на пробуждение от кнопки на пине 2**
```cpp
if (CPower.getWakeupReason() == WakeupReason::EXTERNAL_PIN_2) { /* ... */ }
```
**74. Среагировать на пробуждение от RTC**
```cpp
if (CPower.getWakeupReason() == WakeupReason::RTC_ALARM_SECONDS) { /* ... */ }
```
**75. Вывести длительность последнего сна**
```cpp
Serial.println(CPower.getSleepDurationMillis());
```
**76. Использовать колбэк для логирования**
```cpp
void loggerCallback(WakeupReason reason) { Serial.println((int)reason); }
// CPower.registerEventCallback(loggerCallback);
```
**77. Измерить и вывести температуру чипа**
```cpp
float t;
if(CPower.getInternalTemperature(t) == Status::OK) { Serial.println(t); }
```
**78. Измерить и вывести напряжение ядра**
```cpp
float v;
if(CPower.getCoreVoltage(v) == Status::OK) { Serial.println(v); }
```
**79. Очистить флаг причины пробуждения**
```cpp
CPower.clearWakeupReason();
```
**80. Синхронизировать millis() после сна (обычно авто)**
```cpp
CPower.synchronizeSystemTime();
```
**81. Включить светодиод, если проснулись от кнопки**
```cpp
if (CPower.getWakeupReason() == WakeupReason::EXTERNAL_PIN_0) {
  digitalWrite(LED_BUILTIN, HIGH);
}
```
**82-100. Комбинации проверок и логирования для всех возможных причин пробуждения.**

#### **Раздел 5: Продвинутые Сценарии Пробуждения (Примеры 101-150)**
**101. Пробуждение по адресу 0x7E на шине UART0**
```cpp
ChronosPower::WakeupPolicy p; p.uartWakeup.portIndex = 1; p.uartWakeup.wakeupOnAddressMatch = true; p.uartWakeup.address = 0x7E; CPower.sleep(p);
```
**102. Пробуждение по стартовому биту на UART2**
```cpp
ChronosPower::WakeupPolicy p; p.uartWakeup.portIndex = 3; p.uartWakeup.wakeupOnStartBit = true; CPower.sleep(p);
```
**103. Пробуждение от аналогового компаратора 0**
```cpp
ChronosPower::WakeupPolicy p; p.enableAnalogComparatorWakeup = true; CPower.sleep(p);
```
**104. Сон с подавлением дребезга кнопки (50 мс)**
```cpp
ChronosPower::WakeupPolicy p; p.extInterrupt.pinMask = (1ULL << 2); p.extInterrupt.triggerMode = FALLING; p.extInterrupt.debounceMicros = 50000; CPower.sleep(p);
```
**105. Пробуждение по Wi-Fi пакету (ESP32)**
```cpp
ChronosPower::WakeupPolicy p; p.enableWifiWakeup = true; CPower.sleep(p);
```
**106. Пробуждение по Bluetooth соединению (ESP32)**
```cpp
ChronosPower::WakeupPolicy p; p.enableBluetoothWakeup = true; CPower.sleep(p);
```
**107. RTC-будильник на точное время (15:30:00)**
```cpp
ChronosPower::WakeupPolicy p; p.rtc.alarmHour = 15; p.rtc.alarmMinute = 30; p.rtc.alarmSecond = 0; CPower.sleep(p);
```
**108. RTC-будильник на конкретную дату**
```cpp
ChronosPower::WakeupPolicy p; p.rtc.alarmHour=9; p.rtc.matchDate=true; p.rtc.alarmDay=1; p.rtc.alarmMonth=1; CPower.sleep(p);
```
**109. Сон с оконным WDT (сброс в окне 50%-100%)**
```cpp
ChronosPower::WakeupPolicy p; p.wdt.timeoutMillis=4000; p.wdt.windowedMode=true; p.wdt.windowPercent=50; p.wdt.generateInterrupt=false;
```
**110. Комбинация: Кнопка на пине 4 ИЛИ RTC через 30 сек**
```cpp
ChronosPower::WakeupPolicy p; p.rtc.timeoutSeconds=30; p.extInterrupt.pinMask=(1ULL<<4); p.extInterrupt.triggerMode=FALLING; CPower.sleep(p);
```
**111. Комбинация: Касание ИЛИ WDT на 15 секунд**
```cpp
ChronosPower::WakeupPolicy p; p.wdt.timeoutMillis=15000; p.touchThreshold=40; CPower.sleep(p);
```
**112. Комбинация: UART ИЛИ Кнопка на пине 5**
```cpp
ChronosPower::WakeupPolicy p; p.uartWakeup.portIndex=1; p.extInterrupt.pinMask=(1ULL<<5); p.extInterrupt.triggerMode=RISING; CPower.sleep(p);
```
**113. Пробуждение от двух кнопок с разным триггером (концепт, если чип поддерживает)**
```cpp
// Концепт. Обычно триггер один на группу пинов.
```
**114. Пробуждение по превышению порога АЦП (концепт)**
```cpp
// if(analogRead(A0) > 800) { CPower.sleep(p_with_timer); }
```
**115. Гибернация до тех пор, пока на пине не будет низкого уровня, но с таймаутом в 1 час**
```cpp
ChronosPower::WakeupPolicy p; p.rtc.timeoutSeconds=3600; p.extInterrupt.pinMask=(1ULL<<8); p.extInterrupt.triggerMode=LOW; CPower.hibernate(p);
```
**116-150. Дальнейшие комбинации и вариации параметров.**

#### **Раздел 6: Домены и Массовое Управление (Примеры 151-180)**
**151. Отключить все таймеры и ШИМ**
```cpp
CPower.setClockGatingForDomain(PowerDomain::TIMERS_AND_PWM, false);
```
**152. Включить все коммуникационные интерфейсы**
```cpp
CPower.setClockGatingForDomain(PowerDomain::COMMUNICATIONS, true);
```
**153. Отключить все модули безопасности (AES, SHA, TRNG)**
```cpp
CPower.setClockGatingForDomain(PowerDomain::SECURITY_MODULES, false);
```
**154. Отключить всё, кроме базовой системы и GPIO**
```cpp
CPower.setClockGatingForDomain(PowerDomain::ALL, false); CPower.setClockGatingForDomain(PowerDomain::GPIO, true);
```
**155. Включение только UART1 и SPI2 через маску**
```cpp
ChronosPower::PowerPolicy p; CPower.getCustomPolicy(p); for(int i=0;i<8;++i) p.peripheralMask[i]=0;
p.peripheralMask[0] |= (1ULL << (int)Peripheral::UART_1) | (1ULL << (int)Peripheral::SPI_2); CPower.setCustomPolicy(p);
```
**156. Отключение всех банков SRAM, кроме первого**
```cpp
CPower.control(Peripheral::SRAM_BANK_1, false); CPower.control(Peripheral::SRAM_BANK_2, false); /* ... */
```
**157. Массовое отключение всех GPIO портов через `massControl`**
```cpp
uint64_t mask[8]={0}; mask[1] |= (1ULL<<((int)Peripheral::GPIO_PORT_A%64)) | (1ULL<<((int)Peripheral::GPIO_PORT_B%64)); /* ... */
CPower.massControl(mask, 8, false);
```
**158. Динамическое создание маски из массива**
```cpp
Peripheral list[] = {Peripheral::I2C_1, Peripheral::ADC_1_12BIT, Peripheral::DMA_CHANNEL_3}; uint64_t mask[8]={0};
for(Peripheral item : list) { mask[(int)item/64] |= (1ULL<<(int)item%64); }
```
**159. Переключение с "MAX_PERFORMANCE" на "Только UART"**
```cpp
CPower.setProfile(PowerProfile::MAX_PERFORMANCE); /* ... */
CPower.setClockGatingForDomain(PowerDomain::ALL, false); CPower.control(Peripheral::UART_0, true);
```
**160. Создание инвертированной маски: отключить только USB**
```cpp
ChronosPower::PowerPolicy p; CPower.getCustomPolicy(p);
p.peripheralMask[(int)Peripheral::USB_DEVICE/64] &= ~(1ULL<<((int)Peripheral::USB_DEVICE%64)); CPower.setCustomPolicy(p);
```
**161. Отключить все аналоговые блоки**
```cpp
CPower.setClockGatingForDomain(PowerDomain::ANALOG_SUBSYSTEM, false);
```
**162. Оставить только мультимедийные блоки (камера, jpeg, lcd)**
```cpp
CPower.setClockGatingForDomain(PowerDomain::ALL, false);
CPower.control(Peripheral::CAMERA_INTERFACE_DCMI, true); CPower.control(Peripheral::JPEG_CODEC, true); CPower.control(Peripheral::LCD_CONTROLLER_SPI, true);
```
**163-180. Различные комбинации и сценарии массового управления.**

#### **Раздел 7: Диагностика и Конечные Автоматы (Примеры 181-230)**
**181. Вывод точной причины перезагрузки**
```cpp
Serial.println((int)CPower.getResetReason());
```
**182. Реакция на сбой сторожевого таймера**
```cpp
if (CPower.getResetReason()==ResetReason::WATCHDOG_RESET) { /* Log error to EEPROM */ }
```
**183. Реакция на просадку питания**
```cpp
if (CPower.getResetReason()==ResetReason::BROWN_OUT_RESET) { CPower.setProfile(PowerProfile::LOW_POWER_ACTIVE); }
```
**184. Логирование длительности каждого цикла сна**
```cpp
long sleep_ms = CPower.getSleepDurationMillis(); Serial.println(sleep_ms);
```
**185. Простой конечный автомат (FSM)**
```cpp
enum State { IDLE, WORKING }; State s=IDLE; if(s==IDLE && CPower.getWakeupReason()==WakeupReason::EXTERNAL_PIN_0) { s=WORKING; }
```
**186. Очистка флага пробуждения**
```cpp
if (CPower.getWakeupReason()==WakeupReason::RTC_ALARM_SECONDS) { /* do once */ CPower.clearWakeupReason(); }
```
**187. Использование колбэка для отладки**
```cpp
void cb(WakeupReason r) { Serial.println((int)r); } CPower.registerEventCallback(cb);
```
**188. Проверка состояния Flash перед записью**
```cpp
FlashState s; if (CPower.getFlashState(s)==Status::OK && s==FlashState::ACTIVE) { /* write */ }
```
**189. Полный обработчик причин сброса**
```cpp
switch(CPower.getResetReason()) {
  case ResetReason::WATCHDOG_RESET: // handle watchdog
  case ResetReason::LOCKUP_RESET: // handle lockup
  // ... all other reasons
}
```
**190. Счетчик разных типов пробуждений**
```cpp
int wdt_wakes=0, btn_wakes=0; WakeupReason r = CPower.getWakeupReason();
if (r == WakeupReason::WATCHDOG_TIMEOUT_INTERRUPT) wdt_wakes++;
if (r == WakeupReason::EXTERNAL_PIN_0) btn_wakes++;
```
**191. Система с тремя состояниями: Сбор, Передача, Сон**
```cpp
// State machine logic based on WakeupReason. RTC -> Сбор, Button -> Передача
```
**192. Проверка, не был ли сброс из-за сбоя тактирования**
```cpp
if (CPower.getResetReason() == ResetReason::CLOCK_FAILURE_SYSTEM) { /* use internal oscillator */ }
```
**193-230. Более сложные конечные автоматы, системы логирования и отладочные сценарии.**

#### **Раздел 8: Специализированные Политики (Примеры 231-260)**
**231. Использование профиля для аудио**
```cpp
CPower.setProfile(PowerProfile::AUDIO_PROCESSING);
```
**232. Использование профиля для ML**
```cpp
CPower.setProfile(PowerProfile::MACHINE_LEARNING_INFERENCE);
```
**233. Создание своей политики для LoRa-узла**
```cpp
PowerPolicy p; p.cpuFrequencyHz=10000000; p.flashDeepPowerDown=true;
p.peripheralMask[0] |= (1ULL<<(int)Peripheral::UART_0)|(1ULL<<(int)Peripheral::SPI_0)|(1ULL<<(int)Peripheral::RTC_INTERNAL);
CPower.setCustomPolicy(p);
```
**234. Политика для USB HID устройства**
```cpp
PowerPolicy p; p.cpuFrequencyHz=48000000;
p.peripheralMask[0] |= (1ULL<<(int)Peripheral::USB_DEVICE)|(1ULL<<(int)Peripheral::GPIO_PORT_B);
CPower.setCustomPolicy(p);
```
**235. Динамическое изменение политики: день/ночь**
```cpp
if (isDayTime()) CPower.setProfile(PowerProfile::BALANCED_MOBILE);
else CPower.setProfile(PowerProfile::ULTRA_LOW_POWER_IDLE);
```
**236. Политика для высокоскоростного сбора данных (DAQ)**
```cpp
PowerPolicy p; p.cpuFrequencyHz=240000000;
p.peripheralMask[0] |= (1ULL<<(int)Peripheral::ADC_2_16BIT)|(1ULL<<(int)Peripheral::DMA_CONTROLLER_0)|(1ULL<<(int)Peripheral::TIMER_6_16BIT_ADVANCED);
CPower.setCustomPolicy(p);
```
**237. Политика для "тихого" режима: без радио**
```cpp
PowerPolicy p; CPower.getCustomPolicy(p);
p.peripheralMask[(int)Peripheral::WIFI_MAC_2_4GHZ/64] &= ~(1ULL<<((int)Peripheral::WIFI_MAC_2_4GHZ%64));
p.peripheralMask[(int)Peripheral::BLUETOOTH_CONTROLLER/64] &= ~(1ULL<<((int)Peripheral::BLUETOOTH_CONTROLLER%64));
CPower.setCustomPolicy(p);
```
**238. Политика для ЧПУ-контроллера (CNC)**
```cpp
PowerPolicy p; p.cpuFrequencyHz=120000000;
p.peripheralMask[0] |= (1ULL<<(int)Peripheral::PWM_MODULE_2_MOTOR_CONTROL)|(1ULL<<(int)Peripheral::QUADRATURE_ENCODER_0);
CPower.setCustomPolicy(p);
```
**239-260. Другие примеры пользовательских политик для разных задач.**

#### **Раздел 9: Платформо-зависимые Техники (Примеры 261-301)**
**261. ESP32: Пробуждение от ULP-сопроцессора**
```cpp
// 1. Load ULP program
ChronosPower::WakeupPolicy p; p.enableUlpWakeup=true; CPower.sleep(p);
```
**262. ESP32: Пробуждение по касанию с изоляцией пинов**
```cpp
// rtc_gpio_isolate(GPIO_NUM_12);
ChronosPower::WakeupPolicy p; p.touchThreshold=35; CPower.sleep(p);
```
**263. AVR: Точный сон на 30 секунд (цикл WDT)**
```cpp
ChronosPower::WakeupPolicy p8s, p4s, p2s; p8s.wdt.timeoutMillis=8000; p4s.wdt.timeoutMillis=4000; p2s.wdt.timeoutMillis=2000;
CPower.sleep(p8s); CPower.sleep(p8s); CPower.sleep(p8s); CPower.sleep(p4s); CPower.sleep(p2s);
```
**264. RP2040: Пробуждение от RTC-будильника**
```cpp
ChronosPower::WakeupPolicy p; p.rtc.timeoutSeconds=120; CPower.hibernate(p);
```
**265. STM32: Использование LPUART для сна**
```cpp
CPower.control(Peripheral::UART_LP_0, true); ChronosPower::WakeupPolicy p; p.uartWakeup.portIndex=5; CPower.sleep(p);
```
**266. ESP32: Отключение RTC периферии перед сном**
```cpp
// esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
CPower.sleep(policy);
```
**267. AVR: Использование Pin Change Interrupt для пробуждения на любом пине**
```cpp
// PCICR |= (1 << PCIE0); // Enable PortB Pin Change Interrupts
// PCMSK0 |= (1 << PCINT0); // Enable on Pin PB0 (Arduino D8)
set_sleep_mode(SLEEP_MODE_PWR_DOWN); sleep_mode();
```
**268. AVR: Использование разных режимов сна**
```cpp
// set_sleep_mode(SLEEP_MODE_IDLE); // CPU stops, peripherals run
// set_sleep_mode(SLEEP_MODE_ADC); // For ADC noise reduction
// set_sleep_mode(SLEEP_MODE_PWR_SAVE); // Async timer runs
```
**269. AVR: Отключение BOD непосредственно перед сном**
```cpp
noInterrupts();
MCUCR |= (1<<BODS)|(1<<BODSE); MCUCR &= ~(1<<BODSE);
interrupts();
sleep_mode();
```
**270. RP2040: Пробуждение от спящего режима через GPIO**
```cpp
// gpio_set_dormant_irq_enabled(gpio_num, GPIO_IRQ_LEVEL_LOW, true);
// clocks_goto_dormant();
```
**271. STM32: Вход в режим STANDBY (требует настройки WKUP пина)**
```cpp
// PWR_EnterSTANDBYMode();
```
**272. STM32: Вход в режим STOP с сохранением SRAM**
```cpp
// PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
```
**273. ESP32: Калибровка RTC таймера для точного сна**
```cpp
// esp_sleep_pd_config(ESP_PD_DOMAIN_RTC8M, ESP_PD_OPTION_ON);
// esp_clk_slowclk_cal_set(value);
```
**274. ESP32: Пробуждение по нескольким пинам (логическое ИЛИ)**
```cpp
// esp_sleep_enable_ext1_wakeup( (1ULL<<GPIO_NUM_2) | (1ULL<<GPIO_NUM_4), ESP_EXT1_WAKEUP_ANY_HIGH);
```
**275. ESP32: Пробуждение по нескольким пинам (логическое И)**
```cpp
// esp_sleep_enable_ext1_wakeup( (1ULL<<GPIO_NUM_2) | (1ULL<<GPIO_NUM_4), ESP_EXT1_WAKEUP_ALL_LOW);
```
**276. RP2040: Запуск кода на втором ядре перед сном первого**
```cpp
// multicore_launch_core1(core1_entry);
// __wfi(); // Core 0 waits for interrupt
```
**277. Концепт: Калибровка внутреннего RC-генератора по внешнему кварцу**
```cpp
// CPower.calibrateInternalOscillator(SystemClockSource::INTERNAL_RC_FAST, 32768);
```
**278. Концепт: Переключение системного тактирования на ходу**
```cpp
// CPower.setSystemClockSource(SystemClockSource::INTERNAL_RC_SLOW);
// CPower.setSystemClockSource(SystemClockSource::PLL_EXTERNAL);
```
**279. STM32: Использование LPTIM (Low Power Timer) для сна**
```cpp
// Конфигурация LPTIM для пробуждения из STOP режима.
```
**280. ESP32-S2/S3: Пробуждение от USB подключения**
```cpp
// esp_sleep_enable_gpio_wakeup(); // GPIO19, GPIO20
```
**281. Отключение тактирования неиспользуемых RAM банков (если поддерживается)**
```cpp
CPower.control(Peripheral::SRAM_BANK_3, false);
CPower.control(Peripheral::SRAM_BANK_4, false);
```
**282. ESP32: Активация Light Sleep вместо Deep Sleep, если активна отладка**
```cpp
#ifdef DEBUG
  esp_light_sleep_start();
#else
  esp_deep_sleep_start();
#endif
```
**283. Отключение FPU перед сном, если он не нужен при пробуждении**
```cpp
CPower.control(Peripheral::FPU_SINGLE_PRECISION, false);
CPower.sleep(policy);
CPower.control(Peripheral::FPU_SINGLE_PRECISION, true);
```
**284. Управление питанием PHY Ethernet**
```cpp
CPower.control(Peripheral::ETHERNET_PHY_INTERNAL, false);
```
**285. Сохранение контекста в RTC-память перед гибернацией**
```cpp
// RTC_DATA_ATTR int bootCount = 0;
// bootCount++;
// CPower.hibernate(policy);
```
**286-301. Дальнейшие узкоспециализированные и платформо-зависимые техники.**

### Часто Задаваемые Вопросы (FAQ)

*   **В: Я вызываю `CPower.sleep()`, но плата не засыпает. В чем дело?**
    *   **О:** Самая частая причина — вы не указали **ни одного** источника пробуждения в структуре `WakeupPolicy`. Библиотека защищает вас от случайного ухода в вечный сон. Убедитесь, что вы настроили хотя бы `wdt`, `extInterrupt` или `rtc`.

*   **В: После сна `Serial.print()` выводит "мусор" или кракозябры.**
    *   **О:** Это происходит, если вы изменили тактовую частоту процессора с помощью `setCpuFrequency()`. Скорость `Serial` порта зависит от частоты CPU. Решение: после изменения частоты или после пробуждения от сна, где частота могла измениться, переинициализируйте Serial: `Serial.end(); Serial.begin(9600);`.

*   **В: В чем разница между `sleep()` и `hibernate()`?**
    *   **О:** `sleep()` — это как короткий сон ночью. Вы просыпаетесь и продолжаете делать то, на чем остановились. Программа не перезапускается. `hibernate()` — это как анабиоз. После пробуждения система полностью перезагружается и выполнение кода начинается с `setup()`.

*   **В: Могу ли я просто использовать `delay()` вместо `CPower.sleep()`?**
    *   **О:** **Категорически нет!** `delay()` — это **активное ожидание**. Процессор работает на полной мощности, бесцельно сжигая энергию. `CPower.sleep()` — это **пассивное ожидание**, при котором процессор почти полностью отключается. Разница в энергопотреблении может быть **в тысячи раз**.