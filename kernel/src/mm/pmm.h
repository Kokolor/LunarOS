#include <stdint.h>
#include <stddef.h>

#define MEMORY_SIZE 1024 * 1024
#define BLOCK_SIZE 4096
#define BITMAP_SIZE MEMORY_SIZE / BLOCK_SIZE

void init_pmm();
void *pmm_alloc_block();
void pmm_free_block(void *p);