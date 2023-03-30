#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

#ifdef DYNAMIC
    #include "lib_dynamic.h"
#else
    #include "lib.h"
#endif

#define SUCCESS  0
#define FAILURE  1

#define MAX_COMMAND_LENGTH 50
#define MAX_FILENAME_LENGTH 50

static clock_t ct_start;
static clock_t ct_stop;
static struct rusage ru_start;
static struct rusage ru_stop;

void timing_start(void){
    ct_start = clock();
    getrusage(RUSAGE_SELF, &(ru_start));
}

void timing_stop(void){
    ct_stop = clock();
    getrusage(RUSAGE_SELF, &(ru_stop));
}

void print_time(void){
    double stdtime;
    double cputime_u;
    double cputime_s;

    stdtime = (double) (ct_stop - ct_start) / CLOCKS_PER_SEC;

    cputime_u = (ru_stop.ru_utime.tv_usec - ru_start.ru_utime.tv_usec) / 1.e6;
    cputime_u += (ru_stop.ru_utime.tv_sec - ru_start.ru_utime.tv_sec);

    cputime_s = (ru_stop.ru_stime.tv_usec - ru_start.ru_stime.tv_usec) / 1.e6;
    cputime_s += (ru_stop.ru_stime.tv_sec - ru_start.ru_stime.tv_sec);

    printf(
        "Real time: %.6lf s, User time: %.6lf s, System time: %.6lf s\n",
        stdtime,
        cputime_u,
        cputime_s);
}


int main(void){
    char command[MAX_COMMAND_LENGTH];
    int ret;
    BlockArray *array = NULL;

    #ifdef DYNAMIC
        ret = init_dynamic_handler();

        if (SUCCESS != ret)
        {
            return FAILURE;
        }
    #endif

    printf("Write 'exit' or '[Ctr+C]' to exit program\n");

    while (1)
    {
        printf("> ");

        char *fgets_result = fgets(command, MAX_COMMAND_LENGTH, stdin);

        if (fgets_result != command)
        {
            fprintf(stderr, "Error: could not read text from standard input\n");
            return FAILURE;
        }

        if (strncmp(command, "init ", 5) == 0)
        {
            int size = (-1);
            sscanf(command + 5, "%d", &size);
            printf("Initializing block array with size: %d\n", size);


            if (array != NULL)
            {
                free_blocks_array(array);
            }
            timing_start();

            array = block_array_init(size);

            timing_stop();

            if (array == NULL)
            {
                fprintf(stderr, "Error: failed to initialize array\n");
            }
            else
            {
                printf("Initialized block array with size: %d\n", size);
                print_time();
            }
        }
        else if (strncmp(command, "count ", 6) == 0)
        {
            char filename[MAX_FILENAME_LENGTH];
            sscanf(command + 6, "%s", filename);

            printf("Counting lines and words in file: %s\n", filename);

            timing_start();

            ret = count_lines_words(array, filename);

            timing_stop();

            if (SUCCESS != ret)
            {
                fprintf(stderr, "Error: failed to count lines and words\n");
            }
            else
            {
                printf("Counted lines and words in file: %s\n", filename);
                print_time();
            }
        }
        else if (strncmp(command, "show ", 5) == 0)
        {
            int index = (-1);
            sscanf(command + 5, "%d", &index);

            char *contents = NULL;

            timing_start();

            contents = get_block_contents(array, index);

            timing_stop();

            if (contents == NULL)
            {
                fprintf(stderr, "Error: failed to count lines and words\n");
            }
            else
            {
                printf("Block contents at index %d:\n", index);
                printf("%s", contents);

                print_time();
            }
        }
        else if (strncmp(command, "delete ", 7) == 0)
        {
            int index = (-1);
            sscanf(command + 7, "%d", &index);

            printf("Deleting block contents at index %d:\n", index);

            timing_start();

            ret = delete_block(array, index);

            timing_stop();

            if (SUCCESS != ret)
            {
                fprintf(stderr, "Error: failed to delete contents at index %d:\n", index);
            }
            else
            {
                printf("Deleted block contents at index %d:\n", index);
                print_time();
            }
        }
        else if (strncmp(command, "destroy", 6) == 0)
        {
            printf("Deleting array\n");

            timing_start();

            ret = free_blocks_array(array);
            array = NULL;

            timing_stop();

            if (SUCCESS != ret)
            {
                fprintf(stderr, "Error: failed to delete array\n");
            }
            else
            {
                printf("Deleted arrray\n");
                print_time();
            }
        }
        else if (strncmp(command, "exit", 4) == 0)
        {
            break;
        }
        else
        {
            printf("Invalid command, try again\n");
        }

        puts("");
    }

    if (array != NULL)
    {
        free_blocks_array(array);
    }

    #ifdef DYNAMIC
        free_handler_memory();
    #endif

    return SUCCESS;
}
