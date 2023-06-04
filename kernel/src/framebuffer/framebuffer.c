#include "framebuffer.h"

struct limine_framebuffer *buffer;

void init_framebuffer(struct limine_framebuffer *framebuffer)
{
    buffer = framebuffer;
}

void put_pixel(int x, int y, uint32_t color)
{
    uint32_t *framebuffer_pointer = buffer->address;
    framebuffer_pointer[y * buffer->width + x] = color;
}

void put_rectangle(int x, int y, int width, int height, uint32_t color)
{
    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            put_pixel(j, i, color);
        }
    }
}

void clear_screen(uint32_t color)
{
    term.cursor_x = 0;
    term.cursor_y = 0;
    uint32_t *framebuffer_pointer = buffer->address;
    uint32_t framebuffer_size = buffer->width * buffer->height;

    for (uint32_t i = 0; i < framebuffer_size; i++)
    {
        framebuffer_pointer[i] = color;
    }
}
