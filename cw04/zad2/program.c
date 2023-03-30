#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

#define SIGNAL SIGUSR1
#define MAX_CALLS 4

int depth;


void depth_handler(int signo, siginfo_t* info, void* context) {
    printf("START: PID=%d, current depth=%d\n", getpid(), depth);

    depth++;
    if (depth < MAX_CALLS)
    {
        raise(SIGUSR1);
    }else{
        printf("PID:%d, can't raise: depth>=MAX_CALLS\n", getpid());
    }
    depth--;

    printf("END: PID=%d, current depth=%d\n", getpid(), depth);
}

void NODEFER() {
    printf("NODEFER test: \n");
    struct sigaction action;
    depth = 0;

    sigemptyset(&action.sa_mask);

    action.sa_sigaction = depth_handler;
    action.sa_flags = SA_NODEFER;

    sigaction(SIGNAL, &action, NULL);

    raise(SIGNAL);

    printf("\n");

}
void RESETHAND() {
    printf("RESETHAND test: \n");
    struct sigaction action;
    depth = 0;

    sigemptyset(&action.sa_mask);

    action.sa_sigaction = depth_handler;
    action.sa_flags = SA_RESETHAND;
    sigaction(SIGNAL, &action, NULL);

    raise(SIGNAL);
    raise(SIGNAL);

    printf("\n");
}


void info_handler(int signo, siginfo_t* info, void* context) {
    printf("Signal number: %d\n",  info->si_signo);
    printf("Singal has been sent by proces of id: %d\n",   info->si_pid);
    printf("Processor time used in user mode:%ld\n",   info->si_utime);
    printf("Custom value sent: %d\n",  info->si_value.sival_int);
    printf("Status of exit: %x\n", info->si_status);
}
void SIGINFO() {
    printf("SIGINFO test in parent: \n");
    struct sigaction action;

    sigemptyset(&action.sa_mask);

    action.sa_sigaction = info_handler;
    action.sa_flags = SA_SIGINFO;

    sigaction(SIGNAL, &action, NULL);

    raise(SIGNAL);
    printf("\n");

    printf("SIGINFO test in child: \n");
    if (fork() == 0) {
        raise(SIGNAL);
        exit(0);
    }
    else{
        wait(NULL);
    }
    printf("\n");

    printf("SIGINFO own test: \n");

    sigemptyset(&action.sa_mask);

    action.sa_sigaction = info_handler;
    action.sa_flags = SA_SIGINFO;

    sigaction(SIGNAL, &action, NULL);

    sigval_t sig_value = {2137};
    sigqueue(getpid(), SIGNAL, sig_value);
    printf("\n");
}



int main(int argc, char** argv) {
    if (argc != 1) {
        fprintf(stderr, "Wrong input, try: ./program.\n");
        exit(1);
    }
    SIGINFO();
    NODEFER();
    RESETHAND();
    return 0;
}