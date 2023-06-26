#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define LONG_MIN (-LONG_MAX - 1L)
#define LONG_MAX 2147483647L
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
void insl(uint16_t port, void *addr, int cnt);
void outw(unsigned short port, unsigned short value);
unsigned short inw(unsigned short port);
void io_wait();
void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *ptr, int value, size_t count);
void *memmove(void *dest, const void *src, size_t n);
size_t strlen(const char *str);
void itoa(char *buf, unsigned long int n, int base);
void itoa64(uint64_t value, char *str, int base);
void delay(uint16_t ms);