#ifndef CEBLAR_ATA_H
#define CEBLAR_ATA_H
#include <stdint.h>

int ata_init(void);
int ata_present(void);
int ata_read(uint32_t lba, void *buffer, uint32_t sectors);
int ata_write(uint32_t lba, const void *buffer, uint32_t sectors);
int ata_read_sector(uint32_t lba, void *buffer);
int ata_write_sector(uint32_t lba, const void *buffer);

#endif
