#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define STATUS_FILE "/proc/%d/status"
#define BUFFER_SIZE 1024

// Function to read and print specific lines from /proc/[pid]/status file
void print_proc_status(pid_t pid) {
    char filename[BUFFER_SIZE];
    snprintf(filename, sizeof(filename), STATUS_FILE, pid);
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open status file");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the buffer

        // Print only the relevant lines
        if (strstr(buffer, "Name:") || strstr(buffer, "State:") ||
            strstr(buffer, "Tgid:") || strstr(buffer, "Pid:") ||
            strstr(buffer, "PPid:") || strstr(buffer, "Uid:") ||
            strstr(buffer, "Gid:") || strstr(buffer, "Nice:")) {
            printf("%s", buffer);
        }
    }
    close(fd);
}

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        printf("Child process with PID %d started with nice value 10.\n", getpid());
        
        // Set a nice value for the child process
        if (nice(10) == -1) {
            perror("Failed to set nice value");
            exit(EXIT_FAILURE);
        }

        // Infinite loop to keep the process alive for demonstration
        while (1) {
            printf("Child process is running with PID %d and nice value 10.\n", getpid());
            sleep(5); // Sleep to slow down the loop
        }
    } else { // Parent process
        printf("Parent process with PID %d waiting for child process %d to change nice value.\n", getpid(), pid);
        sleep(1); // Wait for the child to set its nice value
        
        // Print the /proc/[pid]/status information
        printf("\nReading /proc/%d/status:\n", pid);
        print_proc_status(pid);
        
        // Wait for child process to finish
        wait(NULL);
    }
    return 0;
}

