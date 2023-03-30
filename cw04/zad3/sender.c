#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#define FILENAME "catcher_pid.txt"
#define SIGNAL SIGUSR1

int sent_singals_count=1;

void handler(int signo) {
    sent_singals_count++;
}
void send_singal(sigval_t value, pid_t pid){
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);

    struct sigaction sa;
    sa.sa_handler = handler;
    sigaction(SIGUSR1, &sa, NULL);

    // wysłanie sygnału do innego procesu
    sigqueue(pid, SIGUSR1, value);

    // oczekiwanie na potwierdzenie odebrania sygnału przez drugi proces
    sigsuspend(&mask);

    printf("Signal confirmed.\n");

}
int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Wrong input, try: ./program [mode1, mode2, ...].\n");
        exit(1);
    }

    // system("./catcher"); TODO:Zapytać czemu nie można by z tego skorzytać?

    FILE* fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    pid_t pid;
    fscanf(fp, "%d", &pid);
    fclose(fp);

    for (int processed_signal = 1; processed_signal < argc; processed_signal++) {
        sigval_t sig_val = {atoi(argv[processed_signal])};
        send_singal(sig_val, pid);
    }

    return 0;
}