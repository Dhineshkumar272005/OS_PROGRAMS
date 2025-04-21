#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 1024

int main(int argc, char *argv[]) {
    FILE *fp;
    char fline[BUFFER];
    int line = 1;

    // Check for correct number of arguments
    if (argc != 3) {
        printf("Please use the following syntax:\n");
        printf("./a.out filename searchstring\n");
        printf("Example: ./a.out test1.c print\n");
        exit(1);
    }

    // argv[1] is filename
    if (!(fp = fopen(argv[1], "r"))) {
        printf("grep: Could not open file: %s\n", argv[1]);
        exit(1);
    }

    // Read lines from the file
    while (fgets(fline, BUFFER, fp) != NULL) {
        // argv[2] is search string
        if (strstr(fline, argv[2]) != NULL) {
            printf("Line number: %d text: %s", line, fline);
        }
        line++;
    }

    fclose(fp);
    return 0;
}
