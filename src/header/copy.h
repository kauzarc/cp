#ifndef COPY_H
#define COPY_H

#include <stdbool.h>

bool file_exists(const char *file_name);

void copy(const char *src, const char *dest);

void file_copy(const char *src_file_name, const char *dest_file_name);

void folder_copy(const char *src_folder_name, const char *dest_folder_name);

#endif