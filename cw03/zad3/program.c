#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#define PATH_MAX 256


int traverse_dir(char *path, char *string) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char curr_path[PATH_MAX+2]; 
    pid_t pid;
    int status;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(curr_path, PATH_MAX+2, "%s/%s", path, entry->d_name); // ładuje do buffera sciezke pliku

        if (stat(curr_path, &st) == -1) { //ładuje staty do buffera
            perror("lstat");
            return 1;
        }

        if (S_ISDIR(st.st_mode)) { //sprawdza czy jest folderem, jak tak to robi forka
            pid = fork();

            if (pid == -1) {
                perror("fork");
                return 1;
            } else if (pid == 0) {
                if(traverse_dir(curr_path, string)==1){
                   fprintf(stderr, "Error in calling function");
                }
                return 0;
            }
        } else if (S_ISREG(st.st_mode)) { 
            FILE *file = fopen(curr_path, "r");

            if (file == NULL) {
                perror("fopen");
                return 1;
            }
            char extracted_str[sizeof(string)+2];
            if (fgets(extracted_str, sizeof(string)+2, file) != NULL) {// uzywa sie holdera na znaki do wpierdolenia tam pierwcyh buff_size rzeczy z pliku jesli sie da
                if (strncmp(extracted_str, string, strlen(string)) == 0) { //porownuje stringa (o dlugosci naszego stringa) z pliku do stringa szukanego
                    printf("%s ,PID:,%d\n", path, getpid());
                }
            }

            if (fclose(file) == -1) { 
                perror("fclose");
                return 1;
            }
        }
    }

    if (closedir(dir) == -1) {
        perror("closedir");
        return 1;
    }

    while (wait(&status) != -1) {}
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <directory> <string>\n", argv[0]);
        return 0;


    }

    traverse_dir(argv[1], argv[2]);

    return 0;
}
