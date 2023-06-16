#include "pmm.h"

unsigned char bitmap[BITMAP_SIZE];
struct limine_memmap_entry *memmap_entry;
void *memory_start;

struct block_header
{
    size_t size;
};

void init_pmm(struct limine_memmap_entry *memmap, uint64_t entry_count)
{
    memmap_entry = memmap;
    memset(bitmap, 0, BITMAP_SIZE);

    memory_start = NULL;
    for (uint64_t i = 0; i < entry_count; i++)
    {
        if (memmap_entry[i].type == LIMINE_MEMMAP_USABLE)
        {
            memory_start = (void *)memmap_entry[i].base;
            break;
        }
    }

    if (memory_start == NULL)
    {
        printk_error("No usable memory.");
    }
}

void *pmm_alloc_block()
{
    for (int i = 0; i < BITMAP_SIZE; i++)
    {
        if (bitmap[i] == 0)
        {
            bitmap[i] = 1;
            return (void *)((uintptr_t)memory_start + (i * BLOCK_SIZE));
        }
    }

    return NULL;
}

void pmm_free_block(void *p)
{
    int index = ((uintptr_t)p - (uintptr_t)memory_start) / BLOCK_SIZE;
    if (index >= 0 && index < BITMAP_SIZE)
    {
        bitmap[index] = 0;
    }
    else
    {
        printk_error("pmm_free_block: Invalid pointer %p, index out of bounds\n", p);
    }
}

void *malloc(size_t size)
{
    void *block = pmm_alloc_block();
    if (block == NULL)
    {
        return NULL;
    }

    struct block_header *header = (struct block_header *)block;
    header->size = size;
    void *data = (void *)((uintptr_t)block + sizeof(struct block_header));

    return data;
}

void free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    struct block_header *header = (struct block_header *)((uintptr_t)ptr - sizeof(struct block_header));

    pmm_free_block(header);
}
