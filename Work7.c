#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <direct.h>
#include <locale.h>

#define MAX_UNIT_LENGTH 10
#define MAX_LINE_LENGTH 100

// 
// Структуры данных
// 

typedef struct {
    double value;                // Значение измерения
    char unit[MAX_UNIT_LENGTH];  // Единица измерения
    double value_in_meters;      // Значение в метрах (для сравнения)
} Measurement;

// 
// Вспомогательные функции
// 

void print_separator(const char* title) {
    printf("\n");
    for (int i = 0; i < 60; i++) printf("+");
    if (title) {
        printf("\n+ %-56s +\n", title);
        for (int i = 0; i < 60; i++) printf("+");
    }
    printf("\n");
}

// Функция для конвертации в метры
double convert_to_meters(double value, const char* unit) {
    if (strcmp(unit, "m") == 0) return value;           // метры
    if (strcmp(unit, "cm") == 0) return value / 100.0;  // сантиметры
    if (strcmp(unit, "ya") == 0) return value * 0.9144; // ярды
    if (strcmp(unit, "ft") == 0) return value * 0.3048; // футы
    if (strcmp(unit, "lok") == 0) return value * 0.445; // локоть (приблизительно)
    if (strcmp(unit, "ver") == 0) return value * 0.04445; // вершок (приблизительно)
    if (strcmp(unit, "arsh") == 0) return value * 0.7112; // аршин (приблизительно)
    if (strcmp(unit, "sazh") == 0) return value * 2.1336; // сажень (приблизительно)

    // Если единица измерения неизвестна, считаем что это метры
    printf("⚠  Неизвестная единица измерения: '%s', считаем как метры\n", unit);
    return value;
}

// Функция для получения полного названия единицы измерения
const char* get_unit_full_name(const char* unit) {
    if (strcmp(unit, "m") == 0) return "метры";
    if (strcmp(unit, "cm") == 0) return "сантиметры";
    if (strcmp(unit, "ya") == 0) return "ярды";
    if (strcmp(unit, "ft") == 0) return "футы";
    if (strcmp(unit, "lok") == 0) return "локти";
    if (strcmp(unit, "ver") == 0) return "вершки";
    if (strcmp(unit, "arsh") == 0) return "аршины";
    if (strcmp(unit, "sazh") == 0) return "сажени";
    return "неизвестная единица";
}

// Функция для очистки строки от пробелов
char* trim_whitespace(char* str) {
    if (!str) return NULL;

    // Удаляем пробелы в начале
    while (isspace((unsigned char)*str)) str++;

    if (*str == '\0') return str;

    // Удаляем пробелы в конце
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';
    return str;
}

// Функция сравнения для сортировки (по возрастанию)
int compare_measurements_asc(const void* a, const void* b) {
    const Measurement* ma = (const Measurement*)a;
    const Measurement* mb = (const Measurement*)b;

    if (ma->value_in_meters < mb->value_in_meters) return -1;
    if (ma->value_in_meters > mb->value_in_meters) return 1;
    return 0;
}

// Функция сравнения для сортировки (по убыванию)
int compare_measurements_desc(const void* a, const void* b) {
    const Measurement* ma = (const Measurement*)a;
    const Measurement* mb = (const Measurement*)b;

    if (ma->value_in_meters > mb->value_in_meters) return -1;
    if (ma->value_in_meters < mb->value_in_meters) return 1;
    return 0;
}

