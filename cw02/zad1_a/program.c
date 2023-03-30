#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

    FILE *input_file = fopen(input_file_name, "rb");
    if (!input_file) {
        perror(input_file_name);
        return 1;
    }

    FILE *output_file = fopen(output_file_name, "wb"); //TODO: zapytać czemu nie działa [w+]b?
    if (!output_file) {
        perror(output_file_name);
        fclose(input_file);
        return 1;
    }

    clock_t start_time = clock();
    char buffer[1024];
    size_t count;
    while ((count = fread(buffer, sizeof(char), sizeof(buffer), input_file)) > 0) {
        for (size_t i = 0; i < count; i++) {
            if (buffer[i] == char1) {
                buffer[i] = char2;
            }
        }
        fwrite(buffer, sizeof(char), count, output_file);
    }
    // fseek(input_file, 0, SEEK_SET);
    // fseek(output_file, 0, SEEK_SET);
    fclose(input_file);
    fclose(output_file);
    clock_t end_time = clock();

    printf("Writing to file %s succeded\n", output_file_name);

    int attempt=count_lines("pomiar_zad_1.txt");
    double elapsed_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    char message[1024];
    sprintf(message,"%d. run of %s, changing '%c' to '%c' from file '%s' to file '%s' in fread() family, finished in %.6lfs", attempt+1, program_name, char1, char2,input_file_name, output_file_name,  elapsed_time);
    fprintf(stdout, "Elapsed time: %.6lf seconds\n", elapsed_time);

  
    FILE *results_file = fopen("pomiar_zad_1.txt", "a");
    if (!results_file) {
        perror("pomiar_zad_1.txt");
        return 1;
    }
    fprintf(results_file, "%s\n", message);
    fclose(results_file);


    return 0;
}