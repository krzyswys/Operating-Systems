#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int count_lines(char *filename){
    FILE *file = fopen(filename, "r");
    int num_lines = 0;
    char ch;
    if (file == NULL) {
        num_lines = 0;
    } else {
        while ((ch = fgetc(file)) != EOF) {
            if (ch == '\n') {
                num_lines++;
            }
        }
        fclose(file);
    }
    return num_lines;
}

int main(int noOfarg, char *argv[]) {

    if (noOfarg < 5) {
        fprintf(stderr, "To few arguments try: %s char1 char2 input_file output_file\n", argv[0]);
        return 1;
    }
    else if(noOfarg>5){
        fprintf(stderr, "To many arguments try: %s char1 char2 input_file output_file\n", argv[0]);
        return 1;
    }

    char *program_name= argv[0];
    char char1 = argv[1][0];
    char char2 = argv[2][0];
    char *input_file_name = argv[3];
    char *output_file_name = argv[4];

    int input_file = open(input_file_name, O_RDONLY);
    if (input_file < 0) {
        perror(input_file_name);
        return 1;
    }

    int output_file = open(output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_file < 0) {
        perror(output_file_name);
        close(input_file);
        return 1;
    }

    clock_t start_time = clock();
    char buffer[1024];
    size_t count;
    while ((count = read(input_file, buffer, sizeof(buffer))) > 0) {
        for (size_t i = 0; i < count; i++) {
            if (buffer[i] == char1) {
                buffer[i] = char2;
            }
        }
        write(output_file, buffer, count);
    }

    close(input_file);
    close(output_file);
    clock_t end_time = clock();
    printf("Writing to file %s succeded\n", output_file_name);
    int attempt=count_lines("pomiar_zad_1.txt");
    double elapsed_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    char message[1024];
    sprintf(message,"%d. run of %s, changing '%c' to '%c' from file '%s' to file '%s' in read() family, finished in %.6lfs\n", attempt+1, program_name, char1, char2,input_file_name, output_file_name, elapsed_time);

    fprintf(stdout, "Elapsed time: %.6lf seconds\n", elapsed_time);

  
    int results_file = open("pomiar_zad_1.txt", O_WRONLY | O_CREAT | O_APPEND);
    if (results_file<0) {
        perror("pomiar_zad_1.txt");
        return 1;
    }
    write(results_file, message, strlen(message));
    close(results_file);




    return 0;
}
