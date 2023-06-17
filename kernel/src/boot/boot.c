#include <framebuffer/font.h>
#include "boot.h"

struct limine_file *file;

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0};

struct limine_internal_module background = {
    .path = "/background.bmp",
    .cmdline = " Background Image"};

struct limine_internal_module *internal_modules[] = {
    &background};

struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 1,
    .response = NULL,

    .internal_module_count = 1,
    .internal_modules = internal_modules};

uint32_t background_image[1280 * 720];

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

void load_background_image(struct limine_file *file)
{
    struct limine_memmap_entry *memmap_entry = memmap_request.response->entries;
    struct limine_module_response *module_response = module_request.response;
    struct limine_module_response *response = module_request.response;

    for (uint64_t i = 0; i < response->module_count; ++i)
    {
        if (strcmp(file->path, "/background.bmp") == 0)
        {
            BITMAPFILEHEADER *fileHeader = (BITMAPFILEHEADER *)file->address;
            BITMAPINFOHEADER *infoHeader = (BITMAPINFOHEADER *)((uint8_t *)file->address + sizeof(BITMAPFILEHEADER));
            RGBTRIPLE *image = (RGBTRIPLE *)((uint8_t *)file->address + fileHeader->bfOffBits);

            if (fileHeader->bfType != 0x4D42)
            {
                printk("The file is not a BMP file\n");
                continue;
            }

            if (infoHeader->biCompression != 0)
            {
                printk("The BMP image is compressed\n");
                continue;
            }

            if (infoHeader->biBitCount != 24)
            {
                printk("The BMP image is not 24bpp\n");
                continue;
            }

            for (int y = infoHeader->biHeight - 1; y >= 0; --y)
            {
                for (int x = 0; x < infoHeader->biWidth; ++x)
                {
                    RGBTRIPLE *pixel = &image[y * infoHeader->biWidth + x];
                    uint32_t color = (pixel->red << 16) | (pixel->green << 8) | pixel->blue;

                    put_pixel(x, infoHeader->biHeight - 1 - y, color);
                    background_image[(infoHeader->biHeight - 1 - y) * infoHeader->biWidth + x] = color;
                }
            }
        }
    }
}

void redraw_background(int x, int y, int width, int height)
{
    for (int j = y; j < y + height; j++)
    {
        for (int i = x; i < x + width; i++)
        {
            RGBTRIPLE *pixel = &background_image[(j * 1280) + i];
            uint32_t color = (pixel->red << 16) | (pixel->green << 8) | pixel->blue;

            put_pixel(i, j, color);
        }
    }
}

void boot()
{
    struct limine_memmap_entry *memmap_entry = memmap_request.response->entries;
    struct limine_module_response *module_response = module_request.response;
    struct limine_module_response *response = module_request.response;

    file = response->modules[0];

    load_background_image(file);

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
    printk_success("Ramdisk initialized\n\n");
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

    char update_lst[] = "Friday June 16:\nLunarOS ramdisk and write/read system is now up";
    create_file("updates.lst", sizeof(update_lst));
    write_file("updates.lst", update_lst, sizeof(update_lst));
    create_lunar_script("test.luscr", "print[Test LunarScript v1.0]\nlist_files[]");
    printk("\n");

    print_ramdisk_info();

    printk("\n");
    printk("Welcome to LunarOS! -- Type 'help' for the full command list\n");

    delay(20);
    redraw_background(PROGRESS_BAR_X, PROGRESS_BAR_Y, PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT);

    shell_prompt();
}