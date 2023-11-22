# Проект Тераграф. Пример 1. Приемо-передача данных между хост-подсистемой и sw_kernel

## Общее описание

Пример демонстрирует основные механизмы инициализации гетерогенных ядер gpc и взаимодействие хост-подсистем с Graph Processor Core, используются аппаратные очереди.

## Установка

Для установки требуется рекурсивно клонировать репозиторий:

```bash
git clone --recursive git@latex.bmstu.ru:hackathon2023/lab1/lab1.git
cd lab1
```

## Зависимости

Зависимости для сборки проекта:

* набор средст сборки [riscv toolchain](https://gitlab.com/quantr/toolchain/riscv-gnu-toolchain) и экспорт исполняемых файлов в `PATH`

* набор библиотек [picolib](https://github.com/picolibc/picolibc) и экспорт в `C_INCLUDE_PATH`


Для стандартного пользователя ВМ студенческой команды хакатона все необходимые переменные окружения установлены по-умолчанию.

## Сборка проекта

Следует выполнить команду:

```bash
make
```

Результатом выполнения команды станет файлы host_main, sw_kernel_main.rawbinary в директориях ./host/ и ./sw_kernel/.

| :exclamation:  Не забывайте синхронизировать тексты исходного текста host и kernel составляющих проекта |
|---------------------------------------------------------------------------------------------------------|

## Запуск проекта

Параметры запуска проекта:

host_main <символькое устройство gpc> <путь к файлу sw_kernel>

Например:

```
host/host_main /dev/gpc0 sw_kernel/sw_kernel_main.rawbinary
```

## Запуск тестового скрипта

Возможно запустить комплексный тест всех имеющихя в системе ядер gpc помощью скрипта 

```bash
cd host
./test.sh
```

Результат работы теста:


```bash
Стартуем обработчик на /dev/gpc1
Open gpc on /dev/gpc1
Rawbinary loaded from ../sw-kernel/sw_kernel.rawbinary
sw_kernel version: 0x20232109
Leonhard clock frequency (LNH_CF) 190.091429 MHz
Тест приема-передачи 117 млн. 64-х битных слов
Test done
Стартуем обработчик на /dev/gpc12
Open gpc on /dev/gpc12
Rawbinary loaded from ../sw-kernel/sw_kernel.rawbinary
sw_kernel version: 0x20232109
Leonhard clock frequency (LNH_CF) 190.113160 MHz
...
```

## Очистка проекта

Следует выполнить команду:

```bash
make clean
```
