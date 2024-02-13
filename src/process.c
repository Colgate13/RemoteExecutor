#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h> // Add this line to include errno.h

#define INITIAL_PROCESSES 3
#define EXTRA_PROCESSES 2

void spawn_processes(int n);
void sigchld_handler(int sig);

int main() {
    // Set up the SIGCHLD handler
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // Automatically restart system calls if possible
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // Initially spawn child processes
    spawn_processes(INITIAL_PROCESSES);

    // Main process loop
    while (1) {
        pause(); // Wait for signals
    }

    return 0;
}

void spawn_processes(int n) {
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            // Error handling
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Child process
            printf("Child process %d started\n", getpid());

            // Here you can set up IPC mechanisms as needed

            // Simulate some work
            sleep(10);

            // Exit normally
            printf("Child process %d exiting\n", getpid());
            exit(0);
        }
        // Parent process continues to spawn next child
    }
}

void sigchld_handler(int sig) {
    int saved_errno = errno;
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {
        // Child process exited, spawn two more
        spawn_processes(EXTRA_PROCESSES);
    }
    errno = saved_errno;
}
