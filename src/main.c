#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include "header/debug.h"

void file_copy(const char *src_file_name, const char *dest_file_name)
{
    debug_print("opening file: %s\n", src_file_name);
    int src_file = open(src_file_name, O_RDONLY);

    if (src_file < 0)
    {
        printf("ERROR: can't open file: %s\n", src_file_name);
        exit(EXIT_FAILURE);
    }

    debug_print("opening file: %s\n", dest_file_name);
    int dest_file = open(dest_file_name, O_WRONLY | O_CREAT | O_TRUNC);

    if (dest_file < 0)
    {
        printf("ERROR: can't open file: %s\n", dest_file_name);
        exit(EXIT_FAILURE);
    }

    int page_size = getpagesize();
    int buff_size = 4 * page_size;
    char *buff = malloc(buff_size);

    int n = 1;
    bool eof = false;
    while (!eof)
    {
        debug_print("reading %d: %d bytes\n", n, buff_size);

        int size_readed = read(src_file, buff, buff_size);
        if (size_readed < buff_size)
            eof = true;

        debug_print("copying %d: %d bytes\n", n, size_readed);

        write(dest_file, buff, size_readed);

        ++n;
    }

    free(buff);

    struct stat src_stat;
    fstat(src_file, &src_stat);

    fchmod(dest_file, src_stat.st_mode);

    debug_print("closing file: %s\n", src_file_name);
    close(src_file);
    close(dest_file);
}

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        printf("ERROR: argument missing.\nUsage: cp SRC DEST\n");
        return EXIT_FAILURE;
    }

    const char *src = argv[1];
    const char *dest = argv[2];

    debug_print("src: %s, dest: %s\n", src, dest);

    file_copy(src, dest);

    return EXIT_SUCCESS;
}