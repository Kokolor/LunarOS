#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./app <argument>\n");
        return 1;
    }

    if (strcmp(argv[1], "create") == 0)
    {
        system("dd if=/dev/zero of=fat32.img bs=1M count=64\nmkfs.fat -F32 fat32.img");
    }
    else if (strcmp(argv[1], "bing") == 0)
    {
        printf("chillin\n");
    }
    else if (strcmp(argv[1], "copy") == 0)
    {
        char command[100];
        snprintf(command, sizeof(command), "mcopy -i fat32.img ./%s ::/", argv[2]);
        system(command);
    }
    else
    {
        printf("Unknown argument\n");
    }

    return 0;
}
