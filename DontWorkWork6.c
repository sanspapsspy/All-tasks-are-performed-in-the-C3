//Починить 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to validate if string contains only uppercase Latin letters
int is_valid_string(const char* str) {
    if (!str) return 0;  // Проверка на NULL

    // Пустая строка - валидный ввод для шифрования
    if (*str == '\0') return 1;

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
            ciphertext[i] = 'A' + (plaintext[i] - 'A' + shift) % 26;
        }
        else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[len] = '\0';
}

int main() {
    const char* filename = "Readmy.txt";

    printf("Reading from file: %s\n", filename);

    // Open file
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", filename);
        printf("Please make sure the file exists in the same directory.\n");
        return 1;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Handle empty file
    if (file_size == 0) {
        printf("File '%s' is empty - nothing to encrypt\n", filename);
        printf("Please enter shift value: ");

        char shift_str[100];
        if (fgets(shift_str, sizeof(shift_str), stdin) == NULL) {
            printf("Error reading shift value\n");
            fclose(file);
            return 1;
        }

        // Remove newline from input
        shift_str[strcspn(shift_str, "\n")] = '\0';

        // Validate shift value
        if (!is_valid_shift(shift_str)) {
            printf("Error: Shift must be an integer between 1 and 1000\n");
            fclose(file);
            return 1;
        }

        int shift = atoi(shift_str);

        printf("\nProcessing empty file:\n");
        printf("Original: (empty)\n");
        printf("Shift: %d\n", shift);
        printf("Readmy: (empty)\n");

        // Save empty result
        FILE* out = fopen("Readmy.txt", "w");
        if (out) {
            fclose(out);
            printf("Empty encrypted file created as 'Readmy.txt'\n");
        }

        fclose(file);
        return 0;
    }

    // Allocate memory for text
    char* text = malloc(file_size + 1);
    if (!text) {
        printf("Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    // Read file
    size_t bytes_read = fread(text, 1, file_size, file);
    text[bytes_read] = '\0';

    fclose(file);

    // Remove trailing newline characters
    while (bytes_read > 0 && (text[bytes_read - 1] == '\n' || text[bytes_read - 1] == '\r')) {
        text[--bytes_read] = '\0';
    }

    printf("Text read from file: '%s'\n", text);
    printf("Text length: %zu characters\n", strlen(text));

    // Ask for shift value
    printf("Please enter shift value (1-1000): ");
    char shift_str[100];
    if (fgets(shift_str, sizeof(shift_str), stdin) == NULL) {
        printf("Error reading shift value\n");
        free(text);
        return 1;
    }

    // Remove newline from input
    shift_str[strcspn(shift_str, "\n")] = '\0';

    // Validate input string (after removing newlines)
if (!is_valid_string(text)) {
    // If text is empty after trimming, that's OK
    if (strlen(text) == 0) {
        printf("File contains empty string (only whitespace/newlines)\n");
    }
    else {
        printf("Error: Text must contain only uppercase Latin letters (A-Z)\n");
        printf("Invalid text: '%s'\n", text);
        free(text);
        return 1;
    }
}

// Validate shift value
if (!is_valid_shift(shift_str)) {
    printf("Error: Shift must be an integer between 1 and 1000\n");
    free(text);
    return 1;
}

int shift = atoi(shift_str);
int text_len = strlen(text);
char* encrypted = malloc(text_len + 1);

if (!encrypted) {
    printf("Memory allocation failed\n");
    free(text);
    return 1;
}

// Perform encryption
caesar_encrypt(text, shift, encrypted);

// Output result
printf("\n=== Encryption Result ===\n");
if (text_len == 0) {
    printf("Original: (empty)\n");
    printf("Shift: %d\n", shift);
    printf("Encrypted: (empty)\n");
}
else {
    printf("Original: %s\n", text);
    printf("Shift: %d\n", shift);
    printf("Encrypted: %s\n", encrypted);
}

// Save to file
FILE* out = fopen("encrypted.txt", "w");
if (out) {
    fprintf(out, "%s", encrypted);
    fclose(out);
    printf("\nEncrypted text saved to 'encrypted.txt'\n");
}

free(text);
free(encrypted);

printf("\nPress Enter to exit...");
getchar();

return 0;
}
