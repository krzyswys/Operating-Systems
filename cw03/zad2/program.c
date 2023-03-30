#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error, try: %s absolute_directory_path\n", argv[0]);
        return 1;
    }

    setbuf(stdout, NULL);
    printf("%s ", argv[0]);
    if (!execl("/bin/ls", "ls", argv[1], NULL)){
        perror("execl");
        return 1;

    }
    return 0;
}
