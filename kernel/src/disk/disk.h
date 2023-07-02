#include <stdint.h>
#include <lib/lib.h>

#define ATA_PRIMARY_IO 0x1F0
#define ATA_REG_DATA ATA_PRIMARY_IO
#define ATA_REG_SECTORCOUNT ATA_PRIMARY_IO + 2
#define ATA_REG_LBA_LO ATA_PRIMARY_IO + 3
#define ATA_REG_LBA_MID ATA_PRIMARY_IO + 4
#define ATA_REG_LBA_HI ATA_PRIMARY_IO + 5
#define ATA_REG_DRIVESEL ATA_PRIMARY_IO + 6
#define ATA_REG_COMMAND ATA_PRIMARY_IO + 7
#define ATA_CMD_READ_PIO 0x20
#define ATA_CMD_WRITE_PIO 0x30

struct boot_sector
{
    uint8_t jump[3];
    uint8_t oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t fat_count;
    uint16_t root_dir_entries;
    uint16_t total_sectors_short;
    uint8_t media_type;
    uint16_t table_size_16;
    uint16_t sectors_per_track;
    uint16_t head_side_count;
    uint32_t hidden_sector_count;
    uint32_t total_sectors_long;
    uint32_t table_size_32;
    uint16_t ext_flags;
    uint16_t fat_version;
    uint32_t root_cluster;
    uint16_t fs_info_sector;
    uint16_t backup_boot_sector;
    uint8_t reserved[12];
    uint8_t drive_number;
    uint8_t reserved1;
    uint8_t boot_signature;
    uint32_t volume_id;
    uint8_t volume_label[11];
    uint8_t fs_type[8];
    uint8_t boot_code[420];
    uint16_t boot_sector_signature;
} __attribute__((packed));

struct dir_entry
{
    uint8_t name[11];
    uint8_t attr;
    uint8_t nt_res;
    uint8_t create_time_tenth;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t last_access_date;
    uint16_t cluster_high;
    uint16_t last_modif_time;
    uint16_t last_modif_date;
    uint16_t cluster_low;
    uint32_t size;
} __attribute__((packed));

extern struct boot_sector *bs;

void read_sector(uint32_t sector, void *buffer);
void write_sector(uint32_t sector, void *buffer);
void read_dir(uint32_t first_cluster);
void write_file(const char *filename, const void *data, uint32_t size);
int check_disk();