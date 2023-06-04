#include <stdint.h>

#define GDT_FLAG_DATASEG 0x02
#define GDT_FLAG_CODESEG 0x0a
#define GDT_FLAG_TSS 0x09

#define GDT_FLAG_SEGMENT 0x10
#define GDT_FLAG_RING0 0x00
#define GDT_FLAG_RING3 0x60
#define GDT_FLAG_PRESENT 0x80

#define GDT_FLAG_64_BIT 0x20
#define GDT_FLAG_4K_GRAN 0x800

typedef struct
{
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) gdt_descriptor_t;

typedef struct
{
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t access;
    uint8_t limit1_flags;
    uint8_t base2;
} __attribute__((packed)) gdt_entry_t;

void gdt_set_entry(int i, uint16_t limit, uint16_t base, uint8_t access, uint8_t flags);
void init_gdt(void);