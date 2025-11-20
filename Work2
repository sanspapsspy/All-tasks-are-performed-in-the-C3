#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUBJECTS 10
#define MAX_STRING_LENGTH 100

typedef struct Student {
    char* first_name;
    char* last_name;
    char* group;
    int* grades;
    int grades_count;
    double average_grade;
} Student;

typedef struct Node {
    Student student;
    struct Node* next;
} Node;

// Function to calculate average grade
double calculate_average(const int* grades, int count) {
    if (count == 0) return 0.0;

    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += grades[i];
    }
    return (double)sum / count;
}

// Function to create a copy of student
Student copy_student(const Student* src) {
    Student dest;

    // Copy strings
    dest.first_name = malloc(strlen(src->first_name) + 1);
    dest.last_name = malloc(strlen(src->last_name) + 1);
    dest.group = malloc(strlen(src->group) + 1);

    if (!dest.first_name || !dest.last_name || !dest.group) {
        free(dest.first_name);
        free(dest.last_name);
        free(dest.group);
        dest.first_name = dest.last_name = dest.group = NULL;
        return dest;
    }

    strcpy(dest.first_name, src->first_name);
    strcpy(dest.last_name, src->last_name);
    strcpy(dest.group, src->group);

    // Copy grades
    dest.grades_count = src->grades_count;
    dest.grades = malloc(dest.grades_count * sizeof(int));
    if (!dest.grades) {
        free(dest.first_name);
        free(dest.last_name);
        free(dest.group);
        dest.first_name = dest.last_name = dest.group = NULL;
        return dest;
    }

    memcpy(dest.grades, src->grades, dest.grades_count * sizeof(int));
    dest.average_grade = src->average_grade;

    return dest;
}

// Function to free student memory
void free_student(Student* student) {
    free(student->first_name);
    free(student->last_name);
    free(student->group);
    free(student->grades);
}

// Function to free list
void free_list(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        free_student(&temp->student);
        free(temp);
    }
}

// String comparison function
int string_compare(const char* s1, const char* s2) {
    return strcmp(s1, s2);
}

// Function to insert node in sorted order
void insert_sorted(Node** head, const Student* student) {
    Node* new_node = malloc(sizeof(Node));
    if (!new_node) return;

    new_node->student = copy_student(student);
    new_node->next = NULL;

    if (*head == NULL || string_compare(student->group, (*head)->student.group) < 0) {
        new_node->next = *head;
        *head = new_node;
        return;
    }

    Node* current = *head;
    while (current->next != NULL &&
        string_compare(student->group, current->next->student.group) >= 0) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
}

// Main filtering function
Node* filter_and_sort_students(const Student* students, int count, double min_avg) {
    Node* head = NULL;

    for (int i = 0; i < count; i++) {
        if (students[i].average_grade >= min_avg) {
            insert_sorted(&head, &students[i]);
        }
    }

    return head;
}

// Function to print list
void print_list(const Node* head) {
    const Node* current = head;
    while (current) {
        printf("Name: %s %s, Group: %s, Average: %.2f\n",
            current->student.first_name,
            current->student.last_name,
            current->student.group,
            current->student.average_grade);
        current = current->next;
    }
}

// Example usage
int main() {
    // Create sample students
    Student students[3];

    // Student 1
    students[0].first_name = strdup("John");
    students[0].last_name = strdup("Doe");
    students[0].group = strdup("GroupB");
    students[0].grades_count = 3;
    students[0].grades = malloc(3 * sizeof(int));
    students[0].grades[0] = 5; students[0].grades[1] = 4; students[0].grades[2] = 5;
    students[0].average_grade = calculate_average(students[0].grades, 3);

    // Student 2
    students[1].first_name = strdup("Jane");
    students[1].last_name = strdup("Smith");
    students[1].group = strdup("GroupA");
    students[1].grades_count = 3;
    students[1].grades = malloc(3 * sizeof(int));
    students[1].grades[0] = 3; students[1].grades[1] = 3; students[1].grades[2] = 4;
    students[1].average_grade = calculate_average(students[1].grades, 3);

    // Student 3
    students[2].first_name = strdup("Bob");
    students[2].last_name = strdup("Johnson");
    students[2].group = strdup("GroupC");
    students[2].grades_count = 3;
    students[2].grades = malloc(3 * sizeof(int));
    students[2].grades[0] = 5; students[2].grades[1] = 5; students[2].grades[2] = 5;
    students[2].average_grade = calculate_average(students[2].grades, 3);

    double min_avg = 4.0;
    Node* filtered_list = filter_and_sort_students(students, 3, min_avg);

    printf("Students with average >= %.1f (sorted by group):\n", min_avg);
    print_list(filtered_list);

    // Cleanup
    free_list(filtered_list);
    for (int i = 0; i < 3; i++) {
        free_student(&students[i]);
    }

    return 0;
}
