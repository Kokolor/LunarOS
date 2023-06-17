#include <stdint.h>
#include <limine/limine.h>

#define PROGRESS_BAR_WIDTH 600
#define PROGRESS_BAR_HEIGHT 20
#define PROGRESS_BAR_X (1280 / 2 - PROGRESS_BAR_WIDTH / 2)
#define PROGRESS_BAR_Y (720 / 2 - PROGRESS_BAR_HEIGHT / 2)
#define COLOR_START 0x0000ff
#define COLOR_END 0x00ff00

extern struct limine_internal_module background;
extern struct limine_internal_module *internal_modules[];
extern struct limine_module_request module_request;

void boot();

#pragma pack(push, 1)

typedef struct
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;

typedef struct
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} RGBTRIPLE;

#pragma pack(pop) // Fin de l'empaquetage
