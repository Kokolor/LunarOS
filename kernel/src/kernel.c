#include <stdint.h>
#include <stddef.h>
#include <pfa/pfa.h>
#include <disk/disk.h>
#include <debug/debug.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>
#include <framebuffer/framebuffer.h>

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = NULL};

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0};

struct limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0,
    .response = NULL};

void _start(void)
{
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    struct limine_memmap_entry *memmap_entry = memmap_request.response->entries;
    struct limine_bootloader_info_response *bootloader_info = bootloader_info_request.response;

    init_framebuffer(framebuffer);

    clear_screen(0x000000);

    init_gdt();
    init_idt();
    init_pfa(memmap_entry, memmap_request.response->entry_count);

    set_color(0xFFFFFF);
    printk("Welcome to ");
    set_color(0x00FF00);
    printk("LunarOS");
    set_color(0xFFFFFF);
    printk("!\n\n");

    set_color(0xFFFF00);
    printk("Bootloader:\n");
    set_color(0xFFFFFF);
    printk("    Name: ");
    set_color(0x00FFFF);
    printk("%s\n", bootloader_info->name);
    set_color(0xFFFFFF);
    printk("    Version: ");
    set_color(0x00FFFF);
    printk("%s\n\n", bootloader_info->version);
    set_color(0xFFFF00);

    set_color(0xFFFF00);
    printk("Framebuffer:\n");
    set_color(0xFFFFFF);
    printk("    Address: ");
    set_color(0x00FFFF);
    printk("0x%x\n", framebuffer->address);
    set_color(0xFFFFFF);
    printk("    Size: ");
    set_color(0x00FFFF);
    printk("%dx%dx%d\n\n", framebuffer->width, framebuffer->height, framebuffer->bpp);

    print_memory_info(memmap_entry, memmap_request.response->entry_count);

    void *ptr1 = malloc(500);
    if (ptr1 != NULL)
    {
        printk("Memory successfully allocated.\n");
    }
    else
    {
        printk("Failed to allocate memory.\n");
    }

    free(ptr1);
    printk("Memory successfully freed.\n");

    delay(100);
    clear_screen(0x000000);

    set_color(0xffffff);
    printk("Reading FAT32 disk...\n\n");
    set_color(0x00FFFF);

    bs = malloc(sizeof(struct boot_sector));
    if (bs == NULL)
    {
        printk("Failed to allocate memory for boot sector.\n");
        while (1)
            ;
    }

    read_sector(0, bs);

    printk("Bytes per sector: %d\n", bs->bytes_per_sector);
    printk("Sectors per cluster: %d\n", bs->sectors_per_cluster);
    printk("Reserved sector count: %d\n", bs->reserved_sector_count);
    printk("Number of FATs: %d\n", bs->fat_count);
    printk("Root directory entry count: %d\n", bs->root_dir_entries);
    printk("Total sectors (short): %d\n", bs->total_sectors_short);
    printk("Media type: %d\n", bs->media_type);
    printk("FAT size 16: %d\n", bs->table_size_16);
    printk("Sectors per track: %d\n", bs->sectors_per_track);
    printk("Head side count: %d\n", bs->head_side_count);
    printk("Hidden sector count: %d\n", bs->hidden_sector_count);
    printk("Total sectors (long): %d\n", bs->total_sectors_long);

    printk("\n"),
    set_color(0xffffff);

    read_dir(bs->root_cluster);

    while (1)
        ;
}
