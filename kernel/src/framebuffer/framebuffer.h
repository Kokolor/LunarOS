#include <stdint.h>
#include <stddef.h>
#include <limine/limine.h>

extern struct limine_framebuffer *buffer;

void init_framebuffer(struct limine_framebuffer* framebuffer);
void put_pixel(int x, int y, uint32_t col);
void put_rectangle(int x, int y, int width, int height, uint32_t color);
void clear_screen(uint32_t color);

typedef struct
{
    uint8_t *font;
    int cursor_x;
    int cursor_y;
    unsigned int color;
} terminal;

extern terminal term;