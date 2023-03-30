#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

const int SIGNAL_ID = SIGUSR1;

void handle_signal(int sigtype)
{
    printf("Received signal %d in handler\n", sigtype);
}

void check_signal_pending()
{
    sigset_t pending_set;
    sigpending(&pending_set);

    if (sigismember(&pending_set, SIGNAL_ID))
    {
        printf("Signal with id %d is pending.\n", SIGNAL_ID); //TODO: co zrobi wywołanie getP/pid()?
    }
    else
    {
        printf("Signal with id %d is NOT pending.\n", SIGNAL_ID);
    }
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Wrong input, try: ignore|handler|mask|ending\n");
        return 1;
    }


    if (strcmp(argv[1], "ignore") == 0) {
            signal(SIGNAL_ID, SIG_IGN);
        }
    else if (strcmp(argv[1], "handler") == 0) {
            signal(SIGNAL_ID, handle_signal);
       }
    else if (strcmp(argv[1], "pending") == 0 || strcmp(argv[1], "mask") == 0) {
            sigset_t blocked_set;
            sigemptyset(&blocked_set);
            sigaddset(&blocked_set, SIGNAL_ID);
            sigprocmask(SIG_BLOCK, &blocked_set, NULL);
        }
    else {
            printf("Invalid argument: %s\n", argv[1]);
        }

    printf("Raising singal: %d in main process of id: %d\n",SIGNAL_ID, getppid());
    raise(SIGNAL_ID);

    printf("Main process (%d): ", getppid());
    check_signal_pending();

    int pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "Failed to fork.");
        exit(1);
    }

    if (pid == 0)
    {
        if (strcmp(argv[1], "pending") != 0)
        {
            printf("Raising singal: %d in child process of id: %d\n",SIGNAL_ID, getpid());
            raise(SIGNAL_ID);
        }

        printf("Child process (%d): ", getpid());
        check_signal_pending();

        if (execl("./exec", "exec", NULL) == -1)
        {
            fprintf(stderr, "Couldn't run sub-program 'other'. Make sure to compile it first.");
            exit(1);
        }
    }

    int status = 0;
    while (wait(&status) > 0){};
    return 0;
}