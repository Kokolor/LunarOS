#include "framebuffer.h"

struct limine_framebuffer *buffer;

uint32_t term_color = 0xffffff;
int cursor_x, cursor_y;

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

void clear_screen(uint32_t color)
{
    cursor_x = 0;
    cursor_y = 0;

    uint32_t *framebuffer_pointer = buffer->address;
    uint32_t framebuffer_size = buffer->width * buffer->height;

    for (uint32_t i = 0; i < framebuffer_size; i++)
    {
        framebuffer_pointer[i] = color;
    }
}

void put_char(char c, int x, int y)
{
    uint8_t *offset = font + sizeof(font_header) + 16 * c;

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (offset[i] & (1 << j))
            {
                put_pixel(x + 8 - j, y + i, term_color);
            }
        }
    }
}

void set_color(uint32_t color)
{
    term_color = color;
}

void put_string(char *string)
{
    debug_print(string);
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (string[i] == '\n')
        {
            cursor_x = 0;
            cursor_y += 16;
        }
        else
        {
            put_char(string[i], cursor_x, cursor_y);
            cursor_x += 8;
            if (cursor_x >= buffer->width)
            {
                cursor_x = 0;
                cursor_y += 16;
            }
        }
    }
}

void printk(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 's':
            {
                char *str = va_arg(args, char *);
                put_string(str);
                break;
            }
            case 'd':
            {
                int num = va_arg(args, int);
                char str[12];
                itoa(str, num, 10);
                put_string(str);
                break;
            }
            case 'l':
            {
                uint64_t num = va_arg(args, uint64_t);
                char str[21];
                itoa64(num, str, 10);
                put_string(str);
                break;
            }
            case 'p':
            {
                void *ptr = va_arg(args, void *);
                char str[19];
                itoa64((uint64_t)ptr, str, 16);
                put_string(str);
                break;
            }
            case 'x':
            {
                int num = va_arg(args, int);
                char str[10];
                itoa(str, num, 16);
                put_string(str);
                break;
            }
            }
        }
        else
        {
            char str[2] = {*format, '\0'};
            put_string(str);
        }
        format++;
    }
    va_end(args);
}