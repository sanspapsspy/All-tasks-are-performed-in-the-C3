#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include<locale.h>

// Функция для проверки, является ли число простым
bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    // Проверяем делители до √n
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

// Функция для вычисления суммы цифр числа в десятичной системе
int sum_of_digits_base10(int n) {
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

// Функция для преобразования цифры в символ
char digit_to_char(int digit) {
    if (digit >= 0 && digit <= 9) {
        return '0' + digit;
    }
    else {
        return 'A' + (digit - 10);
    }
}

// Функция для преобразования числа в строку в заданной системе счисления
bool number_to_string_in_base(int n, int base, char* buffer, int buffer_size) {
    if (base < 2 || base > 36 || buffer_size < 2) {
        return false;
    }

    // Обработка нуля
    if (n == 0) {
        if (buffer_size < 2) return false;
        buffer[0] = '0';
        buffer[1] = '\0';
        return true;
    }

    // Временный буфер для хранения цифр в обратном порядке
    char temp_buffer[64];
    int index = 0;
    int num = n;

    // Извлекаем цифры
    while (num > 0 && index < 63) {
        int digit = num % base;
        temp_buffer[index++] = digit_to_char(digit);
        num /= base;
    }

    // Проверяем, достаточно ли места в буфере
    if (index >= buffer_size) {
        return false;
    }

    // Разворачиваем цифры в правильном порядке
    for (int i = 0; i < index; i++) {
        buffer[i] = temp_buffer[index - 1 - i];
    }
    buffer[index] = '\0';

    return true;
}

// Функция для вычисления произведения цифр числа в заданной системе счисления
int product_of_digits_in_base(int n, int base) {
    if (n == 0) return 0;

    int product = 1;
    int num = n;

    while (num > 0) {
        int digit = num % base;
        product *= digit;
        num /= base;
    }

    return product;
}

// Функция для проверки, является ли строка палиндромом
bool is_palindrome(const char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            return false;
        }
    }
    return true;
}

// Функция для проверки, является ли число простым числом Софи Жермен
bool is_sophie_germain_prime(int p) {
    return is_prime(p) && is_prime(2 * p + 1);
}

// Функция валидации натурального числа
bool is_valid_natural_number(const char* str) {
    if (str == NULL || *str == '\0') {
        return false;
    }

    // Проверяем, что все символы - цифры
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }

    // Проверяем, что число не равно 0
    return atoi(str) > 0;
}

// Функция валидации основания системы счисления
bool is_valid_base(const char* str) {
    if (!is_valid_natural_number(str)) {
        return false;
    }

    int base = atoi(str);
    return base >= 2 && base <= 36;
}

// a. Все простые числа в диапазоне [1..N]
void print_primes_up_to_n(int n) {
    printf("a) Все простые числа в диапазоне [1..%d]:\n", n);
    bool found = false;

    for (int i = 2; i <= n; i++) {
        if (is_prime(i)) {
            printf("%d ", i);
            found = true;
        }
    }

    if (!found) {
        printf("Простые числа не найдены");
    }
    printf("\n\n");
}

// b. Числа, сумма цифр которых в десятичной системе является простым числом
void print_numbers_with_prime_digit_sum(int n) {
    printf("b) Числа, сумма цифр которых является простым числом:\n");
    bool found = false;

    for (int i = 1; i <= n; i++) {
        int digit_sum = sum_of_digits_base10(i);
        if (is_prime(digit_sum)) {
            printf("%d (сумма цифр: %d) ", i, digit_sum);
            found = true;
        }
    }

    if (!found) {
        printf("Такие числа не найдены");
    }
    printf("\n\n");
}

// c. Числа, произведение цифр в системе с основанием base является простым числом
void print_numbers_with_prime_digit_product(int n, int base) {
    printf("c) Числа, произведение цифр в системе с основанием %d является простым числом:\n", base);
    bool found = false;

    for (int i = 1; i <= n; i++) {
        int product = product_of_digits_in_base(i, base);
        if (product > 1 && is_prime(product)) {
            char representation[64];
            number_to_string_in_base(i, base, representation, sizeof(representation));
            printf("%d (в системе %d: %s, произведение цифр: %d) ",
                i, base, representation, product);
            found = true;
        }
    }

    if (!found) {
        printf("Такие числа не найдены");
    }
    printf("\n\n");
}

// d. Числа, произведение цифр в системе с основанием base НЕ является простым числом
void print_numbers_with_non_prime_digit_product(int n, int base) {
    printf("d) Числа, произведение цифр в системе с основанием %d НЕ является простым числом:\n", base);
    bool found = false;

    for (int i = 1; i <= n; i++) {
        int product = product_of_digits_in_base(i, base);
        // Исключаем случаи, когда произведение равно 0 или 1
        if (product != 0 && product != 1 && !is_prime(product)) {
            char representation[64];
            number_to_string_in_base(i, base, representation, sizeof(representation));
            printf("%d (в системе %d: %s, произведение цифр: %d) ",
                i, base, representation, product);
            found = true;
        }
    }

    if (!found) {
        printf("Такие числа не найдены");
    }
    printf("\n\n");
}

// e. Числа, представление которых в системе с основанием base является палиндромом
void print_palindromic_numbers_in_base(int n, int base) {
    printf("e) Числа, представление которых в системе с основанием %d является палиндромом:\n", base);
    bool found = false;

    for (int i = 1; i <= n; i++) {
        char representation[64];
        if (number_to_string_in_base(i, base, representation, sizeof(representation)) &&
            is_palindrome(representation)) {
            printf("%d (в системе %d: %s) ", i, base, representation);
            found = true;
        }
    }

    if (!found) {
        printf("Такие числа не найдены");
    }
    printf("\n\n");
}

// f. Простые числа Софи Жермен
void print_sophie_germain_primes(int n) {
    printf("f) Простые числа Софи Жермен в диапазоне [1..%d]:\n", n);
    bool found = false;

    for (int i = 2; i <= n; i++) {
        if (is_sophie_germain_prime(i)) {
            printf("%d (2×%d+1=%d) ", i, i, 2 * i + 1);
            found = true;
        }
    }

    if (!found) {
        printf("Простые числа Софи Жермен не найдены");
    }
    printf("\n");
}

// Основная функция
int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "Rus");
    // Проверка количества аргументов
    if (argc != 3) {
        printf("Использование: %s <N> <base>\n", argv[0]);
        printf("  N - натуральное число\n");
        printf("  base - основание системы счисления (2-36)\n");
        printf("Пример: %s 100 16\n", argv[0]);
        return 1;
    }

    // Валидация входных параметров
    if (!is_valid_natural_number(argv[1])) {
        printf("Ошибка: N должно быть натуральным числом\n");
        return 1;
    }

    if (!is_valid_base(argv[2])) {
        printf("Ошибка: base должно быть натуральным числом в диапазоне [2..36]\n");
        return 1;
    }

    // Преобразование параметров
    int N = atoi(argv[1]);
    int base = atoi(argv[2]);

    printf("Анализ чисел в диапазоне [1..%d] с основанием системы счисления %d\n", N, base);
    printf("================================================\n\n");

    // Выполнение всех подзадач
    print_primes_up_to_n(N);
    print_numbers_with_prime_digit_sum(N);
    print_numbers_with_prime_digit_product(N, base);
    print_numbers_with_non_prime_digit_product(N, base);
    print_palindromic_numbers_in_base(N, base);
    print_sophie_germain_primes(N);

    return 0;
}
