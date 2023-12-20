#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "addr_book_lib.h"

// Function to create an empty address book
struct addr_book* addr_book_create_empty(void) {
    struct addr_book* ab = malloc(sizeof(struct addr_book));
    if (ab != NULL) {
        ab->max_size = ADDR_BOOK_INIT_SIZE;
        ab->size = 0;
        ab->array = malloc(ab->max_size * sizeof(struct addr_book_item));
        if (ab->array == NULL) {
            free(ab);
            return NULL; // Allocation failure
        }
    }
    return ab;
}

// Function to delete an existing address book
void addr_book_delete(struct addr_book* ab) {
    free(ab->array);
    free(ab);
}

// Function to add an entry to the address book
int addr_book_add_item(struct addr_book* ab, const char* name,
                       const char* first_name, const char* date) {
    if (ab->size == ab->max_size) {
        // Double the size of the array if it's full
        ab->max_size *= 2;
        ab->array = realloc(ab->array, ab->max_size * sizeof(struct addr_book_item));
        if (ab->array == NULL) {
            return 1; // Allocation failure
        }
    }

    // Parse the date in the "dd.mm.yyyy" format
    int year, month, day;
    if (sscanf(date, "%d;%d;%d", &day, &month, &year) == 3) {
        sscanf(date, "%d.%d.%d", &day, &month, &year);
    }

    // Add the entry to the address book
    struct addr_book_item* entry = &ab->array[ab->size];
    snprintf(entry->name, ADDR_BOOK_NAME_MAX_LEN, "%s", name);
    snprintf(entry->first_name, ADDR_BOOK_FIRST_NAME_MAX_LEN, "%s", first_name);
    entry->birth_date.day = (uint8_t)day;
    entry->birth_date.month = (uint8_t)month;
    entry->birth_date.year = (uint32_t)year;

    ab->size++;
    return 0; // Success
}

// Function to get the effective number of entries in the address book
size_t addr_book_size(const struct addr_book* ab) {
    return ab->size;
}

// Function to get a pointer to the element at the specified index
struct addr_book_item* addr_book_get_element_at(struct addr_book* ab, size_t index) {
    if (index < ab->size) {
        return &ab->array[index];
    }
    return NULL;
}

// Function to remove the element at the specified index
int addr_book_remove_element_at(struct addr_book* ab, size_t index) {
    if (index >= ab->size) {
        return 1; // Index out of bounds
    }

    // Shift elements to fill the gap
    for (size_t i = index; i < ab->size - 1; i++) {
        ab->array[i] = ab->array[i + 1];
    }
    ab->size--;
    return 0; // Success
}

// Function to print the address book to a stream
void addr_book_print(FILE* stream, const struct addr_book* ab) {
    for (size_t i = 0; i < ab->size; i++) {
        fprintf(stream, "Name: %s, First Name: %s, Birth Date: %02d.%02d.%04d\n",
                ab->array[i].name, ab->array[i].first_name,
                ab->array[i].birth_date.day, ab->array[i].birth_date.month,
                ab->array[i].birth_date.year);
    }
}

// Function to save the address book to a CSV file
int addr_book_save(const char* filename, const struct addr_book* ab) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return 1; // Failed to open the file
    }

    for (size_t i = 0; i < ab->size; i++) {
        fprintf(file, "Name: %s, First Name: %s, Birth Date: %02d.%02d.%04d\n",
                ab->array[i].name, ab->array[i].first_name,
                ab->array[i].birth_date.day, ab->array[i].birth_date.month,
                ab->array[i].birth_date.year);
    }
    fclose(file);
    return 0; // Success
}

// Function to remove all elements with a given name
int addr_book_remove_element_with_name(struct addr_book* ab, const char* name) {
    int removed = 0;

    for (size_t i = 0; i < ab->size; i++) {
        if (strcmp(ab->array[i].name, name) == 0) {
            // Shift elements to fill the gap
            for (size_t j = i; j < ab->size - 1; j++) {
                ab->array[j] = ab->array[j + 1];
            }
            ab->size--;
            removed = 1;
            i--; // Revisit the same index since the elements are shifted
        }
    }
    return removed ? 0 : 1; // Success if at least one element is removed
}

// Function to create an address book from a file
struct addr_book* addr_book_create_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL; // File opening error
    }

    struct addr_book* ab = addr_book_create_empty();
    if (ab == NULL) {
        fclose(file);
        perror("Memory allocation error");
        return NULL; // Memory allocation error
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        char name[ADDR_BOOK_NAME_MAX_LEN];
        char first_name[ADDR_BOOK_FIRST_NAME_MAX_LEN];
        int day, month, year;

        if (sscanf(line, "%[^;];%[^;];%d;%d;%d", name, first_name, &day, &month, &year) == 5) {
            // Add the entry to the address book
            printf("Adding entry: %s %s %02d;%02d;%02d\n", name, first_name, day, month, year);
            int result = addr_book_add_item(ab, name, first_name, line);
            if (result != 0) {
                perror("Error adding entry to address book");
                fclose(file);
                addr_book_delete(ab);
                return NULL;
            }
        } else {
            // Handle line parsing error
            fprintf(stderr, "Error parsing line: %s", line);
        }
    }

    // Check if any errors occurred during the loop
    if (ferror(file) != 0) {
        perror("Error reading file");
        fclose(file);
        addr_book_delete(ab);
        return NULL;
    }

    fclose(file);
    return ab;
}

// Function to create an address book from entries with a specific name
struct addr_book* addr_book_create_from_select_name(const struct addr_book* ab_source, const char* name) {
    struct addr_book* selected_ab = addr_book_create_empty();
    if (selected_ab == NULL) {
        return NULL; // Memory allocation error
    }

    for (size_t i = 0; i < ab_source->size; i++) {
        if (strcmp(ab_source->array[i].name, name) == 0) {
            addr_book_add_item(selected_ab, ab_source->array[i].name, ab_source->array[i].first_name, "01;01;1970");
        }
    }
    return selected_ab;
}
