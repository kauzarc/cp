#include "header/copy.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        printf("ERROR: argument missing.\nUsage: cp SRC DEST\n");
        return EXIT_FAILURE;
    }

    copy(argv[1], argv[2]);

    return EXIT_SUCCESS;
}