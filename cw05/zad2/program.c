#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

double f(double x) {
    return 4.0 / (x * x + 1);
}

double calc_integral(double a, double b, double dx) {
    double integral = 0.0;
    double x;
    for (x = a; x < b; x += dx) {
        integral += f(x) * dx;
    }
    return integral;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Wrong input, try: %s dx n\n", argv[0]);
        return 1;
    }

    double dx = atof(argv[1]);
    int n = atoi(argv[2]);
    double a = 0.0;
    double b = 1.0;

    int i, pid;
    double sub_integral, total_integral = 0.0;

    int fd[n][2];

    clock_t start_time = clock();
    for (i = 0; i < n; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            return 1;
        }

        pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        }

        if (pid == 0) { 
            double sub_a = a + i * (b - a) / n;
            double sub_b = sub_a + (b - a) / n;
            sub_integral = calc_integral(sub_a, sub_b, dx);
            write(fd[i][1], &sub_integral, sizeof(sub_integral));
            close(fd[i][1]);  // close write end
            exit(0);
        }
    }

    for (i = 0; i < n; i++) {
        read(fd[i][0], &sub_integral, sizeof(sub_integral));
        close(fd[i][1]);  // close read end
        total_integral += sub_integral;
    }

    while (wait(NULL) > 0);

    clock_t end_time = clock();
    double total_time_ms = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
    double total_time_s = ((double)(end_time - start_time)) / CLOCKS_PER_SEC ;
    printf("Time in (s): %f, time in (ms): %f\n", total_time_s, total_time_ms);
    printf("Integral value: %lf\n", total_integral);

    return 0;
}
