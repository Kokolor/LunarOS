#include <stdint.h>
// #include <ramdisk/ramdisk.h>
#include <limine/limine.h>

#define PROGRESS_BAR_WIDTH 600
#define PROGRESS_BAR_HEIGHT 20
#define PROGRESS_BAR_X (1280 / 2 - PROGRESS_BAR_WIDTH / 2)
#define PROGRESS_BAR_Y (720 / 2 - PROGRESS_BAR_HEIGHT / 2)
#define COLOR_START 0x0000ff
#define COLOR_END 0x00ff00

void boot();