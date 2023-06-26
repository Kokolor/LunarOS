#include <stdint.h>


typedef struct
{
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) gdt_pointer;

typedef struct
{
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t access;
    uint8_t limit1_flags;
    uint8_t base2;
} __attribute__((packed)) gdt_entry;

void gdt_set_entry(int i, uint16_t limit, uint16_t base, uint8_t access, uint8_t flags);
void init_gdt(void);