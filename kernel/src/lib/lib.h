#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
void outw(unsigned short port, unsigned short value);
unsigned short inw(unsigned short port);
void io_wait();
void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *ptr, int value, size_t count);
void strncpy(char *dest, const char *src, size_t n);
int strncmp(const char *str1, const char *str2, size_t n);
char *strtok(char *str, const char *delimiters);
void itoa(char *buf, unsigned long int n, int base);
void itoa64(uint64_t value, char *str, int base);
void delay(uint16_t ms);