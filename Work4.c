#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Function to validate integer input
int get_valid_integer(int min, int max) {
    char input[100];
    int value;

    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error occurred.\n");
            return -1;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = '\0';

        // Check if input contains only digits
        int valid = 1;
        for (int i = 0; input[i] != '\0'; i++) {
            if (input[i] < '0' || input[i] > '9') {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf("Please enter a valid integer between %d and %d: ", min, max);
            continue;
        }

        value = atoi(input);
        if (value < min || value > max) {
            printf("Please enter a number between %d and %d: ", min, max);
            continue;
        }

        break;
    }

    return value;
}

int main() {
    int N, secret_number, guess, attempts = 0;
    int low, high;
    char strategy_choice;

    srand(time(NULL));

    printf("=== Number Guessing Game ===\n");

    // Get N from user
    printf("Enter the maximum number N (1 or greater): ");
    N = get_valid_integer(1, 1000000);

    // Generate secret number
    secret_number = rand() % N + 1;

    printf("I'm thinking of a number between 1 and %d.\n", N);
    printf("Would you like to use efficient strategy? (y/n): ");

    // Get strategy choice
    while (1) {
        if (fgets(&strategy_choice, 2, stdin) == NULL) {
            printf("Input error occurred.\n");
            return 1;
        }
        getchar(); // Clear newline

        if (strategy_choice == 'y' || strategy_choice == 'Y' ||
            strategy_choice == 'n' || strategy_choice == 'N') {
            break;
        }
        printf("Please enter 'y' for yes or 'n' for no: ");
    }

    if (strategy_choice == 'y' || strategy_choice == 'Y') {
        printf("Using binary search strategy for efficient guessing.\n");
        printf("Think of your number and I'll guess it!\n");

        low = 1;
        high = N;
        attempts = 0;

        while (low <= high) {
            guess = low + (high - low) / 2;
            attempts++;

            printf("My guess #%d: %d\n", attempts, guess);
            printf("Is it correct? (c), too low? (l), too high? (h): ");

            char feedback;
            while (1) {
                if (fgets(&feedback, 2, stdin) == NULL) {
                    printf("Input error occurred.\n");
                    return 1;
                }
                getchar(); // Clear newline

                if (feedback == 'c' || feedback == 'C' ||
                    feedback == 'l' || feedback == 'L' ||
                    feedback == 'h' || feedback == 'H') {
                    break;
                }
                printf("Please enter 'c', 'l', or 'h': ");
            }

            if (feedback == 'c' || feedback == 'C') {
                printf("I guessed your number %d in %d attempts!\n", guess, attempts);
                break;
            }
            else if (feedback == 'l' || feedback == 'L') {
                low = guess + 1;
            }
            else if (feedback == 'h' || feedback == 'H') {
                high = guess - 1;
            }
        }
    }
    else {
        printf("Try to guess my number!\n");
        attempts = 0;

        do {
            printf("Enter your guess: ");
            guess = get_valid_integer(1, N);
            attempts++;

            if (guess < secret_number) {
                printf("LT\n");
            }
            else if (guess > secret_number) {
                printf("GT\n");
            }
            else {
                printf("EQ\n");
                printf("Congratulations! You guessed the number %d in %d attempts!\n",
                    secret_number, attempts);
            }
        } while (guess != secret_number);
    }

    printf("Efficient strategy explanation:\n");
    printf("Binary search reduces search space by half each time.\n");
    printf("For range 1..%d, maximum attempts needed: %d\n",
        N, (int)(log(N) / log(2)) + 1);
    printf("Time complexity: O(log N)\n");
    //добавить проверку при доходе в диапозоне
    return 0;
}
