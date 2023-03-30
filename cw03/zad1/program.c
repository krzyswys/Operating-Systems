#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("oo few arguments, try: %s number of processes\n", argv[0]);
        return 1;
    }
    if (argc > 2) {
        printf("Too many arguments, try: %s number of processes\n", argv[0]);
        return 1;
    }

    int num_children = atoi(argv[1]);

    for (int i = 0; i < num_children; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork() error");
            continue;
        } else if (pid == 0) {
            printf("Parent process ID: %d, child process ID: %d\n", getppid(), getpid());
            return 0;

        }
    }

    for (int i = 0; i < num_children; i++) {
        wait(NULL);
    }

    printf("%s\n", argv[1]);

    return 0;
}
