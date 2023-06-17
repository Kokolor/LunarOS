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

uint32_t get_pixel(int x, int y)
{
    uint32_t *framebuffer_pointer = buffer->address;
    return framebuffer_pointer[y * buffer->width + x];
}

void put_rectangle(int x, int y, int width, int height, uint32_t color)
{
    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
        {
            put_pixel(j, i, color);
        }
    }
}

uint32_t interpolate_color(uint32_t start, uint32_t end, int progress, int max_progress)
{
    int r_start = (start >> 16) & 0xff;
    int g_start = (start >> 8) & 0xff;
    int b_start = start & 0xff;

    int r_end = (end >> 16) & 0xff;
    int g_end = (end >> 8) & 0xff;
    int b_end = end & 0xff;

    int r = r_start + ((r_end - r_start) * progress) / max_progress;
    int g = g_start + ((g_end - g_start) * progress) / max_progress;
    int b = b_start + ((b_end - b_start) * progress) / max_progress;

    return (r << 16) | (g << 8) | b;
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
