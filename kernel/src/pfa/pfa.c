#include "pfa.h"

unsigned char page_bitmap[PAGE_COUNT / 8];
static uint8_t heap[PAGE_SIZE];
static mem_block *free_blocks = NULL;

void set_page_used(uint64_t page_idx)
{
    page_bitmap[page_idx / 8] &= ~(1 << (page_idx % 8));
}

void set_page_free(uint64_t page_idx)
{
    page_bitmap[page_idx / 8] |= 1 << (page_idx % 8);
}

void init_pfa(struct limine_memmap_entry **entries, uint64_t entry_count)
{
    for (uint64_t i = 0; i < PAGE_COUNT; i++)
    {
        set_page_used(i);
    }

    for (uint64_t i = 0; i < entry_count; i++)
    {
        struct limine_memmap_entry *entry = entries[i];

        if (entry->type != LIMINE_MEMMAP_USABLE)
        {
            continue;
        }

        uint64_t first_page = entry->base / PAGE_SIZE;
        uint64_t last_page = (entry->base + entry->length - 1) / PAGE_SIZE;

        for (uint64_t page = first_page; page <= last_page; page++)
        {
            set_page_free(page);
        }
    }
}

uint64_t alloc_page()
{
    for (uint64_t i = 0; i < PAGE_COUNT; i++)
    {
        if (page_bitmap[i / 8] & (1 << (i % 8)))
        {
            set_page_used(i);
            return i * PAGE_SIZE;
        }
    }

    return (uint64_t)-1;
}

void free_page(uint64_t page_addr)
{
    uint64_t page_idx = page_addr / PAGE_SIZE;
    set_page_free(page_idx);
}

void *malloc(size_t size)
{
    size_t real_size = size + sizeof(mem_block);
    mem_block *block = free_blocks;
    mem_block *prev_block = NULL;

    while (block != NULL)
    {
        if (block->is_free && block->size >= real_size)
        {
            if (block->size > real_size + sizeof(mem_block))
            {
                mem_block *split_block = (mem_block *)((char *)block + real_size);
                split_block->next = block->next;
                split_block->size = block->size - real_size;
                split_block->is_free = 1;

                block->next = split_block;
                block->size = real_size;
            }

            block->is_free = 0;
            free_blocks = block->next;

            return (void *)(block + 1);
        }

        prev_block = block;
        block = block->next;
    }

    size_t num_pages = (real_size + sizeof(mem_block) + PAGE_SIZE - 1) / PAGE_SIZE;
    size_t block_size = num_pages * PAGE_SIZE;
    block = (mem_block *)alloc_page(block_size);

    if (block == NULL)
    {
        return NULL;
    }

    block->size = block_size;
    block->is_free = 0;
    block->next = NULL;

    if (prev_block != NULL)
    {
        prev_block->next = block;
    }
    else
    {
        free_blocks = block;
    }

    return (void *)(block + 1);
}

void free(void *ptr)
{
    mem_block *header = (mem_block *)(ptr - sizeof(mem_block));
    header->is_free = 1;
}

uint64_t get_total_memory(struct limine_memmap_entry **entries, uint64_t entry_count)
{
    uint64_t total_memory = 0;

    for (uint64_t i = 0; i < entry_count; i++)
    {
        struct limine_memmap_entry *entry = entries[i];

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            total_memory += entry->length;
        }
    }

    return total_memory;
}

void print_memory_info(struct limine_memmap_entry **entries, uint64_t entry_count)
{
    uint64_t total_memory = get_total_memory(entries, entry_count);

    total_memory /= 1024;
    total_memory /= 1024;
    total_memory /= 1024;
    total_memory += 1;

    printk("Total memory: %l GB\n", total_memory);
}