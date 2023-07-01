#include <stdint.h>
#include <stddef.h>
#include <limine/limine.h>

#define PAGE_SIZE 4096
#define MAX_MEMORY (128ULL * 1024 * 1024 * 1024)
#define PAGE_COUNT (MAX_MEMORY / PAGE_SIZE)
#define PAGE_MASK 0xFFFFFFFFFFFFF000

typedef struct mem_block
{
    struct mem_block *next;
    size_t size;
    int is_free;
} mem_block;

void init_pfa(struct limine_memmap_entry **entries, uint64_t entry_count);
uint64_t alloc_page();
void free_page(uint64_t page_addr);
void *malloc(size_t size);
void free(void *ptr);
uint64_t get_total_memory(struct limine_memmap_entry **entries, uint64_t entry_count);
void print_memory_info(struct limine_memmap_entry **entries, uint64_t entry_count);