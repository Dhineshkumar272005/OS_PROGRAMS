#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void find_files(const char *dir, const char *filename) {
    struct dirent *entry;
    struct stat statbuf;
    char path[1024];

    DIR *dp = opendir(dir);
    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        // Skip the current and parent directory entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);
        if (stat(path, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        // If it's a directory, recurse into it
        if (S_ISDIR(statbuf.st_mode)) {
            find_files(path, filename);
        } else if (strcmp(entry->d_name, filename) == 0) {
            printf("Found: %s\n", path);
        }
    }

    closedir(dp);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <directory> <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    find_files(argv[1], argv[2]);
    return 0;
}
