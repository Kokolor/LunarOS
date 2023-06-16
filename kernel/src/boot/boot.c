#include <framebuffer/font.h>
#include "boot.h"

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0};

void put_progress_bar(int progress, int max_progress)
{
    int fillWidth = (progress * PROGRESS_BAR_WIDTH) / max_progress;
    uint32_t color = interpolate_color(COLOR_START, COLOR_END, progress, max_progress);

    for (int i = 0; i < fillWidth; i++)
    {
        put_rectangle(PROGRESS_BAR_X + i, PROGRESS_BAR_Y, 1, PROGRESS_BAR_HEIGHT, color);
    }

    put_rectangle(PROGRESS_BAR_X + fillWidth, PROGRESS_BAR_Y, PROGRESS_BAR_WIDTH - fillWidth, PROGRESS_BAR_HEIGHT, 0xffffff);
}

void boot()
{
    struct limine_memmap_entry *memmap_entry = memmap_request.response->entries;
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
    init_pmm(memmap_entry, memmap_request.response->entry_count);
    put_progress_bar(6, 6);
    delay(20);
    printk_success("PMM initialized\n");
    put_progress_bar(6, 6);
    delay(20);

    term.color = 0xffffff;
    void *test_block = pmm_alloc_block();
    if (test_block != NULL)
    {
        printk("Successfully allocated block at %p\n", test_block);
    }
    else
    {
        printk("Failed to allocate block\n");
    }

    pmm_free_block(test_block);
    printk("Freed block at %p\n", test_block);

    void *ptr1 = malloc(10);
    void *ptr2 = malloc(20);

    if (ptr1 && ptr2)
    {
        printk("Allocation successful.\n");

        free(ptr1);
        free(ptr2);
        printk("Deallocation successful.\n");
    }
    else
    {
        printk("Allocation failed.\n");
    }

    char Boot_txt[] = "Hello, world!";
    create_file("boot.txt", sizeof(Boot_txt));
    write_file("boot.txt", Boot_txt, sizeof(Boot_txt));
    if (Boot_txt != NULL)
    {
        printk("Successfully created boot.txt\n\n");
    }
    else
    {
        printk_error("Failed to create boot.txt\n");
    }
    create_lunar_script("script1.luscr", "print[Hello world from Lunar Script!]\nlist_files[]");

    printk_success("Boot complete. Welcome to LunarOS!\n");

    delay(20);
    put_rectangle(PROGRESS_BAR_X, PROGRESS_BAR_Y, PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT, 0x000000);

    shell_prompt();
    put_char('_', term.cursor_x, term.cursor_y, &term);
}