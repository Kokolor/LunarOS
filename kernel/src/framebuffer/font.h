#include <stdint.h>
#include <lib/lib.h>
#include <stdarg.h>
#include "framebuffer.h"

typedef struct
{
    uint8_t magic[2];
    uint8_t mode;
    uint8_t height;
} font_header;

void init_terminal(terminal *term, int cursor_x, int cursor_y, unsigned int color);
void put_char(char c, int x, int y, terminal *term);
void put_string(char *string);