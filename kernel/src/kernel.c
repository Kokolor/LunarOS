#include <stdint.h>
#include <stddef.h>
#include <limine/limine.h>
#include <debug/debug.h>
#include <arch/x86_64/gdt.h>
#include <framebuffer/font.h>
#include <boot/boot.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

void _start(void)
{
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    init_framebuffer(framebuffer);
    init_terminal(&term, 0, 0, 0xffffff);

    boot();

    while (1)
        ;
}
