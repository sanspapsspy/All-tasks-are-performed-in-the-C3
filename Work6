#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to validate if string contains only uppercase Latin letters
int is_valid_string(const char* str) {
    if (!str || *str == '\0') return 0;

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
        if (!isdigit(str[i])) {
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
            // Apply Caesar cipher: (char - 'A' + shift) mod 26 + 'A'
            ciphertext[i] = 'A' + (plaintext[i] - 'A' + shift) % 26;
        }
        else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[len] = '\0';
}

int main(int argc, char* argv[]) {
    // Validate command line arguments
    if (argc != 3) {
        printf("Usage: %s <text> <shift>\n", argv[0]);
        printf("Text must contain only uppercase Latin letters\n");
        printf("Shift must be between 1 and 1000\n");
        return 1;
    }

    const char* text = argv[1];
    const char* shift_str = argv[2];

    // Validate input string
    if (!is_valid_string(text)) {
        printf("Error: Text must contain only uppercase Latin letters (A-Z)\n");
        return 1;
    }

    // Validate shift value
    if (!is_valid_shift(shift_str)) {
        printf("Error: Shift must be an integer between 1 and 1000\n");
        return 1;
    }

    int shift = atoi(shift_str);
    char* encrypted = malloc(strlen(text) + 1);

    if (!encrypted) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Perform encryption
    caesar_encrypt(text, shift, encrypted);

    // Output result
    printf("Original: %s\n", text);
    printf("Shift: %d\n", shift);
    printf("Encrypted: %s\n", encrypted);

    free(encrypted);
    return 0;
}
