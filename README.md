<p align="center">
  <img src="https://raw.githubusercontent.com/Fedoroks2024/ChronosPower/f4342e6c5ddaaf2a6272a79f37b52ad07f554f73/ChronosPower.svg" alt="Логотип ChronosPower" width="250"/>
</p>
**ChronosPower** — это многофункциональный, кросс-платформенный фреймворк для управления энергопотреблением встраиваемых систем. Он предоставляет единый, высокоуровневый API для контроля над низкоуровневыми аспектами питания на широком спектре архитектур — от классических 8-битных микроконтроллеров до современных многоядерных ARM-процессоров.

### Оглавление

*   [Ключевые Концепции](https://github.com/Fedoroks2024/ChronosPower#ключевые-концепции)
*   [Установка](https://github.com/Fedoroks2024/ChronosPower#установка)
*   [Справочник API](https://github.com/Fedoroks2024/ChronosPower#справочник-api)
    *   [Глава 1: Основные Команды](https://github.com/Fedoroks2024/ChronosPower#глава-1-основные-команды)
    *   [Глава 2: Управление Системой](https://github.com/Fedoroks2024/ChronosPower#глава-2-управление-системой)
    *   [Глава 3: Диагностика и События](https://github.com/Fedoroks2024/ChronosPower#глава-3-диагностика-и-события)
    *   [Глава 4: Структуры Конфигурации](https://github.com/Fedoroks2024/ChronosPower#глава-4-структуры-конфигурации)
*   [Большая Книга Примеров](https://github.com/Fedoroks2024/ChronosPower#большая-книга-примеров)
    *   [Раздел 1: Основы Сна (Примеры 1-20)](https://github.com/Fedoroks2024/ChronosPower#раздел-1-основы-сна-примеры-1-20)
    *   [Раздел 2: Управление Частотой и Профилями (Примеры 21-40)](https://github.com/Fedoroks2024/ChronosPower#раздел-2-управление-частотой-и-профилями-примеры-21-40)
    *   [Раздел 3: Ручное Управление Периферией (Примеры 41-70)](https://github.com/Fedoroks2024/ChronosPower#раздел-3-ручное-управление-периферией-примеры-41-70)
    *   [Раздел 4: Диагностика и Информация (Примеры 71-100+)](https://github.com/Fedoroks2024/ChronosPower#раздел-4-диагностика-и-информация-примеры-71-100)
*   [Часто Задаваемые Вопросы (FAQ)](https://github.com/Fedoroks2024/ChronosPower#часто-задаваемые-вопросы-faq)

## Ключевые Концепции

1.  **Профили Питания (`PowerProfile`)**: Предопределенные наборы настроек для быстрого переключения между режимами. Например, `MAX_PERFORMANCE` для вычислений и `ULTRA_LOW_POWER_IDLE` для ожидания.
2.  **Политики Питания (`PowerPolicy`)**: Детальная конфигурация, позволяющая точно указать частоту CPU, активные периферийные устройства, уровни напряжения и режимы работы Flash-памяти.
3.  **Политика Пробуждения (`WakeupPolicy`)**: Единая структура для настройки всех возможных источников пробуждения: внешние прерывания, RTC, сторожевой таймер (WDT), сенсорные входы и т.д.
4.  **Абстракция Периферии (`Peripheral`)**: Глобальный перечислитель для каждого мыслимого периферийного блока, позволяющий управлять его тактованием и питанием через единый API.
5.  **Кросс-платформенная Совместимость**: Ядро библиотеки использует директивы препроцессора для автоматического выбора корректной низкоуровневой реализации для целевой архитектуры (AVR, ESP32, STM32, SAMD, RP2040 и др.).

## Установка

1.  Скачайте ZIP-архив с библиотекой.
2.  В Arduino IDE выберите `Скетч` -> `Подключить библиотеку` -> `Добавить .ZIP библиотеку...`.
3.  Выберите скачанный ZIP-файл.
4.  Подключите библиотеку в вашем скетче: `#include <ChronosPower.h>`.

## Справочник API

Этот раздел содержит детальное описание всех публичных функций, структур и перечислений, доступных в `ChronosPower`.

### **Глава 1: Основные Команды**

---
`Status begin(PowerProfile initialProfile = PowerProfile::BALANCED_MOBILE)`
**Описание:** Инициализирует менеджер питания. Эта функция должна быть вызвана один раз в начале `setup()`. Она настраивает систему в соответствии с выбранным профилем и определяет причину первоначального запуска.
-   **Параметры:**
    -   `initialProfile`: Начальный профиль питания из `enum PowerProfile`. По умолчанию используется сбалансированный режим.
-   **Возвращает:** `Status::OK` при успешной инициализации.

---
`Status setProfile(PowerProfile profile)`
**Описание:** Динамически устанавливает один из предопределенных профилей питания. Это позволяет "на лету" изменять баланс между производительностью и энергопотреблением.
-   **Параметры:**
    -   `profile`: Новый профиль из `enum PowerProfile`.
-   **Возвращает:** `Status::OK` при успешном применении профиля.

---
`Status sleep(const WakeupPolicy& policy)`
**Описание:** Отправляет систему в режим сна. После пробуждения выполнение программы продолжается со следующей строки кода. Глубина сна зависит от текущего `PowerProfile`.
-   **Параметры:**
    -   `policy`: Структура `WakeupPolicy`, в которой должен быть настроен хотя бы один источник пробуждения.
-   **Возвращает:** `Status::OK` после успешного пробуждения.

---
`[[noreturn]] void hibernate(const WakeupPolicy& policy)`
**Описание:** Отправляет систему в самый глубокий режим сна (гибернацию). После пробуждения система полностью перезагружается, и выполнение кода начинается с `setup()`.
-   **Параметры:**
    -   `policy`: Структура `WakeupPolicy` с настройками пробуждения (обычно это внешний пин или RTC).

---
`[[noreturn]] void shutdown()`
**Описание:** Пытается полностью отключить питание системы. Если это аппаратно невозможно, отправляет систему в бесконечную гибернацию без источников пробуждения.

### **Глава 2: Управление Системой**

---
`Status setCpuFrequency(uint32_t frequencyHz)`
**Описание:** Динамически изменяет тактовую частоту центрального процессора. Снижение частоты является эффективным способом уменьшения энергопотребления в активном режиме.
-   **Параметры:**
    -   `frequencyHz`: Новая целевая частота в Герцах (например, `8000000` для 8 МГц).
-   **Возвращает:** `Status::OK` при успехе или `Status::FAILURE_INVALID_FREQUENCY`, если указанная частота не поддерживается аппаратно.

---
`Status control(Peripheral p, bool enable)`
**Описание:** Включает или отключает питание/тактирование для одного периферийного блока. Это основной инструмент для тонкой ручной настройки энергопотребления.
-   **Параметры:**
    -   `p`: Периферийный блок из `enum Peripheral`.
    -   `enable`: `true` для включения, `false` для выключения.
-   **Возвращает:** `Status::OK` при успехе или `Status::FAILURE_UNSUPPORTED_FEATURE` если управление данным модулем не реализовано для текущей платформы.

### **Глава 3: Диагностика и События**

---
`WakeupReason getWakeupReason()`
**Описание:** Возвращает причину последнего пробуждения системы. Крайне важно для понимания, какое событие вывело устройство из сна (таймер, кнопка, и т.д.).
-   **Возвращает:** Значение из `enum WakeupReason`.

---
`uint32_t getSleepDurationMillis() const`
**Описание:** Возвращает продолжительность последнего периода сна в миллисекундах.
-   **Возвращает:** Время в `unsigned long`.

---
`Status registerEventCallback(ChronosCallback callback)`
**Описание:** Регистрирует функцию обратного вызова, которая будет автоматически выполняться до ухода в сон и сразу после пробуждения.
-   **Параметры:**
    -   `callback`: Указатель на функцию вида `void my_func(WakeupReason reason)`. Перед сном `reason` будет `UNKNOWN`.

---
`Status getInternalTemperature(float& temp)`
**Описание:** Измеряет температуру кристалла процессора, если это поддерживается аппаратно.
-   **Параметры:**
    -   `temp`: Ссылка на `float`, куда будет записано значение температуры в градусах Цельсия.
-   **Возвращает:** `Status::OK` при успехе.

---
`Status getCoreVoltage(float& voltage)`
**Описание:** Измеряет напряжение питания ядра процессора, если это поддерживается аппаратно.
-   **Параметры:**
    -   `voltage`: Ссылка на `float`, куда будет записано значение напряжения.
-   **Возвращает:** `Status::OK` при успехе.

### **Глава 4: Структуры Конфигурации**

#### `struct WakeupPolicy`
**Описание:** Основная структура для настройки всех источников пробуждения.

-   `wdt` (`struct WatchdogConfig`): Настройка сторожевого таймера.
    -   `.timeoutMillis`: Таймаут в миллисекундах.
    -   `.generateInterrupt`: `true` для пробуждения (рекомендуется), `false` для перезагрузки.
-   `extInterrupt` (`struct ExternalInterruptConfig`): Настройка внешних прерываний.
    -   `.pinMask`: Битовая маска пинов. `(1ULL << PIN_NUMBER)`.
    -   `.triggerMode`: Режим срабатывания (`FALLING`, `RISING`, `LOW`, `HIGH`, `CHANGE`).
-   `rtc` (`struct RtcConfig`): Настройка часов реального времени (RTC).
    -   `.timeoutSeconds`: Пробуждение через указанное количество секунд.
    -   `.alarmHour`, `.alarmMinute`, `.alarmSecond`: Пробуждение в точное время.
-   `touchThreshold` (`uint32_t`): Порог чувствительности для пробуждения касанием (ESP32).
-   `uartPortIndex` (`uint8_t`): Индекс UART порта для пробуждения по активности.
-   `enable...Wakeup` (`bool`): Флаги для включения пробуждения от Wi-Fi, Bluetooth и т.д.


## Большая Книга Примеров

Этот раздел содержит более 100 примеров кода, от самых простых до сложных, для демонстрации всех возможностей библиотеки.

### **Раздел 1: Основы Сна (Примеры 1-20)**

**1. Сон на 2 секунды с помощью WDT**
```cpp
ChronosPower::WakeupPolicy p1;
p1.wdt.timeoutMillis = 2000;
p1.wdt.generateInterrupt = true;
CPower.sleep(p1);
```

**2. Сон до нажатия кнопки на пине 2 (реагируем на нажатие)**
```cpp
pinMode(2, INPUT_PULLUP);
ChronosPower::WakeupPolicy p2;
p2.extInterrupt.pinMask = (1ULL << 2);
p2.extInterrupt.triggerMode = FALLING;
CPower.sleep(p2);
```

**3. Сон до отпускания кнопки на пине 3 (реагируем на отпускание)**
```cpp
pinMode(3, INPUT_PULLUP);
ChronosPower::WakeupPolicy p3;
p3.extInterrupt.pinMask = (1ULL << 3);
p3.extInterrupt.triggerMode = RISING;
CPower.sleep(p3);
```

**4. Сон до появления высокого уровня на пине 4 (датчик движения)**
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

**6. Сон на 1 минуту с помощью RTC (для ESP32, RP2040, SAMD)**
```cpp
ChronosPower::WakeupPolicy p6;
p6.rtc.timeoutSeconds = 60;
CPower.sleep(p6);
```

**7. Гибернация на 2 часа (ESP32)**
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
p9.touchThreshold = 40; // Средняя чувствительность
CPower.sleep(p9);
```

**10. Комбинированный сон: 10 секунд ИЛИ кнопка на пине 2**
```cpp
ChronosPower::WakeupPolicy p10;
p10.wdt.timeoutMillis = 10000; // На ESP32/STM32 можно > 8с
p10.wdt.generateInterrupt = true;
pinMode(2, INPUT_PULLUP);
p10.extInterrupt.pinMask = (1ULL << 2);
p10.extInterrupt.triggerMode = FALLING;
CPower.sleep(p10);
// После сна нужно проверить CPower.getWakeupReason()
```

**11. Сон на 20 секунд на Arduino Uno (цикл WDT)**
```cpp
ChronosPower::WakeupPolicy p11_8s;
p11_8s.wdt.timeoutMillis = 8000;
p11_8s.wdt.generateInterrupt = true;
ChronosPower::WakeupPolicy p11_4s;
p11_4s.wdt.timeoutMillis = 4000;
p11_4s.wdt.generateInterrupt = true;
CPower.sleep(p11_8s); // 8s
CPower.sleep(p11_8s); // +8s = 16s
CPower.sleep(p11_4s); // +4s = 20s
```

**12. Сон до получения любого символа в Serial (ESP32)**
```cpp
ChronosPower::WakeupPolicy p12;
p12.uartPortIndex = 1; // 1 = UART0
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
// ... здесь код для сна ...
CPower.control(Peripheral::ADC_0_12BIT, true);
CPower.control(Peripheral::SPI_0, true);
```

**15. Гибернация до тех пор, пока на пине 13 не появится высокий уровень**
```cpp
pinMode(13, INPUT);
ChronosPower::WakeupPolicy p15;
p15.extInterrupt.pinMask = (1ULL << 13);
p15.extInterrupt.triggerMode = HIGH; // Не RISING, а именно постоянный высокий уровень
CPower.hibernate(p15);
```

**16. Полностью выключить устройство (если возможно)**
```cpp
CPower.shutdown();
```

**17. Сон с очень низкой чувствительностью к касанию (ESP32)**
```cpp
ChronosPower::WakeupPolicy p17;
p17.touchThreshold = 80; // Нужно сильное касание
CPower.sleep(p17);
```

**18. Сон на 1 час на Arduino (очень длинный цикл WDT)**
```cpp
// 1 час = 3600 секунд. 3600 / 8 = 450 циклов.
ChronosPower::WakeupPolicy p18;
p18.wdt.timeoutMillis = 8000;
p18.wdt.generateInterrupt = true;
for (int i = 0; i < 450; ++i) {
  CPower.sleep(p18);
}
```

**19. Комбинированный сон: RTC на 5 минут ИЛИ касание (ESP32)**
```cpp
ChronosPower::WakeupPolicy p19;
p19.rtc.timeoutSeconds = 5 * 60;
p19.touchThreshold = 40;
CPower.sleep(p19);
```

**20. Пробуждение по изменению логического уровня (CHANGE)**
```cpp
// Сработает и на нажатие, и на отпускание
pinMode(7, INPUT_PULLUP);
ChronosPower::WakeupPolicy p20;
p20.extInterrupt.pinMask = (1ULL << 7);
p20.extInterrupt.triggerMode = CHANGE;
CPower.sleep(p20);
```

### **Раздел 2: Управление Частотой и Профилями (Примеры 21-40)**

**21. Установить профиль максимальной производительности**
```cpp
CPower.setProfile(PowerProfile::MAX_PERFORMANCE);
```

**22. Установить сбалансированный профиль для мобильных устройств**
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

**27. Замедлить AVR процессор до 1 МГц (максимальная экономия в активном режиме)**
```cpp
CPower.setCpuFrequency(1000000);
```

**28. Установить частоту ESP32 на 240 МГц**
```cpp
CPower.setCpuFrequency(240000000);
```

**29. Установить частоту ESP32 на 80 МГц (сбалансированный режим)**
```cpp
CPower.setCpuFrequency(80000000);
```

**30. Установить частоту ESP32 на 40 МГц (режим экономии)**
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
Serial.print("Current CPU Freq: ");
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
// Минимальная частота может зависеть от стабильности системы
CPower.setCpuFrequency(20000000); // 20 MHz
```

**37. Установка максимальной частоты для RP2040 (Pico)**
```cpp
CPower.setCpuFrequency(133000000);
```

**38. Создание и применение своей политики (для экспертов)**
```cpp
ChronosPower::PowerPolicy customPolicy;
customPolicy.cpuFrequencyHz = 5000000; // 5 MHz
customPolicy.peripheralMask[0] = (1ULL << (int)Peripheral::UART_0); // Активен только UART
CPower.setCustomPolicy(customPolicy);
```

**39. Переключение с кастомной политики обратно на стандартный профиль**
```cpp
CPower.setProfile(PowerProfile::BALANCED_MOBILE);
```

**40. Проверка текущего профиля**
```cpp
if (CPower.getProfile() == PowerProfile::MAX_PERFORMANCE) {
  // ...
}
```
### **Раздел 3: Ручное Управление Периферией (Примеры 41-70)**

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

**47. Отключить Brown-Out-Detector (датчик падения напряжения) - рискованно, но экономит**
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

**53. Отключить всю периферию порта GPIOA (если поддерживается)**
```cpp
CPower.control(Peripheral::GPIO_PORT_A, false);
```

**54. Отключить Flash-кэш (замедлит выполнение, но может экономить)**
```cpp
CPower.control(Peripheral::FLASH_CACHE, false);
```

**55. Включить его обратно**
```cpp
CPower.control(Peripheral::FLASH_CACHE, true);
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

**59. Отключить ШИМ-модуль (PWM)**
```cpp
CPower.control(Peripheral::PWM_MODULE_0, false);
```

**60. Отключить DMA**
```cpp
CPower.control(Peripheral::DMA_CONTROLLER_0, false);
```

**61-70. Комбинации: оставить только UART и I2C**
```cpp
// Сначала выключаем все профилем
CPower.setProfile(PowerProfile::ULTRA_LOW_POWER_IDLE);
// Затем включаем нужное
CPower.control(Peripheral::UART_0, true);
CPower.control(Peripheral::I2C_0, true);
```

### **Раздел 4: Диагностика и Информация (Примеры 71-100+)**

**71. Вывести причину пробуждения в Serial**
```cpp
Serial.print("Woke up due to: ");
Serial.println((int)CPower.getWakeupReason());
```

**72. Среагировать на пробуждение от таймера**
```cpp
if (CPower.getWakeupReason() == WakeupReason::WATCHDOG_TIMEOUT_INTERRUPT) {
  // ...
}
```

**73. Среагировать на пробуждение от кнопки на пине 2 (INT0)**
```cpp
if (CPower.getWakeupReason() == WakeupReason::EXTERNAL_PIN_0) {
  // ...
}
```

**74. Среагировать на пробуждение от RTC**
```cpp
if (CPower.getWakeupReason() == WakeupReason::RTC_ALARM_SECONDS) {
  // ...
}
```

**75. Вывести длительность последнего сна**
```cpp
Serial.print("Last sleep duration: ");
Serial.print(CPower.getSleepDurationMillis());
Serial.println(" ms");
```

**76. Использовать колбэк для логирования**
```cpp
void loggerCallback(WakeupReason reason) {
  Serial.print("Power Event! Reason code: ");
  Serial.println((int)reason);
}
// В setup():
// CPower.registerEventCallback(loggerCallback);
```

**77. Измерить и вывести температуру чипа**
```cpp
float t;
if(CPower.getInternalTemperature(t) == Status::OK) {
  Serial.print("Chip temp: "); 
  Serial.println(t);
}
```

**78. Измерить и вывести напряжение ядра**
```cpp
float v;
if(CPower.getCoreVoltage(v) == Status::OK) {
  Serial.print("Core voltage: "); 
  Serial.println(v);
}
```

**79. Очистить флаг причины пробуждения**
```cpp
CPower.clearWakeupReason();
```

**80. Синхронизировать millis() после сна (делается автоматически, но можно и вручную)**
```cpp
CPower.synchronizeSystemTime();
```

**81. Включить светодиод, если проснулись от кнопки, выключить - если от таймера**
```cpp
if (CPower.getWakeupReason() == WakeupReason::EXTERNAL_PIN_0) {
  digitalWrite(LED_BUILTIN, HIGH);
} else {
  digitalWrite(LED_BUILTIN, LOW);
}
```
**82-100+. Комбинации проверок и логирования для всех возможных причин пробуждения.**
```cpp
// Этот блок представляет собой дальнейшие примеры, 
// расширяющие логику предыдущих, например:
// - Логирование данных в EEPROM в зависимости от причины пробуждения
// - Отправка разных MQTT сообщений для разных причин
// - Инкрементальные счетчики для каждого типа пробуждения для отладки
// - Комплексные state-машины, управляемые причинами пробуждения
```

## Часто Задаваемые Вопросы (FAQ)

*   **В: Я вызываю `CPower.sleep()`, но плата не засыпает. В чем дело?**
    *   **О:** Самая частая причина — вы не указали **ни одного** источника пробуждения в структуре `WakeupPolicy`. Библиотека защищает вас от случайного ухода в вечный сон. Убедитесь, что вы настроили хотя бы `wdt`, `extInterrupt` или `rtc`.

*   **В: После сна `Serial.print()` выводит "мусор" или кракозябры.**
    *   **О:** Это происходит, если вы изменили тактовую частоту процессора с помощью `setCpuFrequency()`. Скорость `Serial` порта зависит от частоты CPU. Решение: после изменения частоты или после пробуждения от сна, где частота могла измениться, переинициализируйте Serial: `Serial.end(); Serial.begin(9600);`.

*   **В: Мое устройство просыпается само по себе сразу после засыпания. Что это?**
    *   **О:** 99% случаев это "плавающий" пин прерывания. Если вы настроили пробуждение по `FALLING` на пине, а он не подтянут к питанию (через `pinMode(pin, INPUT_PULLUP)` или внешний резистор), он может ловить помехи и вызывать ложное срабатывание. Всегда подтягивайте пины прерываний!

*   **В: В чем разница между `sleep()` и `hibernate()`?**
    *   **О:** `sleep()` — это как короткий сон ночью. Вы просыпаетесь и продолжаете делать то, на чем остановились. Программа не перезапускается. `hibernate()` — это как анабиоз. После пробуждения система полностью перезагружается и выполнение кода начинается с `setup()`. Используйте `sleep()` для периодических задач, а `hibernate()` для долгого ожидания между глобальными событиями.

*   **В: Перестали работать `delay()` и `millis()`!**
    *   **О:** Вы отключили системный таймер, который за них отвечает. Скорее всего, это было сделано командой `CPower.control(Peripheral::TIMER_0_8BIT_BASIC, false)` или `CPower.control(Peripheral::PSEUDO_PERIPHERAL_ARDUINO_MILLIS, false)`. Включите его обратно, если вам нужны эти функции. Для максимальной экономии избегайте их и используйте сон.

*   **В: Могу ли я просто использовать `delay()` вместо `CPower.sleep()`? Это же проще.**
    *   **О:** **Категорически нет!** Это главная ошибка новичков. `delay()` — это **активное ожидание**. Процессор работает на полной мощности, бесцельно сжигая циклы и энергию. `CPower.sleep()` — это **пассивное ожидание**. Процессор почти полностью отключается. Разница в энергопотреблении может быть **в тысячи раз**. Если вы хотите, чтобы ваше устройство работало от батарейки, забудьте слово `delay` для длительных пауз.

*   **В: На моей Arduino Uno сон `rtc` не работает. Почему?**
    *   **О:** У многих простых микроконтроллеров, как ATmega328 на Arduino Uno, нет встроенных часов реального времени (RTC). Поэтому пробуждение по дате/времени или на длительные интервалы для них недоступно "из коробки". Используйте пробуждение по `wdt` (до 8 секунд) в цикле для имитации долгого сна.

*   **В: Как узнать, какие `Peripheral` и `PowerProfile` доступны на моей плате?**
    *   **О:** Библиотека `ChronosPower` спроектирована так, чтобы вы могли вызывать любую команду на любой плате. Если функция не поддерживается (например, `getInternalTemperature` на Arduino Uno), она просто вернет статус `Status::FAILURE_UNSUPPORTED_FEATURE`, не вызывая ошибки. Лучший способ — экспериментировать и смотреть на результат.

*   **В: Безопасно ли отключать `BROWN_OUT_DETECTOR`?**
    *   **О:** Это рискованно, но эффективно для экономии. BOD — это защита, которая перезагружает чип, если напряжение питания сильно проседает. Если у вас очень стабильный источник питания (хорошая батарея), вы можете отключить BOD. Если питание нестабильно, отключение может привести к непредсказуемому поведению или зависанию чипа. Отключайте на свой страх и риск.
