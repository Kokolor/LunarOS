#include "pmm.h"

uint64_t *bitmap;
size_t bitmap_size;
void *memory_start;
size_t total_memory;

void init_pmm(struct limine_memmap_entry *memmap, uint64_t entry_count)
{
    for (uint64_t i = 0; i < entry_count; i++)
    {
        if (memmap[i].type == LIMINE_MEMMAP_USABLE)
        {
            memory_start = (void *)memmap[i].base;
            total_memory = memmap[i].length;

            bitmap_size = total_memory / (BLOCK_SIZE * BITS_PER_BYTE);

            if (bitmap_size > total_memory)
            {
                printk_error("init_pmm: Not enough memory for the bitmap.\n");
                return;
            }

            bitmap = (uint64_t *)memory_start;
            memory_start = (void *)((size_t)memory_start + bitmap_size);
            total_memory -= bitmap_size;
            memset(bitmap, 0, bitmap_size);
            
            break;
        }
    }
}