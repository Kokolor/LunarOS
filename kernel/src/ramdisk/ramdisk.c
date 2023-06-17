#include "ramdisk.h"

typedef struct
{
    char name[FILENAME_MAX_LENGTH];
    void *start;
    size_t size;
} file;

file files[MAX_FILES];
size_t file_count = 0;

void *create_file(const char *name, size_t size)
{
    if (file_count >= MAX_FILES)
    {
        printk_error("Unable to create file: Exceeded maximum number of files allowed.\n");
        return NULL;
    }

    void *start = malloc(size);
    if (!start)
    {
        printk_error("Failed to create file: Not enough memory to allocate for the file.");
        return NULL;
    }

    strncpy(files[file_count].name, name, FILENAME_MAX_LENGTH);
    files[file_count].start = start;
    files[file_count].size = size;
    file_count++;

    return start;
}

void *read_file(const char *name, void *buffer, size_t size)
{
    for (size_t i = 0; i < file_count; i++)
    {
        if (strncmp(files[i].name, name, FILENAME_MAX_LENGTH) == 0)
        {
            memcpy(buffer, files[i].start, MIN(size, files[i].size));
            return buffer;
        }
    }

    printk_error("Unable to read file: File not found.\n");
    return NULL;
}

void write_file(const char *name, const void *data, size_t size)
{
    for (size_t i = 0; i < file_count; i++)
    {
        if (strncmp(files[i].name, name, FILENAME_MAX_LENGTH) == 0)
        {
            memcpy(files[i].start, data, MIN(size, files[i].size));
            return;
        }
    }

    printk_error("Unable to write file: File not found.\n");
}

void list_files()
{
    for (size_t i = 0; i < file_count; i++)
    {
        printk("File name: %s\n", files[i].name);
        printk("Start address: %p\n", files[i].start);
        printk("Size: %l bytes\n", files[i].size);
        printk("\n");
    }
}

void print_ramdisk_info() 
{
    size_t totalSize = 0;
    size_t smallestFileSize = -1;
    size_t largestFileSize = 0;
    char *smallestFile = NULL;
    char *largestFile = NULL;

    printk("------ RAMDisk Information ------\n");
    printk("Total number of files: %l\n", file_count);
    
    for (size_t i = 0; i < file_count; i++) 
    {
        size_t currentFileSize = files[i].size;
        totalSize += currentFileSize;

        if (currentFileSize < smallestFileSize)
        {
            smallestFileSize = currentFileSize;
            smallestFile = files[i].name;
        }
        
        if (currentFileSize > largestFileSize)
        {
            largestFileSize = currentFileSize;
            largestFile = files[i].name;
        }

        printk("\nFile #%l Information:\n", i+1);
        printk("File Name: %s\n", files[i].name);
        printk("Start Address: %p\n", files[i].start);
        printk("File Size: %l bytes\n", currentFileSize);
    }

    printk("\n------ Additional Information ------\n");
    printk("Total memory used: %l bytes\n", totalSize);
    printk("Average file size: %l bytes\n", file_count > 0 ? totalSize / file_count : 0);
    printk("Smallest file: %s (Size: %l bytes)\n", smallestFile, smallestFileSize);
    printk("Largest file: %s (Size: %l bytes)\n", largestFile, largestFileSize);
    printk("------------------------------------\n");
}
