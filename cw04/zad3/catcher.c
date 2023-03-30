#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIGNAL SIGUSR1
#define FILENAME "catcher_pid.txt"

int singal_value;
bool cought = true;
int request_count = 0; 

void get_time(){
    time_t now = time(NULL); 
    struct tm *timeinfo = localtime(&now);  
    char buffer[9]; 
    strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
    printf("Current time: %s\n", buffer);
}

void get_numbers() { 
    for (int i = 0; i < 100; i++){
        printf("%d, ", i+1);
    }
    printf("\n");
}


void get_requests() { 
    printf("Requests since start: %d\n", request_count);
}

void handle_timer(int sig)
{
    get_time();
}
void time_loop() {
    struct itimerval timer;

    // ustawienie czasu początkowego i odstępu czasowego
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // ustawienie obsługi sygnału SIGALRM
    struct sigaction sa;
    sa.sa_handler = handle_timer;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // uruchomienie timera
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        exit(1);
    }
}

void handler_signal(int signo, siginfo_t* info, void* context) {
    singal_value = info->si_status;
    request_count += 1;
    cought = false;
    kill(info->si_pid, SIGNAL);
}

int main() {

    FILE* fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d", getpid());
    fclose(fp);

    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = handler_signal;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGNAL, &action, NULL);
    

    printf("Gotta Catch'Em All, started, PID: %d\n", getpid());
    while (true) {
        if (!cought){
        
            if(singal_value==1){
                get_numbers();
            }
            else if(singal_value==2){
                get_time();
            }
            else if(singal_value==3){
                get_requests();
            }
            else if(singal_value==4){
                time_loop();
            }
            else if(singal_value==5){
                printf("Cought'em all (%d), finished\n", request_count);
                exit(0);
            }
            else{
                printf("Cought unmarked singal_value: %d, try [1,5]",singal_value);
            }
            cought = true;
            if (singal_value != 4){
                alarm(0);
            }
        }
    }

    return 0;
}
