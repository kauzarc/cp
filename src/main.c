#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include "header/debug.h"

bool file_exists(const char *file_name)
{
    struct stat buffer;
    return (stat(file_name, &buffer) == 0);
}

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
    debug_print("closing file: %s\n", dest_file_name);
    close(dest_file);
}

void folder_copy(const char *src_folder_name, const char *dest_folder_name)
{
    debug_print("opening folder: %s\n", src_folder_name);
    DIR *src_folder = opendir(src_folder_name);

    if (!file_exists(dest_folder_name))
    {
        struct stat src_folder_stat;
        stat(src_folder_name, &src_folder_stat);

        debug_print("creating folder: %s\n", dest_folder_name);
        if (mkdir(dest_folder_name, src_folder_stat.st_mode) != 0)
        {
            printf("ERROR: can't create folder: %s\n", dest_folder_name);
            exit(EXIT_FAILURE);
        }
    }

    for (struct dirent *src_entry = readdir(src_folder); src_entry != NULL; src_entry = readdir(src_folder))
    {
        if (strcmp(src_entry->d_name, ".") != 0 && strcmp(src_entry->d_name, "..") != 0)
        {
            int subsrc_name_size = strlen(src_folder_name) + 1 + strlen(src_entry->d_name);
            char *subsrc_name = malloc((subsrc_name_size + 1) * sizeof(char));

            strcpy(subsrc_name, src_folder_name);
            strcat(subsrc_name, "/");
            strcat(subsrc_name, src_entry->d_name);

            int subdest_name_size = strlen(dest_folder_name) + 1 + strlen(src_entry->d_name);
            char *subdest_name = malloc((subdest_name_size + 1) * sizeof(char));

            strcpy(subdest_name, dest_folder_name);
            strcat(subdest_name, "/");
            strcat(subdest_name, src_entry->d_name);

            struct stat src_entry_stat;
            stat(src_entry->d_name, &src_entry_stat);

            if (S_ISDIR(src_entry_stat.st_mode))
            {
                folder_copy(subsrc_name, subdest_name);
            }
            else if (S_ISREG(src_entry_stat.st_mode))
            {
                file_copy(subsrc_name, subdest_name);
            }

            free(subsrc_name);
            free(subdest_name);
        }
    }

    debug_print("closing folder: %s\n", src_folder_name);
    closedir(src_folder);
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

    folder_copy(src, dest);

    return EXIT_SUCCESS;
}