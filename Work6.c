//Починить должен работать при аргументах командной строки Readmy.txt 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <direct.h>
#include<locale.h>

#pragma warning(disable: 4996)
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

// Function to validate if string contains only uppercase Latin letters
int is_valid_string(const char* str) {
    if (!str) {
        return 0;
    }

    // Пустая строка - валидный ввод для шифрования
    if (*str == '\0') {
        return 1;
    }

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < 'A' || str[i] > 'Z') {
            return 0;
        }
    }
    return 1;
}

// Function to validate shift value
int is_valid_shift(const char* str) {
    if (!str || *str == '\0') return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }

    int shift = atoi(str);
    return shift >= 1 && shift <= 1000;
}

// Caesar cipher encryption function
void caesar_encrypt(const char* plaintext, int shift, char* ciphertext) {
    int len = strlen(plaintext);

    for (int i = 0; i < len; i++) {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z') {
            ciphertext[i] = 'A' + (plaintext[i] - 'A' + shift) % 26;
        }
        else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[len] = '\0';
}

// 
// Основная функция
// 

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, ("RUS"));
    print_separator("ШИФР ЦЕЗАРЯ");
    print_separator(NULL);

    // 
    // ПРОВЕРКА АРГУМЕНТОВ КОМАНДНОЙ СТРОКИ
    // 
    print_separator("ПРОВЕРКА АРГУМЕНТОВ");

    if (argc != 3) {
        printf("ОШИБКА: Неправильное количество аргументов: %d\n\n", argc);
        printf("Использование: %s <файл> <сдвиг>\n", argv[0]);
        printf("Пример: %s Readmy.txt 4\n\n", argv[0]);

        printf("Полученные аргументы:\n");
        for (int i = 0; i < argc; i++) {
            printf("  Аргумент %d: '%s'\n", i, argv[i]);
        }

        // Показать текущую директорию
        char current_dir[1024];
        if (_getcwd(current_dir, sizeof(current_dir)) != NULL) {
            printf("\nТекущая рабочая директория:\n%s\n", current_dir);
        }

        print_separator("СПИСОК ФАЙЛОВ В ДИРЕКТОРИИ");
        system("dir /B");
        print_separator(NULL);

        return 1;
    }

    const char* filename = argv[1];
    const char* shift_str = argv[2];

    printf("✓ Имя файла: '%s'\n", filename);
    printf("✓ Значение сдвига: '%s'\n", shift_str);

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

    // Сначала пробуем открыть файл
    FILE* file = fopen(full_path, "r");
    if (!file) {
        // Пробуем относительный путь
        file = fopen(filename, "r");
    }

    if (!file) {
        printf("✗ ФАЙЛ НЕ НАЙДЕН: '%s'\n", filename);
        printf("\nОшибка системы: %s\n", strerror(errno));

        print_separator("РЕКОМЕНДАЦИИ");
        printf("1. Проверьте правильность имени файла\n");
        printf("2. Убедитесь, что файл находится в директории:\n");
        printf("   %s\n", current_dir);
        printf("3. Проверьте права доступа к файлу\n");

        print_separator("СОЗДАНИЕ ФАЙЛА ВРУЧНУЮ");
        printf("Для создания файла выполните в терминале:\n");
        printf("  echo ТЕКСТ > %s\n", filename);
        printf("\nПример:\n");
        printf("  echo HELLOWORLDDMFFKFJDJSDH > Readmy.txt\n");

        print_separator(NULL);
        return 1;
    }

    printf("✓ Файл найден и открыт для чтения\n");

    // 
    // ПРОВЕРКА ЗНАЧЕНИЯ СДВИГА
    // 
    print_separator("ПРОВЕРКА СДВИГА");

    if (!is_valid_shift(shift_str)) {
        printf("✗ Ошибка: Недопустимое значение сдвига\n");
        printf("  Сдвиг должен быть целым числом от 1 до 1000\n");
        printf("  Получено: '%s'\n", shift_str);
        fclose(file);
        print_separator(NULL);
        return 1;
    }

    int shift = atoi(shift_str);
    printf("✓ Значение сдвига корректно: %d\n", shift);

    // 
    // ПРОВЕРКА РАЗМЕРА ФАЙЛА
    // 
    print_separator("ПРОВЕРКА РАЗМЕРА ФАЙЛА");

    // Определение размера файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    printf("✓ Размер файла: %ld байт\n", file_size);

    // 
    // ОБРАБОТКА ПУСТОГО ФАЙЛА
    // 
    if (file_size == 0) {
        print_separator("ОБНАРУЖЕН ПУСТОЙ ФАЙЛ");

        printf("Файл '%s' пустой\n", filename);
        printf("Заполняю файл текстом 'HELLOWORLDDMFFKFJDJSDH'...\n");

        // Закрываем файл для чтения
        fclose(file);

        // Открываем файл для записи
        file = fopen(full_path, "w");
        if (!file) {
            file = fopen(filename, "w");
        }

        if (!file) {
            printf("✗ Не удалось открыть файл для записи\n");
            print_separator(NULL);
            return 1;
        }

        // Записываем текст в файл
        const char* default_text = "HELLOWORLDDMFFKFJDJSDH";
        fprintf(file, "%s", default_text);
        fclose(file);

        printf("✓ Файл успешно заполнен текстом: '%s'\n", default_text);
        printf("✓ Размер файла теперь: %zu байт\n", strlen(default_text));

        // Открываем файл снова для чтения
        file = fopen(full_path, "r");
        if (!file) {
            file = fopen(filename, "r");
        }

        if (!file) {
            printf("✗ Не удалось открыть файл после заполнения\n");
            print_separator(NULL);
            return 1;
        }

        // Обновляем размер файла
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        printf("✓ Файл готов к шифрованию\n");
    }

    // 
    // ЧТЕНИЕ ФАЙЛА
    // 
    print_separator("ЧТЕНИЕ ФАЙЛА");

    printf("Чтение содержимого файла...\n");

    // Выделение памяти для текста
    char* text = (char*)malloc(file_size + 1);
    if (!text) {
        printf("✗ Ошибка выделения памяти\n");
        fclose(file);
        print_separator(NULL);
        return 1;
    }

    // Чтение файла
    size_t bytes_read = fread(text, 1, file_size, file);
    text[bytes_read] = '\0';

    fclose(file);

    printf("✓ Прочитано байт: %zu\n", bytes_read);

    // 
    // ОБРАБОТКА ПРОЧИТАННОГО ТЕКСТА
    // 
    print_separator("ОБРАБОТКА ТЕКСТА");

    printf("Очистка текста от символов новой строки...\n");

    // Сохраняем оригинал для отображения
    char* original_text = strdup(text);

    // Удаление завершающих символов новой строки и пробелов
    while (bytes_read > 0 &&
        (text[bytes_read - 1] == '\n' ||
            text[bytes_read - 1] == '\r' ||
            text[bytes_read - 1] == ' ' ||
            text[bytes_read - 1] == '\t')) {
        text[--bytes_read] = '\0';
    }

    printf("Содержимое файла:\n");
    printf("----------------------------------------\n");
    printf("%s\n", original_text);
    printf("----------------------------------------\n\n");

    printf("Очищенное содержимое:\n");
    printf("----------------------------------------\n");
    printf("%s\n", text);
    printf("----------------------------------------\n");

    printf("Длина очищенного текста: %zu символов\n", strlen(text));

    // 
    // ПРОВЕРКА ВАЛИДНОСТИ ТЕКСТА
    // 
    print_separator("ПРОВЕРКА ТЕКСТА");

    if (!is_valid_string(text)) {
        if (strlen(text) == 1) {
            printf("Файл содержит только пробелы или пустые строки\n");
            printf("Заполняю файл текстом 'HELLOWORLDDMFFKFJDJSDH'...\n");

            // Открываем файл для записи
            file = fopen(full_path, "w");
            if (!file) {
                file = fopen(filename, "w");
            }

            if (file) {
                const char* default_text = "HELLOWORLDDMFFKFJDJSDH";
                fprintf(file, "%s", default_text);
                fclose(file);

                printf("✓ Файл заполнен текстом: '%s'\n", default_text);

                // Обновляем текст
                free(text);
                free(original_text);

                text = strdup(default_text);
                original_text = strdup(default_text);
            }
        }
        else {
            printf("  Разрешены только заглавные латинские буквы (A-Z)\n");
            printf("----------------------------------------\n");
            printf("%s\n", text);
            printf("----------------------------------------\n");
            printf("\nИсправьте текст в файле и запустите программу снова\n");

            free(original_text);
            free(text);
            print_separator(NULL);
            return 1;
        }
    }

    printf("✓ Текст прошел проверку\n");

    // 
    // ШИФРОВАНИЕ ТЕКСТА
    // 
    print_separator("ПРОЦЕСС ШИФРОВАНИЯ");

    size_t text_len = strlen(text);
    printf("Выделение памяти для зашифрованного текста...\n");
    char* encrypted = (char*)malloc(text_len + 1);
    if (!encrypted) {
        printf("✗ Ошибка выделения памяти для шифра\n");
        free(original_text);
        free(text);
        print_separator(NULL);
        return 1;
    }

    printf("Выполнение шифрования Цезаря...\n");
    printf("Сдвиг: %d символов\n", shift);

    caesar_encrypt(text, shift, encrypted);

    printf("✓ Шифрование завершено\n");

    // 
    // ВЫВОД РЕЗУЛЬТАТОВ
    // 
    print_separator("РЕЗУЛЬТАТ ШИФРОВАНИЯ");

    printf("Исходный текст:\n");
    printf("----------------------------------------\n");
    printf("%s\n", text);
    printf("----------------------------------------\n\n");

    printf("Сдвиг: %d\n\n", shift);

    printf("Зашифрованный текст:\n");
    printf("----------------------------------------\n");
    printf("%s\n", encrypted);
    printf("----------------------------------------\n");

    // 
    // СОХРАНЕНИЕ РЕЗУЛЬТАТА В ФАЙЛ
    // 
    print_separator("СОХРАНЕНИЕ РЕЗУЛЬТАТА");

    printf("Сохранение результата в файл 'encrypted.txt'...\n");
    FILE* out = fopen("encrypted.txt", "w");
    if (out) {
        fprintf(out, "%s", encrypted);
        fclose(out);
        printf("✓ Результат успешно сохранен\n");

        // Проверка сохраненного файла
        FILE* verify = fopen("encrypted.txt", "r");
        if (verify) {
            fseek(verify, 0, SEEK_END);
            long out_size = ftell(verify);
            fclose(verify);
            printf("✓ Размер выходного файла: %ld байт\n", out_size);
        }
    }
    else {
        printf("✗ Не удалось сохранить результат\n");
    }

    // 
    // ОЧИСТКА ПАМЯТИ И ЗАВЕРШЕНИЕ
    // 
    print_separator("ЗАВЕРШЕНИЕ РАБОТЫ");

    free(original_text);
    free(text);
    free(encrypted);

    printf("Память освобождена\n");
    printf("Программа завершена успешно\n");

    print_separator(NULL);

    return 0;
}
