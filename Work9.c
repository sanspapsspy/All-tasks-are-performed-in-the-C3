#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define CODE_LENGTH 4

// Function to generate random code with unique digits
void generate_code(int* code) {
    bool used[10] = { false };

    for (int i = 0; i < CODE_LENGTH; i++) {
        int digit;
        do {
            digit = rand() % 10;
        } while (used[digit]);

        code[i] = digit;
        used[digit] = true;
    }
}

// Function to validate user input
bool is_valid_guess(const char* input) {
    if (strlen(input) != CODE_LENGTH) {
        return false;
    }

    bool used[10] = { false };

    for (int i = 0; i < CODE_LENGTH; i++) {
        if (!isdigit(input[i])) {
            return false;
        }

        int digit = input[i] - '0';
        if (used[digit]) {
            return false; // Duplicate digit
        }
        used[digit] = true;
    }

    return true;
}

// Function to calculate bulls and cows
void calculate_bulls_and_cows(const int* code, const int* guess, int* bulls, int* cows) {
    *bulls = 0;
    *cows = 0;

    bool code_used[10] = { false };
    bool guess_used[10] = { false };

    // Count bulls (correct digits in correct positions)
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (code[i] == guess[i]) {
            (*bulls)++;
            code_used[code[i]] = true;
            guess_used[guess[i]] = true;
        }
    }

    // Count cows (correct digits in wrong positions)
    for (int i = 0; i < CODE_LENGTH; i++) {
        for (int j = 0; j < CODE_LENGTH; j++) {
            if (i != j && code[i] == guess[j] &&
                !code_used[code[i]] && !guess_used[guess[j]]) {
                (*cows)++;
                code_used[code[i]] = true;
                guess_used[guess[j]] = true;
                break;
            }
        }
    }
}

// Function to convert string to integer array
void string_to_digit_array(const char* str, int* arr) {
    for (int i = 0; i < CODE_LENGTH; i++) {
        arr[i] = str[i] - '0';
    }
}

int main() {
    int secret_code[CODE_LENGTH];
    int user_guess[CODE_LENGTH];
    char input[100];
    int attempts = 0;

    srand(time(NULL));

    printf("=== Bulls and Cows Game ===\n");
    printf("I've generated a %d-digit code with unique digits (0-9).\n", CODE_LENGTH);
    printf("Try to guess it!\n");
    printf("Bulls = correct digits in correct positions\n");
    printf("Cows = correct digits in wrong positions\n\n");

    // Generate secret code
    generate_code(secret_code);

    // Debug: uncomment to see the secret code
    // printf("Secret code: ");
    // for (int i = 0; i < CODE_LENGTH; i++) {
    //     printf("%d", secret_code[i]);
    // }
    // printf("\n");

    printf("Enter your %d-digit guess (all digits must be unique):\n", CODE_LENGTH);

    // Game loop
    while (true) {
        printf("Attempt %d: ", attempts + 1);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error occurred.\n");
            return 1;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = '\0';

        // Validate input
        if (!is_valid_guess(input)) {
            printf("Invalid input! Please enter %d unique digits (0-9): ", CODE_LENGTH);
            continue;
        }

        // Convert input to digit array
        string_to_digit_array(input, user_guess);
        attempts++;

        // Calculate bulls and cows
        int bulls, cows;
        calculate_bulls_and_cows(secret_code, user_guess, &bulls, &cows);

        printf("Result: %d bulls, %d cows\n", bulls, cows);

        // Check for win
        if (bulls == CODE_LENGTH) {
            printf("\nCongratulations! You guessed the code in %d attempts!\n", attempts);
            printf("The secret code was: ");
            for (int i = 0; i < CODE_LENGTH; i++) {
                printf("%d", secret_code[i]);
            }
            printf("\n");
            break;
        }

        // Provide hints after several attempts
        if (attempts == 5) {
            printf("Hint: Try to identify which digits are in the code first.\n");
        }
        else if (attempts == 10) {
            printf("Hint: Focus on the positions of bulls you've found.\n");
        }
    }

    printf("\nGame statistics:\n");
    printf("Total attempts: %d\n", attempts);

    if (attempts <= 6) {
        printf("Excellent performance!\n");
    }
    else if (attempts <= 12) {
        printf("Good job!\n");
    }
    else {
        printf("Keep practicing!\n");
    }

    printf("\nStrategy tips:\n");
    printf("1. Start with a guess that covers many different digits\n");
    printf("2. Use bulls information to lock in correct positions\n");
    printf("3. Use cows information to identify correct digits in wrong positions\n");
    printf("4. Eliminate digits that don't appear in any bulls or cows\n");

    return 0;
}
