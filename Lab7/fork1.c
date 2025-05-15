#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork(); // Create a child process

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child Process (PID: %d) executing 'ls -lt'...\n", getpid());
        execl("/bin/ls", "ls", "-lt", (char *)NULL);

        // If execl fails
        perror("execl failed");
        exit(1);
    } else {
        // Parent process
        printf("Parent Process (PID: %d) waiting for child (PID: %d) to complete...\n", getpid(), pid);
        wait(NULL); // Wait for child to finish
        printf("Child process completed.\n");
    }

    return 0;
}
