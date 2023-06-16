#include <stdint.h>
#include <stddef.h>

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
void outw(unsigned short port, unsigned short value);
unsigned short inw(unsigned short port);
void io_wait();
void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *ptr, int value, size_t count);
void itoa(char *buf, unsigned long int n, int base);
void itoa64(uint64_t value, char* str, int base);
void delay(uint16_t ms);