#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

void grep_file(const char *filename, const char *search_string) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    char buffer[BUFFER_SIZE];
    int line_number = 0;

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        line_number++;
        if (strstr(buffer, search_string) != NULL) {
            printf("Line %d: %s", line_number, buffer);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <search_string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    grep_file(argv[1], argv[2]);
    return 0;
}

