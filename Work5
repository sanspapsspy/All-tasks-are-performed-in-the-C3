#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DIGITS 100

// Function to convert character to digit
int char_to_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    return -1;
}

// Function to convert number to string in given base
int number_to_base_string(unsigned long long number, int base, char* buffer, int buffer_size) {
    if (base < 2 || base > 36 || buffer_size < 2) return 0;

    // Handle zero case
    if (number == 0) {
        if (buffer_size < 2) return 0;
        buffer[0] = '0';
        buffer[1] = '\0';
        return 1;
    }

    int index = 0;
    char temp_buffer[MAX_DIGITS];

    // Convert number to string in reverse order
    while (number > 0 && index < MAX_DIGITS - 1) {
        int digit = number % base;
        temp_buffer[index++] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        number /= base;
    }

    // Check if buffer is large enough
    if (index >= buffer_size) return 0;

    // Reverse the string
    for (int i = 0; i < index; i++) {
        buffer[i] = temp_buffer[index - 1 - i];
    }
    buffer[index] = '\0';

    return 1;
}

// Function to generate histogram
void generate_histogram(unsigned long long number, int base) {
    char number_str[MAX_DIGITS];
    int frequency[36] = { 0 }; // Frequency for digits 0-35

    // Convert number to string in given base
    if (!number_to_base_string(number, base, number_str, sizeof(number_str))) {
        printf("Error converting number to base %d\n", base);
        return;
    }

    // Count frequency of each digit
    for (int i = 0; number_str[i] != '\0'; i++) {
        int digit = char_to_digit(number_str[i]);
        if (digit >= 0 && digit < base) {
            frequency[digit]++;
        }
    }

    // Print histogram
    printf("Histogram for number %llu in base %d:\n", number, base);
    printf("Number representation: %s\n", number_str);

    for (int i = 0; i < base; i++) {
        printf("%d ", i);
        for (int j = 0; j < frequency[i]; j++) {
            printf("*");
        }
        printf("\n");
    }
    printf("\n");
}

// Validation functions
int is_valid_number(const char* str) {
    if (!str || *str == '\0') return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int is_valid_base(const char* str) {
    if (!str || *str == '\0') return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }

    int base = atoi(str);
    return base >= 2 && base <= 36;
}

int main() {
    char input[100];
    unsigned long long number;
    int base;

    printf("=== Digit Frequency Histogram ===\n");

    // Get number input
    while (1) {
        printf("Enter a non-negative integer (0 to 10^9): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error occurred.\n");
            return 1;
        }
        input[strcspn(input, "\n")] = '\0';

        if (is_valid_number(input)) {
            number = strtoull(input, NULL, 10);
            if (number <= 1000000000ULL) { // 10^9
                break;
            }
        }
        printf("Invalid input. Please enter a non-negative integer up to 10^9.\n");
    }

    // Get base input
    while (1) {
        printf("Enter base (2 to 36): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error occurred.\n");
            return 1;
        }
        input[strcspn(input, "\n")] = '\0';

        if (is_valid_base(input)) {
            base = atoi(input);
            break;
        }
        printf("Invalid input. Please enter a base between 2 and 36.\n");
    }

    generate_histogram(number, base);

    return 0;
}
