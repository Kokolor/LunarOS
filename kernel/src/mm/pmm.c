#include "pmm.h"

unsigned char bitmap[BITMAP_SIZE];
void *memory_start = (void *)0x100000;

void init_pmm()
{
    memset(bitmap, 0, BITMAP_SIZE);
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
    int index = ((int)p) / BLOCK_SIZE;
    bitmap[index] = 0;
}
