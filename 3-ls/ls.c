#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

void listDirectory();

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

void listDirectory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;

    dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (stat(fullpath, &info) != 0) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        printf("%s", entry->d_name);

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
    }

    closedir(dir);
}

