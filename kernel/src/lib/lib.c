#include "lib.h"

// Thanks chatgpt for this file

void outb(unsigned short port, unsigned char value)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(value), "Nd"(port));
}

unsigned char inb(unsigned short port)
{
    unsigned char result;
    asm volatile("inb %1, %0"
                 : "=a"(result)
                 : "Nd"(port));
    return result;
}

void outw(unsigned short port, unsigned short value)
{
    asm volatile("outw %0, %1"
                 :
                 : "a"(value), "Nd"(port));
}

unsigned short inw(unsigned short port)
{
    unsigned short result;
    asm volatile("inw %1, %0"
                 : "=a"(result)
                 : "Nd"(port));
    return result;
}

void io_wait()
{
    asm volatile("outb %%al, $0x80"
                 :
                 : "a"(0));
}

void *memcpy(void *dest, const void *src, size_t count)
{
    char *dest_c = (char *)dest;
    const char *src_c = (const char *)src;
    for (size_t i = 0; i < count; i++)
    {
        dest_c[i] = src_c[i];
    }
    return dest;
}

void *memset(void *ptr, int value, size_t count)
{
    char *ptr_c = (char *)ptr;
    for (size_t i = 0; i < count; i++)
    {
        ptr_c[i] = (char)value;
    }
    return ptr;
}

int strcmp(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }

    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

void strncpy(char *dest, const char *src, size_t n)
{
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
}

int strncmp(const char *str1, const char *str2, size_t n)
{
    while (n && *str1 && (*str1 == *str2))
    {
        ++str1;
        ++str2;
        --n;
    }
    if (n == 0)
    {
        return 0;
    }
    else
    {
        return (*(unsigned char *)str1 - *(unsigned char *)str2);
    }
}

char *strtok(char *str, const char *delimiters)
{
    static char *next_token = NULL;
    bool found_delim = false;

    if (str != NULL)
    {
        next_token = str;
    }

    char *start = next_token;
    while (*start != '\0')
    {
        found_delim = false;

        for (size_t i = 0; delimiters[i] != '\0'; i++)
        {
            if (*start == delimiters[i])
            {
                found_delim = true;
                break;
            }
        }

        if (found_delim)
        {
            *start = '\0';
            start++;
        }
        else
        {
            break;
        }
    }

    if (*start == '\0')
    {
        return NULL;
    }

    char *end = start;
    while (*end != '\0')
    {
        bool found_next_delim = false;

        for (size_t i = 0; delimiters[i] != '\0'; i++)
        {
            if (*end == delimiters[i])
            {
                found_next_delim = true;
                break;
            }
        }

        if (found_next_delim)
        {
            break;
        }

        end++;
    }

    next_token = end;

    return start;
}

void itoa(char *buf, unsigned long int n, int base)
{
    unsigned long int tmp;
    int i, j;

    tmp = n;
    i = 0;

    do
    {
        tmp = n % base;
        buf[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (n /= base);
    buf[i--] = 0;

    for (j = 0; j < i; j++, i--)
    {
        tmp = buf[j];
        buf[j] = buf[i];
        buf[i] = tmp;
    }
}

void itoa64(uint64_t value, char *str, int base)
{
    char *ptr = str, *ptr1 = str, tmp_char;
    uint64_t tmp_value;

    do
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "fedcba9876543210123456789abcdef"[15 + (tmp_value - value * base)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0)
        *ptr++ = '-';
    *ptr-- = '\0';

    while (ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

void delay(uint16_t ms)
{
    for (long long int i = 0; i < 5000 * (uint16_t)ms / 2; i++)
        io_wait();
}