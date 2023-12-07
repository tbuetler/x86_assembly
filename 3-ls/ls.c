#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define MAX_ENTRIES 1000

typedef struct {
    char name[PATH_MAX];
    char symbol;
} Entry;

int compareEntries(const void *a, const void *b) {
    return strcmp(((Entry *)a)->name, ((Entry *)b)->name);
}

void listDirectory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;

    Entry entries[MAX_ENTRIES];
    int entryCount = 0;

    dir = opendir(path);

    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL && entryCount < MAX_ENTRIES) {
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (lstat(fullpath, &info) == -1) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        strncpy(entries[entryCount].name, entry->d_name, sizeof(entries[entryCount].name) - 1);
        entries[entryCount].name[sizeof(entries[entryCount].name) - 1] = '\0';

        if (S_ISDIR(info.st_mode)) {
            entries[entryCount].symbol = '/';
        } else if (S_ISREG(info.st_mode)) {
            entries[entryCount].symbol = (info.st_mode & S_IXUSR) ? '*' : '\0';
        } else if (S_ISLNK(info.st_mode)) {
            entries[entryCount].symbol = '@';
        } else {
            entries[entryCount].symbol = '\0';
        }

        entryCount++;
    }

    closedir(dir);

    qsort(entries, entryCount, sizeof(Entry), compareEntries);

    for (int i = 0; i < entryCount; i++) {
        printf("%s%c\n", entries[i].name, entries[i].symbol);
    }
}

int main(int argc, char *argv[]) {
    const char *path;

    if (argc == 1) {
        // No parameter provided, list contents of current directory
        path = ".";
    } else if (argc == 2) {
        // Parameter provided, list contents of specified directory or file
        path = argv[1];
    } else {
        fprintf(stderr, "Usage: %s [directory or file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    listDirectory(path);

    return EXIT_SUCCESS;
}
