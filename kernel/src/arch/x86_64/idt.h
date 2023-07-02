#include <stdint.h>
#include <stdbool.h>
#include "pic.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define panic(message, frame) _panic(__FILE__ ":" TOSTRING(__LINE__) ": " message, frame)

extern int setup_step;

#define KEY_1 0x02
#define KEY_2 0x03

extern struct interrupt_frame;

typedef struct
{
    uint16_t base_low;
    uint16_t cs;
    uint8_t ist;
    uint8_t attributes;
    uint16_t base_mid;
    uint32_t base_high;
    uint32_t rsv0;
} __attribute__((packed)) idt_entry;

typedef struct
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idt_pointer;

void init_idt();