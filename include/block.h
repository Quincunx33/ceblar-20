#ifndef CEBLAR_BLOCK_H
#define CEBLAR_BLOCK_H
#include <stdint.h>
typedef int (*block_read_fn)(uint32_t lba,void *buffer);
typedef int (*block_write_fn)(uint32_t lba,const void *buffer);
typedef struct block_device {const char *name;uint32_t sector_size;uint64_t sector_count;block_read_fn read_sector;block_write_fn write_sector;uint8_t present;} block_device_t;
void block_init(void);int block_register(block_device_t *device);block_device_t *block_default(void);int block_read_sector(uint32_t lba,void *buffer);int block_write_sector(uint32_t lba,const void *buffer);
#endif
