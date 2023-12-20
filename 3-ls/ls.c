#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

// declare functions we will use later
int compareStrings(const void *a, const void *b);
void listDirectory(const char *path);

int main(int argc, char *argv[]) {
    const char *path;

	// first check for arguments so we work further with the right args
    // Check if a parameter is provided
    if (argc == 1) {
        // No parameter, list the current directory
        path = ".";
    } else if (argc == 2) {
        // Parameter given, list the specified directory or file
        path = argv[1];
    } else {
        // Incorrect usage
        fprintf(stderr, "Usage: %s [directory or file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // List directory contents
    listDirectory(path);

    return 0;
}

// Function to compare strings for sorting
int compareStrings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// Function to list directory contents
void listDirectory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    char **entries;
    int numEntries = 0;

    // Open the directory
    dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    // Count the number of entries (excluding special directories and those starting with a dot)
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&
            entry->d_name[0] != '.') {
            numEntries++;
        }
    }

    // Allocate memory for entries
    entries = (char **)malloc(numEntries * sizeof(char *));
    if (entries == NULL) {
        perror("Memory allocation error");
        closedir(dir);
        exit(EXIT_FAILURE);
    }

    // Reset directory stream position
    rewinddir(dir);

    // Store entries in the array
    int index = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&
            entry->d_name[0] != '.') {
            entries[index] = strdup(entry->d_name);
            index++;
        }
    }

    // Close the directory
    closedir(dir);

    // Sort the entries
    qsort(entries, numEntries, sizeof(char *), compareStrings);

    // Print the sorted entries
    for (int i = 0; i < numEntries; i++) {
        char fullpath[PATH_MAX];

        // Check for path length
        // snprinf was suggested by the internet. It is secure print. It prevents buffer overflow and
        // I can set the maximum number of chars that can be written to the buffer
        if (snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entries[i]) >= PATH_MAX) {
            fprintf(stderr, "Path length exceeds maximum limit\n");
            continue;
        }

        // Get file information
        if (lstat(fullpath, &info) != 0) {
            perror("Error getting file information");
            // Continue to the next entry
            continue;
        }

        // Print entry name
        // however somehow the order is important, or maybe use cases?
        printf("%s", entries[i]);

        // Append / for directories
        if (S_ISDIR(info.st_mode)) {
            printf("/");
        }
		// Append @ for symbolic links
		else if (S_ISLNK(info.st_mode)) {
			printf("@");
		}
        // Append * for executable files
        else if (info.st_mode & S_IXUSR) {
            printf("*");
        }
        printf("\n");

        // Free dynamically allocated memory
        free(entries[i]);
    }
    free(entries);
}