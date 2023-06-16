#include <stdint.h>
#include <stddef.h>
#include <limine/limine.h>

#define MEMORY_SIZE 1024 * 1024
#define BLOCK_SIZE 4096
#define BITMAP_SIZE MEMORY_SIZE / BLOCK_SIZE

extern struct limine_memmap_entry *memmap_entry;

void init_pmm(struct limine_memmap_entry *memmap, uint64_t entry_count);
void *pmm_alloc_block();
void pmm_free_block(void *p);