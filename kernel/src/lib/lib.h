#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define LONG_MIN (-LONG_MAX - 1L)
#define LONG_MAX 2147483647L
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
void outw(unsigned short port, unsigned short value);
unsigned short inw(unsigned short port);
void io_wait();
void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *ptr, int value, size_t count);
void *memmove(void *dest, const void *src, size_t n);
void strncpy(char *dest, const char *src, size_t n);
int strncmp(const char *str1, const char *str2, size_t n);
char *strchr(const char *str, int character);
size_t strlen(const char *str);
char *strdup(const char *s);
char *strtok(char *str, const char *delimiters);
char *strrchr(const char *str, int character);
long int strtol(const char *str, char **endptr, int base);
int atoi(const char *str);
void itoa(char *buf, unsigned long int n, int base);
void itoa64(uint64_t value, char *str, int base);
void delay(uint16_t ms);