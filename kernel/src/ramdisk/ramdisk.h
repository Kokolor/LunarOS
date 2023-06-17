#include <stdint.h>
#include <lib/lib.h>
#include <mm/pmm.h>

#define FILENAME_MAX_LENGTH 255
#define MAX_FILES 1024

void *create_file(const char *name, size_t size);
void *read_file(const char *name, void *buffer, size_t size);
void write_file(const char *name, const void *data, size_t size);
void list_files();
void print_ramdisk_info();