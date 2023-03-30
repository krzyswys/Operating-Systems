#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

int main() {
    DIR *dir;
    struct dirent *curr_file;
    struct stat buf;
    long long total_size = 0;
    int check=0;
    

    dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return 1;
    }

    while ((curr_file = readdir(dir))) {
        check = stat(curr_file->d_name, &buf);
        if ( check== -1) {
            perror("stat");
            continue;
        }
        
        if (!S_ISDIR(buf.st_mode)) {
            printf("%ld %s\n", buf.st_size, curr_file->d_name);
            total_size += buf.st_size;
        }
    }

    printf("Total size: %lld\n", total_size);
    closedir(dir);

    return  0;
}
