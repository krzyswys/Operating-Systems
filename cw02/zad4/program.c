#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ftw.h>
#include <libgen.h>

static long long total_size = 0;

int print_file_info(const char* fpath, const struct stat* sb, int typeflag) {
    if (typeflag==FTW_F){    // if (!S_ISDIR(sb->st_mode)) { //TODO: zapytać jaka jest różnica międyz tymi dwoma
        const char* file_name = fpath;
        const char* p = fpath;
        while (*p) {
            if (*p == '/') {
                file_name = p + 1;
            }
            p++;
        }
        printf("%ld\t%s\n", sb->st_size, file_name);
        total_size += (long long)sb->st_size;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Too few arguments, try: %s <dir_name>\n", argv[0]);
        return 1;
    }
    else if (argc > 2) {
        fprintf(stderr, "Too many arguments, try: %s <dir_name>\n", argv[0]);
        return 1;
    }

    if (ftw(argv[1], print_file_info, 2) == -1) {
        perror("ftw");
        return 1;
    }

    printf("Total size: %lld bytes\n", total_size);

    return 0;
}
