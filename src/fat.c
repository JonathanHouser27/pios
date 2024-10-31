#include "fat.h"
#include "sd.h"
#include "rprintf.h"
#include "serial.h"

#define SECTOR_SIZE 512

// Global variables
struct boot_sector *bs;
char bootSector[512];
char fat_table[8 * 512]; // Adjusted size
unsigned int root_sector;

// Custom strcmp function
int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

// Custom strncpy function using unsigned long
void my_strncpy(char *dest, const char *src, unsigned long n) {
    while (n && (*src)) {
        *dest++ = *src++;
        n--;
    }
    while (n--) {
        *dest++ = '\0';
    }
}

// Custom toupper function
char my_toupper(char c) {
    return (c >= 'a' && c <= 'z') ? (c - 'a' + 'A') : c;
}

// FAT Initialization
int fatInit() {
    sd_readblock(0, (unsigned char *)bootSector, 1); // Read boot sector
    bs = (struct boot_sector *)bootSector; // Cast to boot_sector struct

    // Validate the boot signature
    if (bs->boot_signature != 0xAA55) {
        return -1; // Invalid boot signature
    }

    // Validate filesystem type
    if (my_strcmp(bs->fs_type, "FAT12") != 0) {
        return -1; // Unsupported filesystem type
    }

    // Read FAT table
    for (int i = 0; i < bs->num_fat_tables; i++) {
        sd_readblock(bs->num_reserved_sectors + i * bs->num_sectors_per_fat,
                     (unsigned char *)(fat_table + (i * bs->num_sectors_per_fat * SECTOR_SIZE)),
                     bs->num_sectors_per_fat);
    }

    // Compute root sector
    root_sector = bs->num_reserved_sectors + (bs->num_fat_tables * bs->num_sectors_per_fat);

    return 0; // Success
}

// FAT Open Function
int fatOpen(const char *filename, struct root_directory_entry *rde) {
    char upper_filename[12] = { ' ' };

    // Convert filename to uppercase
    for (int i = 0; i < 8; i++) {
        if (i < strlen(filename)) {
            upper_filename[i] = my_toupper(filename[i]);
        }
    }
    for (int i = 8; i < 11; i++) {
        upper_filename[i] = ' '; // Fill extension with spaces
    }
    upper_filename[11] = '\0'; // Null-terminate

    // Read root directory entries
    struct root_directory_entry entry;
    for (int i = 0; i < bs->num_root_dir_entries; i++) {
        sd_readblock(root_sector + (i * 32) / SECTOR_SIZE, (unsigned char *)&entry, 1);

        // Combine name and extension for comparison
        char combined_name[12];
        my_strncpy(combined_name, entry.file_name, 8);
        my_strncpy(combined_name + 8, entry.file_extension, 3);
        combined_name[11] = '\0'; // Null-terminate

        if (my_strcmp(combined_name, upper_filename) == 0) {
            *rde = entry; // Found the file
            return 0; // Success
        }
    }

    return -1; // File not found
}

// FAT Read Function
int fatRead(struct root_directory_entry *rde, char *buffer, int size) {
    unsigned int cluster = rde->cluster; // Starting cluster
    unsigned int bytes_read = 0;

    while (bytes_read < size) {
        // Calculate sector number
        unsigned int sector = (cluster - 2) * bs->num_sectors_per_cluster + root_sector;
        sd_readblock(sector, (unsigned char *)(buffer + bytes_read), 1);
        bytes_read += bs->bytes_per_sector;

        // Get next cluster from FAT
        if (cluster % 2 == 0) { // even cluster number
            cluster = (fat_table[cluster * 3 / 2] & 0x0F) | (fat_table[cluster * 3 / 2 + 1] << 4);
        } else { // odd cluster number
            cluster = (fat_table[cluster * 3 / 2] >> 4) | (fat_table[cluster * 3 / 2 + 1] & 0x0F) << 4;
        }

        if (cluster >= 0xFF8) break; // End of file
    }

    return bytes_read; // Number of bytes read
}
