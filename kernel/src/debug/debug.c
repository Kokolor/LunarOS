#include "debug.h"

void debug_print(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        outb(0xE9, str[i]);
    }
}
