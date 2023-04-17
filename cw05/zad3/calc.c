#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define FIFO_PATH "/tmp/fifo_tmp_file"
ssize_t bytes_written;

double f(double x) {
    return 4/(x*x+1);
} 

double integral(double a, double b, double dx) {
    double acc = 0.0;
    for (double x = a; x < b; x += dx)
        acc += f(x)*dx;
    return acc;
}

int main(int argc, char** argv) {
    double dx = atof(argv[1]);
    double a = atof(argv[2]);
    double b = atof(argv[3]);

    double calculated = integral(a, b, dx);

    size_t buff_size = 4096;
    char* buff = (char*) malloc(buff_size);


    if (buff == NULL) {
        fprintf(stderr, "Couldn't alllocate memory.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(buff, buff_size, "%f\n", calculated);


    int fifo = open(FIFO_PATH, O_WRONLY);
    bytes_written = write(fifo, buff, strlen(buff));
    
    close(fifo);
    free(buff);
    return 0;
}