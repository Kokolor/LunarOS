#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "font.h"
#include <limine/limine.h>

extern struct limine_framebuffer *buffer;
extern int cursor_x, cursor_y;

void init_framebuffer(struct limine_framebuffer *framebuffer);
void put_pixel(int x, int y, uint32_t color);
uint32_t get_pixel(int x, int y);
void put_rectangle(int x, int y, int width, int height, uint32_t color);
void clear_screen(uint32_t color);
void set_color(uint32_t color);
void put_string(char *string);
void printk(const char *format, ...);