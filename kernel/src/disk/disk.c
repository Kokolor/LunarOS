#include "disk.h"

struct boot_sector *bs;

void read_sector(uint32_t sector, void *buffer)
{
    outb(ATA_REG_DRIVESEL, 0xE0 | ((sector & 0x0F000000) >> 24));
    outb(ATA_REG_SECTORCOUNT, 1);
    outb(ATA_REG_LBA_LO, sector & 0xFF);
    outb(ATA_REG_LBA_MID, (sector & 0xFF00) >> 8);
    outb(ATA_REG_LBA_HI, (sector & 0xFF0000) >> 16);
    outb(ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    while ((inb(ATA_REG_COMMAND) & 0x08) == 0)
        ;

    insw(ATA_REG_DATA, buffer, 256);
}

void write_sector(uint32_t sector, void *buffer)
{
    outb(ATA_REG_DRIVESEL, 0xE0 | ((sector & 0x0F000000) >> 24));
    outb(ATA_REG_SECTORCOUNT, 1);
    outb(ATA_REG_LBA_LO, sector & 0xFF);
    outb(ATA_REG_LBA_MID, (sector & 0xFF00) >> 8);
    outb(ATA_REG_LBA_HI, (sector & 0xFF0000) >> 16);
    outb(ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

    while ((inb(ATA_REG_COMMAND) & 0x08) == 0)
        ;

    outsw(ATA_REG_DATA, buffer, 256);
}

void read_dir(uint32_t first_cluster)
{
    uint32_t sector = bs->reserved_sector_count + (bs->fat_count * bs->table_size_32) + ((first_cluster - 2) * bs->sectors_per_cluster);

    struct dir_entry entries[bs->bytes_per_sector / sizeof(struct dir_entry)];

    bool found_files = false;

    for (uint32_t i = 0; i < bs->sectors_per_cluster; i++)
    {
        read_sector(sector + i, entries);

        for (uint32_t j = 0; j < bs->bytes_per_sector / sizeof(struct dir_entry); j++)
        {
            if (entries[j].name[0] == 0x00)
            {
                return;
            }
            else if ((uint8_t)entries[j].name[0] == 0xE5)
            {
                continue;
            }

            if (!(entries[j].attr & 0x10))
            {
                found_files = true;
                char filename[13];
                memset(filename, 0, sizeof(filename));
                memcpy(filename, entries[j].name, 11);
                printk("File found: %s\n", filename);
            }
        }
    }

    if (!found_files)
    {
        printk("No files found in the directory.\n");
    }
}

int check_disk()
{
    if (bs->boot_sector_signature != 0xAA55)
    {
        set_color(0xff0000);
        printk("The disk is not compatible with the operating system.\n");
        while (1)
            ;
    }

    if (strncmp((char *)bs->fs_type, "FAT32", 5) != 0)
    {
        set_color(0xff0000);
        printk("The disk is not formatted as FAT32.\n");
        while (1)
            ;
    }

    if (bs->reserved[0] != 0x00)
    {
        set_color(0xff0000);
        printk("The disk is reserved.\n");
        while (1)
            ;
    }

    set_color(0x00ff00);
    printk("The disk is compatible with the operating system.\n\n");
    set_color(0x00ffff);

    return 1;
}