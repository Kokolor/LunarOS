// #include <framebuffer/font.h>
#include "idt.h"

int setup_step = 0;

struct interrupt_frame
{
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
    uint64_t error_code;
};

__attribute__((aligned(0x10))) static idt_entry idt[256];
static idt_pointer idtr;

void _panic(char *message, struct interrupt_frame *frame)
{
    clear_screen(0x550000);

    printk("####### ######  ######  ####### ######  \n");
    printk("#       #     # #     # #     # #     # \n");
    printk("#       #     # #     # #     # #     # \n");
    printk("#####   ######  ######  #     # ######  \n");
    printk("#       #   #   #   #   #     # #   #   \n");
    printk("#       #    #  #    #  #     # #    #  \n");
    printk("####### #     # #     # ####### #     # \n");
    printk("\n");
    printk("LunarOS has been stopped due to a system error.\n");
    printk("\n");

    char buffer[64];
    set_color(0x00FF00);
    printk("Panic: ");
    printk(message);
    printk(" - Address 0x");
    itoa64(frame->rip, buffer, 16);
    printk(buffer);
    printk("\n");

    set_color(0xFF0000);
    printk("System halted.\n");

    while (1)
    {
        __asm__("hlt");
    }
}

__attribute__((interrupt)) void zero_division_handler(struct interrupt_frame *frame)
{
    panic("DIVISION BY ZERO", frame);
}

__attribute__((interrupt)) void debug_handler(struct interrupt_frame *frame)
{
    panic("DEBUG", frame);
}

__attribute__((interrupt)) void gpf_handler(struct interrupt_frame *frame)
{
    panic("GENERAL PROTECTION FAULT", frame);
}

__attribute__((interrupt)) void pf_handler(struct interrupt_frame *frame)
{
    panic("PAGE FAULT", frame);
}

__attribute__((interrupt)) void non_maskable_interrupt_handler(struct interrupt_frame *frame)
{
    panic("NON-MASKABLE INTERRUPT", frame);
}

__attribute__((interrupt)) void breakpoint_handler(struct interrupt_frame *frame)
{
    panic("BREAKPOINT", frame);
}

__attribute__((interrupt)) void overflow_handler(struct interrupt_frame *frame)
{
    panic("OVERFLOW", frame);
}

__attribute__((interrupt)) void bound_range_exceeded_handler(struct interrupt_frame *frame)
{
    panic("BOUND RANGE EXCEEDED", frame);
}

__attribute__((interrupt)) void invalid_opcode_handler(struct interrupt_frame *frame)
{
    panic("INVALID OPCODE", frame);
}

__attribute__((interrupt)) void device_not_available_handler(struct interrupt_frame *frame)
{
    panic("DEVICE NOT AVAILABLE", frame);
}

__attribute__((interrupt)) void double_fault_handler(struct interrupt_frame *frame)
{
    panic("DOUBLE FAULT", frame);
}

__attribute__((interrupt)) void invalid_tss_handler(struct interrupt_frame *frame)
{
    panic("INVALID TSS", frame);
}

__attribute__((interrupt)) void segment_not_present_handler(struct interrupt_frame *frame)
{
    panic("SEGMENT NOT PRESENT", frame);
}

__attribute__((interrupt)) void stack_fault_handler(struct interrupt_frame *frame)
{
    panic("STACK FAULT", frame);
}

__attribute__((interrupt)) void floating_point_error_handler(struct interrupt_frame *frame)
{
    panic("FLOATING POINT ERROR", frame);
}

__attribute__((interrupt)) void alignment_check_handler(struct interrupt_frame *frame)
{
    panic("ALIGNMENT CHECK", frame);
}

__attribute__((interrupt)) void machine_check_handler(struct interrupt_frame *frame)
{
    panic("MACHINE CHECK", frame);
}

__attribute__((interrupt)) void simd_floating_point_handler(struct interrupt_frame *frame)
{
    panic("SIMD FLOATING POINT", frame);
}

__attribute__((interrupt)) void virtualization_exception_handler(struct interrupt_frame *frame)
{
    panic("VIRTUALIZATION EXCEPTION", frame);
}

__attribute__((interrupt)) void kb_handler(struct interrupt_frame *frame)
{
    uint8_t scancode = inb(0x60);

    pic_end_master();
}

void idt_set_entry(uint8_t vector, void *isr, uint8_t flags)
{
    idt_entry *descriptor = &idt[vector];

    descriptor->base_low = (uint64_t)isr & 0xFFFF;
    descriptor->cs = 0x08;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->base_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->base_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->rsv0 = 0;
}

void init_idt()
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry) * 256 - 1;

    idt_set_entry(0, (uint64_t)zero_division_handler, 0x8E);
    idt_set_entry(1, (uint64_t)debug_handler, 0x8E);
    idt_set_entry(2, (uint64_t)non_maskable_interrupt_handler, 0x8E);
    idt_set_entry(3, (uint64_t)breakpoint_handler, 0x8E);
    idt_set_entry(4, (uint64_t)overflow_handler, 0x8E);
    idt_set_entry(5, (uint64_t)bound_range_exceeded_handler, 0x8E);
    idt_set_entry(6, (uint64_t)invalid_opcode_handler, 0x8E);
    idt_set_entry(7, (uint64_t)device_not_available_handler, 0x8E);
    idt_set_entry(8, (uint64_t)double_fault_handler, 0x8E);
    idt_set_entry(10, (uint64_t)invalid_tss_handler, 0x8E);
    idt_set_entry(11, (uint64_t)segment_not_present_handler, 0x8E);
    idt_set_entry(12, (uint64_t)stack_fault_handler, 0x8E);
    idt_set_entry(13, (uint64_t)gpf_handler, 0x8E);
    idt_set_entry(14, (uint64_t)pf_handler, 0x8E);
    idt_set_entry(16, (uint64_t)floating_point_error_handler, 0x8E);
    idt_set_entry(17, (uint64_t)alignment_check_handler, 0x8E);
    idt_set_entry(18, (uint64_t)machine_check_handler, 0x8E);
    idt_set_entry(19, (uint64_t)simd_floating_point_handler, 0x8E);
    idt_set_entry(20, (uint64_t)virtualization_exception_handler, 0x8E);

    idt_set_entry(33, (uint64_t)kb_handler, 0x8E);

    remap_pic();
    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);

    __asm__ volatile("lidt %0"
                     :
                     : "m"(idtr));
    __asm__ volatile("sti");
}