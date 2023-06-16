#include <stdint.h>
#include <stddef.h>
#include <limine/limine.h>
#include <debug/debug.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>
#include <mm/pmm.h>

#define PROGRESS_BAR_WIDTH 600
#define PROGRESS_BAR_HEIGHT 20
#define PROGRESS_BAR_X (1280 / 2 - PROGRESS_BAR_WIDTH / 2)
#define PROGRESS_BAR_Y 700
#define COLOR_START 0x0000ff
#define COLOR_END 0x00ff00

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

void put_progress_bar(int progress, int max_progress)
{
    int fillWidth = (progress * PROGRESS_BAR_WIDTH) / max_progress;
    uint32_t color = interpolate_color(COLOR_START, COLOR_END, progress, max_progress);

    // Dessine la partie remplie de la barre de progression
    for (int i = 0; i < fillWidth; i++)
    {
        put_rectangle(PROGRESS_BAR_X + i, PROGRESS_BAR_Y, 1, PROGRESS_BAR_HEIGHT, color);
    }

    // Dessine la partie non remplie de la barre de progression
    put_rectangle(PROGRESS_BAR_X + fillWidth, PROGRESS_BAR_Y, PROGRESS_BAR_WIDTH - fillWidth, PROGRESS_BAR_HEIGHT, 0xffffff);
}

void boot()
{
    put_progress_bar(0, 6);
    delay(20);
    printk_success("Framebuffer initialized\n");
    put_progress_bar(1, 6);
    delay(20);
    init_gdt();
    put_progress_bar(2, 6);
    delay(20);
    printk_success("GDT initialized\n");
    put_progress_bar(3, 6);
    delay(20);
    init_idt();
    put_progress_bar(4, 6);
    delay(20);
    printk_success("IDT initialized\n");
    put_progress_bar(5, 6);
    delay(20);
    init_pmm();
    put_progress_bar(6, 6);
    delay(20);
    printk_success("PMM initialized\n");
    put_progress_bar(6, 6);
    delay(20);

    term.color = 0xffffff;
    printk_success("Boot complete. Welcome to LunarOS!\n");

    delay(20);
    put_rectangle(PROGRESS_BAR_X, PROGRESS_BAR_Y, PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT, 0x000000);

    shell_prompt();
    put_char('_', term.cursor_x, term.cursor_y, &term);
}

void _start(void)
{
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    init_framebuffer(framebuffer);
    init_terminal(&term, 0, 0, 0xffffff);
    clear_screen(0x000000);

    put_rectangle(PROGRESS_BAR_X, PROGRESS_BAR_Y, PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT, 0xffffff);

    boot();

    while (1)
        ;
}
