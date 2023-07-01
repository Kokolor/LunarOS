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

void read_dir(uint32_t first_cluster)
{
    uint32_t sector = bs->reserved_sector_count + (bs->fat_count * bs->table_size_32) + ((first_cluster - 2) * bs->sectors_per_cluster);

    struct dir_entry entries[bs->bytes_per_sector / sizeof(struct dir_entry)];

    bool found_file = false;
    char filename[12];

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
                found_file = true;
                memcpy(filename, entries[j].name, 11);
                filename[11] = '\0';
                break;
            }
        }

        if (found_file)
        {
            break;
        }
    }

    if (found_file)
    {
        printk("File found in the directory:\n%s\n", filename);
    }
    else
    {
        printk("No files found in the directory.\n");
    }
}
