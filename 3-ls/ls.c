#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

// Function to compare strings for sorting
int compareStrings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void listDirectory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    char **entries;
    int numEntries = 0;

    dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Count the number of entries (excluding "." and ".." and those starting with a dot)
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&
            entry->d_name[0] != '.') {
            numEntries++;
        }
    }

    // Allocate memory for entries
    entries = (char **)malloc(numEntries * sizeof(char *));
    if (entries == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    rewinddir(dir);  // Reset directory stream position

    // Store entries in the array
    int index = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&
            entry->d_name[0] != '.') {
            entries[index] = strdup(entry->d_name);
            index++;
        }
    }

    closedir(dir);

    // Sort the entries
    qsort(entries, numEntries, sizeof(char *), compareStrings);

    // Print the sorted entries
    for (int i = 0; i < numEntries; i++) {
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entries[i]);

        if (stat(fullpath, &info) != 0) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        printf("%s", entries[i]);

        // Append / for directories
        if (S_ISDIR(info.st_mode)) {
            printf("/");
        }
        // Append * for executable files
        else if (info.st_mode & S_IXUSR) {
            printf("*");
        }
        // Append @ for symbolic links
        else if (S_ISLNK(info.st_mode)) {
            printf("@");
        }

        printf("\n");

        free(entries[i]);  // Free dynamically allocated memory
    }

    free(entries);
}

int main(int argc, char *argv[]) {
    const char *path;

    if (argc == 1) {
        // No parameter, list the current directory
        path = ".";
    } else if (argc == 2) {
        // Parameter given, list the specified directory or file
        path = argv[1];
    } else {
        fprintf(stderr, "Usage: %s [directory or file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    listDirectory(path);

    return 0;
}
