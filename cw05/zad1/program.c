#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdlib.h>


int main(int argc, char** argv) {
    struct tm tm;
    if (argc==2){
        if (sscanf(argv[1], "%d.%d.%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year) == 3 ||
            sscanf(argv[1], "%d/%d/%d", &tm.tm_mon, &tm.tm_mday, &tm.tm_year) == 3 ||
            sscanf(argv[1], "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) == 3) {
                FILE* cowsay_input = popen("cowsay", "w");
                fwrite(argv[1], sizeof(char), strlen(argv[1]), cowsay_input);
                pclose(cowsay_input);
        }

        else if(!isalpha(argv[1])){
                FILE* cowsay_input = popen("cowsay", "w");
                fwrite(argv[1], sizeof(char), strlen(argv[1]), cowsay_input);
                pclose(cowsay_input);
        }
        else{
            printf("Wrong input, try: nadawca/data: text/(day./-mon./-year)  %d\n", argc);
            exit(1);
        }
    }
    else if(argc ==4){

        char *message = NULL;
        size_t len = strlen(argv[1]) + strlen(argv[2]) + strlen(argv[3]) + 1;
        message = malloc(len);
        if (message == NULL) {
            printf("Failed to allocate memory.\n");
            return -1;
        }
        snprintf(message, len, "%s%s%s", argv[1], argv[2], argv[3]);

        FILE* cowsay_input = popen("cowsay", "w");
        fwrite(message, sizeof(char), strlen(message), cowsay_input);
        pclose(cowsay_input);
        free(message);
    }
    else{
        printf("Wrong input, try: nadawca/data or adres email, tytul, tresc %d\n", argc);
        exit(1);
    }

}