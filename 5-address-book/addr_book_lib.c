/*
Author:	Tim Bütler
Date:	20.12.2023

Functionality Overview:
	The address book management system is designed to handle the creation, deletion,
	and manipulation of entries in a digital address book.

1. addr_book_create_empty:
   Purpose:	Allocates memory for an empty address book.
   Usage: 	Initialize a new address book.

2. addr_book_delete:
   Purpose: Deallocates memory used by an address book.
   Usage: 	Clean up and free resources when done with the address book.

3. addr_book_add_item:
   Purpose: Adds a new entry to the address book, dynamically resizing if necessary.
   Usage: 	Populate the address book with contact information.

4. addr_book_size:
   Purpose: Returns the number of entries in the address book.
   Usage: 	Obtain the current size of the address book.

5. addr_book_get_element_at:
   Purpose: Returns a pointer to the entry at the specified index.
   Usage: 	Access and manipulate specific entries in the address book.

6. addr_book_remove_element_at:
   Purpose: Removes an entry at the specified index, shifting remaining entries.
   Usage: 	Delete a specific entry from the address book.

7. addr_book_print:
   Purpose: Prints the contents of the address book to a specified stream (e.g., stdout, file).
   Usage: 	Display the address book's entries for user or debugging purposes.

8. addr_book_save:
   Purpose: Saves the address book entries to a CSV file.
   Usage: 	Persistently store address book data for future use.

9. addr_book_remove_element_with_name:
   Purpose: Removes all entries with a specified name from the address book.
   Usage:	 Delete multiple entries based on a common name.

10.addr_book_create_from_file:
   Purpose: Reads entries from a CSV file, creating an address book.
   Usage: 	Populate the address book from a file, facilitating data import.

11.addr_book_create_from_select_name:
   Purpose: Creates a new address book with entries that match a specific name.
   Usage: 	Generate a subset of the address book containing entries with a given name.

Disclaimer:
	For additional researches the internet has been used: Websites like stackoverflow and reddit - just in case :)

*/

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
int addr_book_add_item(struct addr_book* ab, const char* name, const char* first_name, const char* date) {
    int day, month, year;

	if (NULL == ab->array) {
		free(ab);
	}

    if (ab->size == ab->max_size) {
        // Double the size of the array if it's full
        ab->max_size *= 2;
        ab->array = realloc(ab->array, ab->max_size * sizeof(struct addr_book_item));
        if (ab->array == NULL) {
            return 1; // Allocation failure
        }
    }

   // Use sscanf to dynamically determine the date format
    if (sscanf(date, "%d.%d.%d", &day, &month, &year) == 3) {
        // Date format: day.month.year
    } else if (sscanf(date, "%d;%d;%d", &year, &month, &day) == 3) {
        // Date format: year;month;day
    }

    // Add the entry to the address book
    struct addr_book_item* entry = &ab->array[ab->size];

    //snprintf was suggested by the forums. It is secure print. It has a fixed length.
    snprintf(entry->name, ADDR_BOOK_NAME_MAX_LEN + 1, "%s", name);
    snprintf(entry->first_name, ADDR_BOOK_FIRST_NAME_MAX_LEN + 1, "%s", first_name);

    entry->birth_date.day = day;
    entry->birth_date.month = month;
    entry->birth_date.year = year;

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
        // fprintf(stream, "Name: %s, First Name: %s, Birth Date: %02d.%02d.%04d\n",
		fprintf(stream, "%s;%s;%d;%d;%d\n",
			ab->array[i].name,
			ab->array[i].first_name,
			ab->array[i].birth_date.year,
			ab->array[i].birth_date.month,
			ab->array[i].birth_date.day);
    }
}

// Function to save the address book to a CSV file
int addr_book_save(const char* filename, const struct addr_book* ab) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return 1; // Failed to open the file
    }

    for (size_t i = 0; i < ab->size; i++) {
        // fprintf(file, "Name: %s, First Name: %s, Birth Date: %02d.%02d.%04d\n",
        fprintf(file, "%s;%s;%d;%d;%d\n",
                ab->array[i].name,
                ab->array[i].first_name,
                ab->array[i].birth_date.year,
                ab->array[i].birth_date.month,
                ab->array[i].birth_date.day);
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

// Function to parse a date with flexible formats
int parse_date(const char* date_str, int* year, int* month, int* day) {
    // Attempt to extract year, month, and day separately
    int result = sscanf(date_str, "%d.%d.%d", day, month, year);
    if (result != 3) {
        result = sscanf(date_str, "%d;%d;%d", year, month, day);
        if (result != 3) {
            // Add more formats as needed
            return 1; // Parsing failed
        }
    }

    return 0; // Parsing successful
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
        int year, month, day;

        if (sscanf(line, "%[^;];%[^;];%s", name, first_name, line) == 3) {
            // Try to parse the date with flexible formats
            if (parse_date(line, &year, &month, &day) == 0) {
                // Add the entry to the address book
                int result = addr_book_add_item(ab, name, first_name, line);
                if (result != 0) {
                    perror("Error adding entry to address book");
                    fclose(file);
                    addr_book_delete(ab);
                    return NULL;
                }
            } else {
                // Handle date parsing error
                fprintf(stderr, "Error parsing date in line: %s", line);
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
struct addr_book* addr_book_create_from_select_name(const struct addr_book* ab, const char* name) {
    struct addr_book* selected_ab = addr_book_create_empty();

    for (size_t i = 0; i < ab->size; i++) {
        if (strcmp(ab->array[i].name, name) == 0) {
            // Copy the entire entry, including the date
            addr_book_add_item(selected_ab, ab->array[i].name, ab->array[i].first_name, "");

            // Copy the birth date from the original address book
            selected_ab->array[selected_ab->size - 1].birth_date = ab->array[i].birth_date;
        }
    }

    return selected_ab;
}
