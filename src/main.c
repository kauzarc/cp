#include <stdio.h>
#include "header/debug.h"

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        printf("ERROR: argument missing.\nUsage: cp SOURCE DEST\n");
        return 1;
    }

    const char *source = argv[1];
    const char *dest = argv[2];

    debug_print("source: %s, dest: %s\n", source, dest);

    return 0;
}