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
    printk_success("Framebuffer initialized\n");
    init_gdt();
    printk_success("GDT initialized\n");
    init_idt();
    printk_success("IDT initialized\n");
    init_pmm(memmap_entry, memmap_request.response->entry_count);
    printk_success("PMM initialized\n\n");
    printk("Memory start : %p\n", memory_start);

    printk("\n");
}