// Функция для проверки валидности единицы измерения
int is_valid_unit(const char* unit) {
    const char* valid_units[] = { "m", "cm", "ya", "ft", "lok", "ver", "arsh", "sazh", NULL };

    for (int i = 0; valid_units[i] != NULL; i++) {
        if (strcmp(unit, valid_units[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// 
// Функции работы с файлами
// 

// Чтение измерений из файла
Measurement* read_measurements(const char* filename, int* count, int* invalid_lines) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    Measurement* measurements = NULL;
    int capacity = 10;
    int read_count = 0;
    *invalid_lines = 0;
    char line[MAX_LINE_LENGTH];
    int line_number = 0;

    measurements = (Measurement *)malloc(capacity * sizeof(Measurement));
    if (!measurements) {
        fclose(file);
        return NULL;
    }

    printf("Чтение файла...\n");

    while (fgets(line, sizeof(line), file)) {
        line_number++;

        // Удаляем символ новой строки
        line[strcspn(line, "\n")] = '\0';

        // Очищаем от пробелов
        char* trimmed = trim_whitespace(line);

        // Пропускаем пустые строки
        if (strlen(trimmed) == 0) {
            printf("  Строка %d: ПУСТАЯ - пропущена\n", line_number);
            continue;
        }

        // Парсим строку: значение и единица измерения
        double value;
        char unit[MAX_UNIT_LENGTH];

        if (sscanf(trimmed, "%lf %9s", &value, unit) == 2) {
            // Проверяем валидность единицы измерения
            if (!is_valid_unit(unit)) {
                printf("  Строка %d: НЕВАЛИДНАЯ единица '%s' - пропущена\n",
                    line_number, unit);
                (*invalid_lines)++;
                continue;
            }

            // Проверяем, нужно ли увеличить массив
            if (read_count >= capacity) {
                capacity *= 2;
                Measurement* temp = (Measurement*)realloc(measurements, capacity * sizeof(Measurement));
                if (!temp) {
                    free(measurements);
                    fclose(file);
                    return NULL;
                }
                measurements = temp;
            }
            else {
                printf("  Строка %d: НЕВЕРНЫЙ ФОРМАТ '%s' - пропущена\n",
                    line_number, trimmed);
                (*invalid_lines)++;
                return 0;
            }

            // Сохраняем измерение
            measurements[read_count].value = value;
            strncpy(measurements[read_count].unit, unit, MAX_UNIT_LENGTH - 1);
            measurements[read_count].unit[MAX_UNIT_LENGTH - 1] = '\0';
            measurements[read_count].value_in_meters = convert_to_meters(value, unit);

            printf("  Строка %d: %.6f %s (%s) ≈ %.6f м\n",
                line_number,
                value,
                unit,
                get_unit_full_name(unit),
                measurements[read_count].value_in_meters);

            read_count++;
        }
    }

    fclose(file);
    *count = read_count;
    return measurements;
}

// Запись измерений в файл
int write_measurements(const char* filename, const Measurement* measurements, int count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        return 0;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%.6f %s\n", measurements[i].value, measurements[i].unit);
    }

    fclose(file);
    return 1;
}

// 
// Основная функция
// 

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, ("RU"));
    print_separator("СОРТИРОВКА ИЗМЕРЕНИЙ С РАЗНЫМИ ЕДИНИЦАМИ");
    printf("Поддерживаемые единицы измерения:\n");
    printf("  m    - метры\n");
    printf("  cm   - сантиметры\n");
    printf("  ya   - ярды\n");
    printf("  ft   - футы\n");
    printf("  lok  - локти\n");
    printf("  ver  - вершки\n");
    printf("  arsh - аршины\n");
    printf("  sazh - сажени\n");


    // 
    // ПРОВЕРКА АРГУМЕНТОВ КОМАНДНОЙ СТРОКИ
    // 
    print_separator("ПРОВЕРКА АРГУМЕНТОВ");

    char filename[256] = "Readmy.txt";  // имя файла по умолчанию

    if (argc > 1) {
        strncpy(filename, argv[1], sizeof(filename) - 1);
        filename[sizeof(filename) - 1] = '\0';
        printf("✓ Используется файл из аргумента: '%s'\n", filename);
    }
    else {
        printf("✓ Используется файл по умолчанию: '%s'\n", filename);
    }

    // 
    // ВЫБОР РЕЖИМА СОРТИРОВКИ
    // 
    print_separator("ВЫБОР РЕЖИМА СОРТИРОВКИ");

    int sort_mode = 1; // 1 - по возрастанию, 2 - по убыванию

    printf("Выберите режим сортировки:\n");
    printf("1. По возрастанию (от меньшей длины к большей)\n");
    printf("2. По убыванию (от большей длины к меньшей)\n");
    printf("Ваш выбор [1]: ");

    char input[10];
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0 && input[0] == '2') {
            sort_mode = 2;
            printf("✓ Выбран режим: ПО УБЫВАНИЮ\n");
        }
        else {
            printf("✓ Выбран режим: ПО ВОЗРАСТАНИЮ\n");
        }
    }

    // 
    // ПРОВЕРКА ТЕКУЩЕЙ ДИРЕКТОРИИ
    // 
    print_separator("ИНФОРМАЦИЯ О ДИРЕКТОРИИ");

    char current_dir[1024];
    if (_getcwd(current_dir, sizeof(current_dir)) != NULL) {
        printf("Текущая рабочая директория:\n%s\n", current_dir);
    }

    // Построить полный путь к файлу
    char full_path[1024];
    _snprintf(full_path, sizeof(full_path), "%s\\%s", current_dir, filename);
    printf("\nПолный путь к файлу:\n%s\n", full_path);

    // 
    // ПРОВЕРКА ФАЙЛА
    // 
    print_separator("ПРОВЕРКА ФАЙЛА");

    printf("Проверка существования файла...\n");

    // Проверяем существование файла
    FILE* test_file = fopen(full_path, "r");
    if (!test_file) {
        test_file = fopen(filename, "r");
    }

    if (!test_file) {
        printf("✗ ФАЙЛ НЕ НАЙДЕН: '%s'\n", filename);
        printf("\nОшибка системы: %s\n", strerror(errno));

        print_separator("РЕКОМЕНДАЦИИ");
        printf("1. Проверьте правильность имени файла\n");
        printf("2. Убедитесь, что файл находится в директории:\n");
        printf("   %s\n", current_dir);
        printf("3. Создайте файл с измерениями\n");

        print_separator("ПРИМЕР ФАЙЛА С ИЗМЕРЕНИЯМИ");
        printf("Создайте файл %s со следующим содержимым:\n", filename);
        printf("----------------------------------------\n");
        printf("1.5 m\n");
        printf("100 cm\n");
        printf("2 ya\n");
        printf("3 ft\n");
        printf("2.5 lok\n");
        printf("10 ver\n");
        printf("1.7 arsh\n");
        printf("0.5 sazh\n");
        printf("----------------------------------------\n");
        printf("Формат: <число> <единица_измерения>\n");

        print_separator(NULL);
        return 1;
    }
    fclose(test_file);
    printf("✓ Файл найден\n");

    // 
    // ЧТЕНИЕ ИЗМЕРЕНИЙ ИЗ ФАЙЛА
    // 
    print_separator("ЧТЕНИЕ ИЗМЕРЕНИЙ ИЗ ФАЙЛА");

    int count = 0;
    int invalid_lines = 0;

    Measurement* measurements = read_measurements(full_path, &count, &invalid_lines);
    if (!measurements) {
        // Попробуем относительный путь
        measurements = read_measurements(filename, &count, &invalid_lines);
        if (!measurements) {
            printf("✗ Не удалось прочитать измерения из файла\n");
            print_separator(NULL);
            return 1;
        }
    }

    printf("\n✓ Чтение завершено\n");
    printf("  Успешно прочитано измерений: %d\n", count);
    printf("  Пропущено невалидных строк: %d\n", invalid_lines);

    if (count == 0) {
        printf("\n✗ В файле не найдено ни одного валидного измерения\n");
        printf("  Добавьте измерения в файл и запустите программу снова\n");

        free(measurements);
        print_separator("ФОРМАТ ИЗМЕРЕНИЙ");
        printf("Каждая строка должна содержать:\n");
        printf("  <число> <единица_измерения>\n");
        printf("\nПримеры:\n");
        printf("  1.5 m     - 1.5 метров\n");
        printf("  100 cm    - 100 сантиметров\n");
        printf("  2 ya      - 2 ярда\n");
        printf("  3 ft      - 3 фута\n");
        printf("  2.5 lok   - 2.5 локтя\n");

        print_separator(NULL);
        return 1;
    }

    // 
    // ВЫВОД ИСХОДНЫХ ДАННЫХ
    // 
    print_separator("ИСХОДНЫЕ ИЗМЕРЕНИЯ");

    printf("Найдено %d измерений:\n", count);
    printf("№  | Значение | Единица | Название         | В метрах\n");
    printf("---+----------+---------+------------------+-------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-2d | %8.3f | %-7s | %-16s | %8.6f м\n",
            i + 1,
            measurements[i].value,
            measurements[i].unit,
            get_unit_full_name(measurements[i].unit),
            measurements[i].value_in_meters);
    }

    // 
    // СОРТИРОВКА ИЗМЕРЕНИЙ
    // 
    print_separator("ПРОЦЕСС СОРТИРОВКИ");

    if (sort_mode == 1) {
        printf("Сортировка по ВОЗРАСТАНИЮ длины...\n");
        qsort(measurements, count, sizeof(Measurement), compare_measurements_asc);
    }
    else {
        printf("Сортировка по УБЫВАНИЮ длины...\n");
        qsort(measurements, count, sizeof(Measurement), compare_measurements_desc);
    }

    printf("✓ Сортировка завершена\n");

    // 
    // ВЫВОД ОТСОРТИРОВАННЫХ ДАННЫХ
    // 
    print_separator("ОТСОРТИРОВАННЫЕ ИЗМЕРЕНИЯ");

    printf("Измерения отсортированы %s:\n",
        sort_mode == 1 ? "по возрастанию" : "по убыванию");
    printf("№  | Значение | Единица | Название         | В метрах\n");
    printf("---+----------+---------+------------------+-------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-2d | %8.3f | %-7s | %-16s | %8.6f м\n",
            i + 1,
            measurements[i].value,
            measurements[i].unit,
            get_unit_full_name(measurements[i].unit),
            measurements[i].value_in_meters);
    }

    // 
    // ПЕРЕЗАПИСЬ ИСХОДНОГО ФАЙЛА
    // 
    print_separator("ПЕРЕЗАПИСЬ ФАЙЛА");

    printf("Перезаписываю исходный файл '%s' отсортированными данными...\n", filename);

    if (!write_measurements(full_path, measurements, count)) {
        // Попробуем относительный путь
        if (!write_measurements(filename, measurements, count)) {
            printf("✗ Не удалось перезаписать исходный файл\n");
            printf("  Пробую сохранить результат в новый файл...\n");

            // Создаем имя для нового файла
            char backup_filename[256];
            _snprintf(backup_filename, sizeof(backup_filename), "sorted_%s", filename);

            if (write_measurements(backup_filename, measurements, count)) {
                printf("✓ Результат сохранен в файл '%s'\n", backup_filename);
            }
            else {
                printf("✗ Не удалось сохранить результат вообще\n");
                free(measurements);
                print_separator(NULL);
                return 1;
            }
        }
        else {
            printf("✓ Исходный файл перезаписан через относительный путь\n");
        }
    }
    else {
        printf("✓ Исходный файл успешно перезаписан\n");
    }

    // 
    // СОЗДАНИЕ ФАЙЛА С РЕЗУЛЬТАТАМИ В МЕТРАХ
    // 
    print_separator("СОЗДАНИЕ ФАЙЛА С РЕЗУЛЬТАТАМИ В МЕТРАХ");

    printf("Создать дополнительный файл с результатами в метрах?\n");
    printf("1. Да, создать файл 'results_in_meters.txt'\n");
    printf("2. Нет, только перезаписать исходный файл\n");
    printf("Ваш выбор [1]: ");

    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0 || input[0] == '1') {
            FILE* meters_file = fopen("results_in_meters.txt", "w");
            if (meters_file) {
                fprintf(meters_file, "Отсортированные измерения (в метрах):\n");
                fprintf(meters_file, "=====================================\n");

                for (int i = 0; i < count; i++) {
                    fprintf(meters_file, "%d. %8.3f %s = %8.6f м\n",
                        i + 1,
                        measurements[i].value,
                        measurements[i].unit,
                        measurements[i].value_in_meters);
                }

                fclose(meters_file);
                printf("✓ Файл 'results_in_meters.txt' создан\n");
            }
            else {
                printf("✗ Не удалось создать файл с результатами в метрах\n");
            }
        }
    }

    // 
    // ВЫВОД СВОДНОЙ ИНФОРМАЦИИ
    // 
    print_separator("СВОДНАЯ ИНФОРМАЦИЯ");

    // Находим минимальное и максимальное значение
    double min_value = measurements[0].value_in_meters;
    double max_value = measurements[0].value_in_meters;
    const char* min_unit = measurements[0].unit;
    const char* max_unit = measurements[0].unit;
    double min_original = measurements[0].value;
    double max_original = measurements[0].value;

    for (int i = 1; i < count; i++) {
        if (measurements[i].value_in_meters < min_value) {
            min_value = measurements[i].value_in_meters;
            min_unit = measurements[i].unit;
            min_original = measurements[i].value;
        }
        if (measurements[i].value_in_meters > max_value) {
            max_value = measurements[i].value_in_meters;
            max_unit = measurements[i].unit;
            max_original = measurements[i].value;
        }
    }

    printf("Статистика:\n");
    printf("  Всего измерений: %d\n", count);
    printf("  Невалидных строк пропущено: %d\n", invalid_lines);
    printf("  Режим сортировки: %s\n",
        sort_mode == 1 ? "по возрастанию" : "по убыванию");
    printf("\n  Самое короткое измерение:\n");
    printf("    %.6f %s (%s) = %.6f м\n",
        min_original, min_unit, get_unit_full_name(min_unit), min_value);
    printf("\n  Самое длинное измерение:\n");
    printf("    %.6f %s (%s) = %.6f м\n",
        max_original, max_unit, get_unit_full_name(max_unit), max_value);
    printf("\n  Диапазон: от %.6f м до %.6f м\n", min_value, max_value);

    // 
    // ОЧИСТКА ПАМЯТИ И ЗАВЕРШЕНИЕ
    // 
    print_separator("ЗАВЕРШЕНИЕ РАБОТЫ");


    free(measurements);

    printf("Память освобождена\n");
    printf("Программа завершена успешно\n");
    printf("\nФайл '%s' теперь содержит отсортированные измерения\n", filename);

    print_separator(NULL);

    return 0;
}
