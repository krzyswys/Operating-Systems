#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


#define FIFO_PATH "/tmp/fifo_tmp_file"
#define BUFF_SIZE 4096




int main(int argc, char** argv) {
    double dx = atof(argv[1]);
    int n = atoi(argv[2]);
    double a = 0.0;
    double b = 1.0;

    clock_t start_time = clock();
    mkfifo(FIFO_PATH, 0666);

    for (int i = 0; i < n; i++)
        if (!fork()) {
            double sub_a = a + i * (b - a) / n;
            double sub_b = sub_a + (b - a) / n;
            size_t sub_a_size = sizeof(sub_a) + 1;
            size_t sub_b_size = sizeof(sub_b)+1;
            char* arg2 = (char*) malloc(sub_a_size);
            char* arg3 = (char*) malloc(sub_b_size);
            snprintf(arg2, sub_a_size, "%lf", sub_a);
            snprintf(arg3, sub_b_size, "%lf", sub_b);
            execl("./calc", "calc", argv[1], arg2, arg3, NULL);
            free(arg2);
            free(arg3);
        }



    
    int fifo = open(FIFO_PATH, O_RDONLY);


    int buff_size = BUFF_SIZE;
    char *read_buff = malloc(buff_size + 1);

    double sum = 0.0;

    while (1) {
        int bytes_read = read(fifo, read_buff, 2048);
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        } else if (bytes_read == 0) {
            break;
        }

        char *token = strtok(read_buff, "\n");
        while (token != NULL) {
            double num = atof(token);
            sum += num;
            token = strtok(NULL, "\n");
        }
    }
    close(fifo);
    unlink(FIFO_PATH);
    free(read_buff);

    clock_t end_time = clock();
    double total_time_ms = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
    double total_time_s = ((double)(end_time - start_time)) / CLOCKS_PER_SEC ;
    printf("Time in (s): %f, time in (ms): %f\n", total_time_s, total_time_ms);
    printf("Integral value: %lf\n", sum);

    return 0;
}
