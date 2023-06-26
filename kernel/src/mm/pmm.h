#include <stdint.h>
#include <stddef.h>
#include <limine/limine.h>
#include <arch/x86_64/idt.h>

#define BLOCK_SIZE 4096
#define BITS_PER_BYTE 8

extern uint64_t* bitmap;
extern size_t bitmap_size;
extern void *memory_start;
extern size_t total_memory;