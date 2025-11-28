#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to compute GCD using Euclidean algorithm
int gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to simplify fraction
void simplify_fraction(int* numerator, int* denominator) {
    int common_divisor = gcd(*numerator, *denominator);
    *numerator /= common_divisor;
    *denominator /= common_divisor;

    // Ensure denominator is positive
    if (*denominator < 0) {
        *numerator = -*numerator;
        *denominator = -*denominator;
    }
}

// Function to perform fraction operations
void fraction_operations(int a, int b, int c, int d) {
    // Sum: a/b + c/d = (a*d + c*b)/(b*d)
    int sum_num = a * d + c * b;
    int sum_den = b * d;
    simplify_fraction(&sum_num, &sum_den);
    printf("Sum: %d/%d\n", sum_num, sum_den);

    // Difference: a/b - c/d = (a*d - c*b)/(b*d)
    int diff_num = a * d - c * b;
    int diff_den = b * d;
    simplify_fraction(&diff_num, &diff_den);
    printf("Difference: %d/%d\n", diff_num, diff_den);

    // Product: (a/b) * (c/d) = (a*c)/(b*d)
    int prod_num = a * c;
    int prod_den = b * d;
    simplify_fraction(&prod_num, &prod_den);
    printf("Product: %d/%d\n", prod_num, prod_den);

    // Quotient: (a/b) / (c/d) = (a*d)/(b*c)
    if (c == 0) {
        printf("Quotient: undefined (division by zero)\n");
    }
    else {
        int quot_num = a * d;
        int quot_den = b * c;
        simplify_fraction(&quot_num, &quot_den);
        printf("Quotient: %d/%d\n", quot_num, quot_den);
    }
}

// Validation functions
int is_non_negative_integer(const char* str) {
    if (!str || *str == '\0') return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int is_non_zero_integer(const char* str) {
    if (!str || *str == '\0') return 0;

    // Check for sign
    int start = 0;
    if (str[0] == '-') {
        start = 1;
    }

    // Check if all characters are digits
    for (int i = start; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }

    // Check if number is zero
    int value = atoi(str);
    return value != 0;
}

int main() {
    char input[100];
    int a, b, c, d;

    printf("Enter four integers a, b, c, d for fractions a/b and c/d:\n");
    printf("a and c must be non-negative integers, b and d must be non-zero integers.\n");

    // Input a
    while (1) {
        printf("Enter a (non-negative integer): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error occurred.\n");
            return 1;
        }
        input[strcspn(input, "\n")] = '\0';

        if (is_non_negative_integer(input)) {
            a = atoi(input);
            break;
        }
        printf("Invalid input. ");
    }

    // Input b
    while (1) {
        printf("Enter b (non-zero integer): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error occurred.\n");
            return 1;
        }
        input[strcspn(input, "\n")] = '\0';

        if (is_non_zero_integer(input)) {
            b = atoi(input);
            break;
        }
        printf("Invalid input. ");
    }

    // Input c
    while (1) {
        printf("Enter c (non-negative integer): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error occurred.\n");
            return 1;
        }
        input[strcspn(input, "\n")] = '\0';

        if (is_non_negative_integer(input)) {
            c = atoi(input);
            break;
        }
        printf("Invalid input. ");
    }

    // Input d
    while (1) {
        printf("Enter d (non-zero integer): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error occurred.\n");
            return 1;
        }
        input[strcspn(input, "\n")] = '\0';

        if (is_non_zero_integer(input)) {
            d = atoi(input);
            break;
        }
        printf("Invalid input. ");
    }

    printf("\nFractions: %d/%d and %d/%d\n", a, b, c, d);
    fraction_operations(a, b, c, d);

    return 0;
}
