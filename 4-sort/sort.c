#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100

// Function to compare strings for ascending order
int compareStringsAsc(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// Function to compare strings for descending order
int compareStringsDesc(const void *a, const void *b) {
    return strcmp(*(const char **)b, *(const char **)a);
}

int main(int argc, char *argv[]) {
    char *lines[MAX_LINES];
    char buffer[100];

    int i = 0;
    while (i < MAX_LINES && fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Remove newline character if present
        buffer[strcspn(buffer, "\n")] = '\0';

        lines[i] = strdup(buffer);
        i++;
    }

    if (argc == 2 && strcmp(argv[1], "-r") == 0) {
        // Sorting in descending order
        qsort(lines, i, sizeof(char *), compareStringsDesc);
    } else {
        // Sorting in ascending order
        qsort(lines, i, sizeof(char *), compareStringsAsc);
    }

    // Print the sorted lines
    for (int j = 0; j < i; j++) {
        printf("%s\n", lines[j]);
        free(lines[j]);  // Free dynamically allocated memory
    }

    return 0;
}
