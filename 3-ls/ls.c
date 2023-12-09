#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void sorting(const char *path);
void listPreviousFolder();
void listFile(const char *filename);

int main(int argc, char **argv) {
    if (argc <= 1) {
        sorting("."); // Default to current directory if no argument is provided
        return 0;
    }

    if (argc == 2 && strcmp(argv[1], "..") == 0) {
        printf("You typed in <..>\n");
        listPreviousFolder();
    } else if (argc == 2 && access(argv[1], F_OK) == 0) {
        printf("You typed in a filename. Listing the file:\n");
        listFile(argv[1]);
    } else if (argc == 2) {
        printf("You typed in a directory path. Listing the contents:\n");
        sorting(argv[1]); // Call sorting with the specified directory path
    } else {
        printf("Usage: %s <You may use a parameter like .. or provide a filename or a directory path>\n", argv[0]);
        return 1;
    }

    return 0;
}

void sorting(const char *path) {
    DIR *dir;
    struct dirent *ent;

    char **fileNames;
    char **fileTypes;
    int length = 0;

    if ((dir = opendir(path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }
            length++;
        }
        closedir(dir);

        fileNames = (char **)malloc(length * sizeof(char *));
        fileTypes = (char **)malloc(length * sizeof(char *));
        for (int i = 0; i < length; i++) {
            fileNames[i] = (char *)malloc(256 * sizeof(char));
            fileTypes[i] = (char *)malloc(2 * sizeof(char));
        }

        dir = opendir(path);
        int i = 0;
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            strcpy(fileNames[i], ent->d_name);

            struct stat st;
            char fullpath[512]; // Assuming maximum path length of 512 characters
            snprintf(fullpath, sizeof(fullpath), "%s/%s", path, ent->d_name);

            if (lstat(fullpath, &st) == 0) {
                if (S_ISDIR(st.st_mode)) {
                    strcpy(fileTypes[i], "/");
                } else if (S_ISLNK(st.st_mode)) {
                    strcpy(fileTypes[i], "@");
                } else if (st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH) {
                    strcpy(fileTypes[i], "*");
                } else {
                    strcpy(fileTypes[i], "");
                }
            } else {
                strcpy(fileTypes[i], "");
            }

            i++;
        }
        closedir(dir);

        // Sort the entries case-insensitively and alphabetically
        for (int i = 0; i < length - 1; i++) {
            for (int j = i + 1; j < length; j++) {
                if (strcasecmp(fileNames[j], fileNames[i]) < 0 || (strcasecmp(fileNames[j], fileNames[i]) == 0 && strcmp(fileTypes[j], fileTypes[i]) < 0)) {
                    // Swap fileNames
                    char temp[256];
                    strcpy(temp, fileNames[i]);
                    strcpy(fileNames[i], fileNames[j]);
                    strcpy(fileNames[j], temp);

                    // Swap fileTypes
                    strcpy(temp, fileTypes[i]);
                    strcpy(fileTypes[i], fileTypes[j]);
                    strcpy(fileTypes[j], temp);
                }
            }
        }

        // Print the sorted entries
        for (int i = 0; i < length; i++) {
            printf("%s%s\n", fileNames[i], fileTypes[i]);
        }

        // Free dynamically allocated memory
        for (int i = 0; i < length; i++) {
            free(fileNames[i]);
            free(fileTypes[i]);
        }
        free(fileNames);
        free(fileTypes);
    } else {
        perror("Error opening directory");
    }
}

void listPreviousFolder() {
    struct dirent **fileList;
    int n;

    n = scandir("..", &fileList, NULL, alphasort);
    if (n < 0) {
        perror("Error opening the previous folder");
        return;
    }

    for (int i = 0; i < n; ++i) {
        if (strcmp(fileList[i]->d_name, ".") != 0 && strcmp(fileList[i]->d_name, "..") != 0) {
            printf("%s\n", fileList[i]->d_name);
        }
        free(fileList[i]);
    }

    free(fileList);
}

void listFile(const char *filename) {
    printf("%s\n", filename);
}
