#include <stdio.h>
#include <stdlib.h>

// Recursive Fibonacci function with memoization
long long fibonacci_recursive(int n, long long* memo) {
    if (n <= 1) {
        return n;
    }
    if (memo[n] != -1) {
        return memo[n];
    }
    memo[n] = fibonacci_recursive(n - 1, memo) + fibonacci_recursive(n - 2, memo);
    return memo[n];
}

// Function to generate first n Fibonacci numbers
long long* generate_fibonacci_sequence(int n) {
    if (n <= 0) {
        return NULL;
    }

    // Allocate memory for result array
    long long* result = (long long*)malloc(n * sizeof(long long));
    if (!result) {
        return NULL;
    }

    // Allocate memory for memoization
    long long* memo = (long long*)malloc((n + 1) * sizeof(long long));
    if (!memo) {
        free(result);
        return NULL;
    }

    // Initialize memoization array
    for (int i = 0; i <= n; i++) {
        memo[i] = -1;
    }

    // Generate Fibonacci sequence
    for (int i = 0; i < n; i++) {
        result[i] = fibonacci_recursive(i, memo);
    }

    free(memo);
    return result;
}

// Validation function for positive integer
int is_positive_integer(const char* str) {
    if (!str || *str == '\0') return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int main() {
    char input[100];
    int n;

    printf("Enter the number of Fibonacci numbers to generate: ");

    // Input validation loop
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error occurred.\n");
            return 1;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = '\0';

        if (!is_positive_integer(input)) {
            printf("Please enter a positive integer: ");
            continue;
        }

        n = atoi(input);
        if (n <= 0) {
            printf("Please enter a positive integer: ");
            continue;
        }

        break;
    }

    long long* fibonacci_sequence = generate_fibonacci_sequence(n);
    if (!fibonacci_sequence) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("First %d Fibonacci numbers:\n", n);
    for (int i = 0; i < n; i++) {
        printf("F(%d) = %lld\n", i, fibonacci_sequence[i]);
    }

    free(fibonacci_sequence);
    return 0;
}
