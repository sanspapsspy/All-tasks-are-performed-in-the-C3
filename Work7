#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100
#define MAX_UNIT_LENGTH 10

typedef struct {
    double value;
    char unit[MAX_UNIT_LENGTH];
    double value_in_meters; // Converted value for sorting
} Measurement;

// Function to convert to meters
double convert_to_meters(double value, const char* unit) {
    if (strcmp(unit, "m") == 0) return value;
    if (strcmp(unit, "cm") == 0) return value / 100.0;
    if (strcmp(unit, "ya") == 0) return value * 0.9144;
    if (strcmp(unit, "ft") == 0) return value * 0.3048;
    if (strcmp(unit, "lok") == 0) return value * 0.445; // approximate
    if (strcmp(unit, "ver") == 0) return value * 0.04445; // approximate
    if (strcmp(unit, "arsh") == 0) return value * 0.7112; // approximate
    if (strcmp(unit, "sazh") == 0) return value * 2.1336; // approximate
    return value; // default to meters if unknown unit
}

// Comparison function for qsort
int compare_measurements(const void* a, const void* b) {
    const Measurement* ma = (const Measurement*)a;
    const Measurement* mb = (const Measurement*)b;

    if (ma->value_in_meters < mb->value_in_meters) return -1;
    if (ma->value_in_meters > mb->value_in_meters) return 1;
    return 0;
}

// Function to trim whitespace from string
char* trim_whitespace(char* str) {
    char* end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';
    return str;
}

// Function to read measurements from file
Measurement* read_measurements(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    Measurement* measurements = NULL;
    char line[MAX_LINE_LENGTH];
    *count = 0;
    int capacity = 10;

    measurements = malloc(capacity * sizeof(Measurement));
    if (!measurements) {
        fclose(file);
        return NULL;
    }

    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        // Skip empty lines
        if (strlen(trim_whitespace(line)) == 0) {
            continue;
        }

        // Parse line: <value> <unit>
        double value;
        char unit[MAX_UNIT_LENGTH];

        if (sscanf(line, "%lf %9s", &value, unit) == 2) {
            // Resize array if needed
            if (*count >= capacity) {
                capacity *= 2;
                Measurement* temp = realloc(measurements, capacity * sizeof(Measurement));
                if (!temp) {
                    free(measurements);
                    fclose(file);
                    return NULL;
                }
                measurements = temp;
            }

            // Store measurement
            measurements[*count].value = value;
            strncpy(measurements[*count].unit, unit, MAX_UNIT_LENGTH - 1);
            measurements[*count].unit[MAX_UNIT_LENGTH - 1] = '\0';
            measurements[*count].value_in_meters = convert_to_meters(value, unit);
            (*count)++;
        }
    }

    fclose(file);
    return measurements;
}

// Function to write measurements to file
int write_measurements(const char* filename, const Measurement* measurements, int count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return 0;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%.6f %s\n", measurements[i].value, measurements[i].unit);
    }

    fclose(file);
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    int count;

    // Read measurements from file
    Measurement* measurements = read_measurements(filename, &count);
    if (!measurements) {
        printf("Failed to read measurements from file\n");
        return 1;
    }

    if (count == 0) {
        printf("No valid measurements found in file\n");
        free(measurements);
        return 1;
    }

    // Sort measurements
    qsort(measurements, count, sizeof(Measurement), compare_measurements);

    // Write sorted measurements back to file
    if (!write_measurements(filename, measurements, count)) {
        printf("Failed to write sorted measurements to file\n");
        free(measurements);
        return 1;
    }

    printf("Successfully sorted %d measurements in file '%s'\n", count, filename);
    printf("Sorted measurements:\n");
    for (int i = 0; i < count; i++) {
        printf("%.6f %s (≈ %.6f m)\n",
            measurements[i].value,
            measurements[i].unit,
            measurements[i].value_in_meters);
    }

    free(measurements);
    return 0;
}
