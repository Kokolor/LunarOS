#include "gdt.h"

gdt_entry_t gdt[5];
gdt_descriptor_t gdtr;

extern void load_gdt(gdt_descriptor_t*);

void set_gdt_entry(int i, uint16_t limit, uint16_t base, uint8_t access, uint8_t flags) {
    gdt[i].base0 = base & 0xFFFF;
    gdt[i].base1 = (base >> 16) & 0xFF;
    gdt[i].base2 = (base >> 24) & 0xFF;
    
    gdt[i].limit0 = limit & 0xFFFF;
    
    gdt[i].access = access;
    
    gdt[i].limit1_flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}

void init_gdt(void) {
    set_gdt_entry(0, 0, 0, 0, 0);
    set_gdt_entry(1, 0, 0, 0x9A, 0xA0);
    set_gdt_entry(2, 0, 0, 0x92, 0xA0);
    set_gdt_entry(3, 0, 0, 0xFA, 0xA0);
    set_gdt_entry(4, 0, 0, 0xF2, 0xA0);
    
    gdtr.size = sizeof(gdt) - 1;
    gdtr.offset = (uint64_t)&gdt;
    
    load_gdt(&gdtr);
}
