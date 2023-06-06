#include <stdint.h>
#include <stddef.h>
#include <limine/limine.h>
#include <debug/debug.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

void _start(void)
{
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    init_framebuffer(framebuffer);
    init_terminal(&term, 0, 0, 0xffffff);
    clear_screen(0x0000ff);
    init_gdt();
    init_idt();
    put_string("Welcome to LunarOS!\n");
    shell_prompt();
    put_char('_', term.cursor_x, term.cursor_y, &term);

    while (1);
}
