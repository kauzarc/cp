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
    char *buff = malloc(page_size * sizeof(char));

    int n = 1;
    bool eof = false;
    while (!eof)
    {
        debug_print("reading page %d\n", n);
        int size_readed = read(src_file, buff, page_size);
        if (size_readed < page_size)
            eof = true;

        debug_print("copying page %d\n", n);
        write(dest_file, buff, size_readed);

        ++n;
    }

    free(buff);

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