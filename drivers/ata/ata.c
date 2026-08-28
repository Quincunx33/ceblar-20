#include <stdint.h>
#include "ata.h"
#include "block.h"
#include "drivers.h"

#define ATA_DATA 0x1f0
#define ATA_SECTOR_COUNT 0x1f2
#define ATA_LBA0 0x1f3
#define ATA_LBA1 0x1f4
#define ATA_LBA2 0x1f5
#define ATA_DRIVE 0x1f6
#define ATA_STATUS 0x1f7
#define ATA_COMMAND 0x1f7
#define ATA_CONTROL 0x3f6
#define ATA_CMD_READ 0x20
#define ATA_CMD_WRITE 0x30
#define ATA_CMD_IDENTIFY 0xec
#define ATA_CMD_CACHE_FLUSH 0xe7
#define ATA_STATUS_ERR 0x01
#define ATA_STATUS_DRQ 0x08
#define ATA_STATUS_DF 0x20
#define ATA_STATUS_BSY 0x80
#define ATA_TIMEOUT 100000u

static int present;
static block_device_t ata_block={.name="ata0",.sector_size=512u,.sector_count=0x10000000ULL,.read_sector=ata_read_sector,.write_sector=ata_write_sector,.present=0};
static inline void outb(uint16_t port, uint8_t value) { __asm__ volatile("outb %0,%1" : : "a"(value), "Nd"(port)); }
static inline uint8_t inb(uint16_t port) { uint8_t value; __asm__ volatile("inb %1,%0" : "=a"(value) : "Nd"(port)); return value; }
static inline void outw(uint16_t port, uint16_t value) { __asm__ volatile("outw %0,%1" : : "a"(value), "Nd"(port)); }
static inline uint16_t inw(uint16_t port) { uint16_t value; __asm__ volatile("inw %1,%0" : "=a"(value) : "Nd"(port)); return value; }
static void ata_delay(void) { (void)inb(ATA_CONTROL); (void)inb(ATA_CONTROL); (void)inb(ATA_CONTROL); (void)inb(ATA_CONTROL); }
static int ata_wait(uint8_t wanted) { uint32_t n; uint8_t status = 0; for (n = 0; n < ATA_TIMEOUT; n++) { status = inb(ATA_STATUS); if (status & (ATA_STATUS_ERR | ATA_STATUS_DF)) return -1; if (!(status & ATA_STATUS_BSY) && (status & wanted) == wanted) return 0; } return -1; }
static int ata_select(uint32_t lba) { if (lba >= 0x10000000u) return -1; outb(ATA_DRIVE, (uint8_t)(0xe0u | ((lba >> 24) & 0x0fu))); ata_delay(); outb(ATA_SECTOR_COUNT, 1); outb(ATA_LBA0, (uint8_t)lba); outb(ATA_LBA1, (uint8_t)(lba >> 8)); outb(ATA_LBA2, (uint8_t)(lba >> 16)); return 0; }
static int ata_transfer(uint32_t lba, void *buffer, int write) { uint32_t i; uint16_t *words = (uint16_t *)buffer; if (!present || !buffer || ata_select(lba) != 0) return -1; outb(ATA_COMMAND, write ? ATA_CMD_WRITE : ATA_CMD_READ); if (ata_wait(ATA_STATUS_DRQ) != 0) return -1; if (write) { for (i = 0; i < 256; i++) outw(ATA_DATA, words[i]); outb(ATA_COMMAND, ATA_CMD_CACHE_FLUSH); return ata_wait(0); } for (i = 0; i < 256; i++) words[i] = inw(ATA_DATA); return 0; }
int ata_init(void) { uint8_t status; uint16_t identify[256]; uint32_t i; present = 0; outb(ATA_DRIVE, 0xa0); ata_delay(); outb(ATA_SECTOR_COUNT, 0); outb(ATA_LBA0, 0); outb(ATA_LBA1, 0); outb(ATA_LBA2, 0); outb(ATA_COMMAND, ATA_CMD_IDENTIFY); status = inb(ATA_STATUS); if (status == 0 || status == 0xff) { serial_write("ata: primary master absent\n"); return -1; } for (i = 0; i < ATA_TIMEOUT; i++) { status = inb(ATA_STATUS); if (status & (ATA_STATUS_ERR | ATA_STATUS_DF)) { serial_write("ata: identify error\n"); return -1; } if (!(status & ATA_STATUS_BSY) && (status & ATA_STATUS_DRQ)) break; } if (i == ATA_TIMEOUT) { serial_write("ata: identify timeout\n"); return -1; } for (i = 0; i < 256; i++) identify[i] = inw(ATA_DATA); (void)identify; present = 1; ata_block.present=1; (void)block_register(&ata_block); serial_write("ata: primary master ready, LBA28 PIO\n"); return 0; }
int ata_present(void) { return present; }
int ata_read(uint32_t lba, void *buffer, uint32_t sectors) { uint32_t i; uint8_t *p = (uint8_t *)buffer; if (!buffer || sectors == 0 || lba >= 0x10000000u || sectors > 0x10000000u - lba) return -1; for (i = 0; i < sectors; i++) if (ata_transfer(lba + i, p + i * 512u, 0) != 0) return -1; return 0; }
int ata_write(uint32_t lba, const void *buffer, uint32_t sectors) { uint32_t i; const uint8_t *p = (const uint8_t *)buffer; if (!buffer || sectors == 0 || lba >= 0x10000000u || sectors > 0x10000000u - lba) return -1; for (i = 0; i < sectors; i++) if (ata_transfer(lba + i, (void *)(p + i * 512u), 1) != 0) return -1; return 0; }
int ata_read_sector(uint32_t lba, void *buffer) { return ata_read(lba, buffer, 1); }
int ata_write_sector(uint32_t lba, const void *buffer) { return ata_write(lba, buffer, 1); }
