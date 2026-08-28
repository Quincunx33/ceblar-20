#include <stdint.h>
#include "block.h"
#include "drivers.h"

#define BLOCK_MAX_DEVICES 4u
static block_device_t *devices[BLOCK_MAX_DEVICES];
static uint32_t device_count;

void block_init(void){device_count=0;for(uint32_t i=0;i<BLOCK_MAX_DEVICES;i++)devices[i]=0;serial_write("block: generic sector layer ready\n");}
int block_register(block_device_t *device){if(!device||!device->present||device->sector_size!=512u||!device->read_sector||device_count>=BLOCK_MAX_DEVICES)return-1;devices[device_count++]=device;return 0;}
block_device_t *block_default(void){return device_count?devices[0]:0;}
int block_read_sector(uint32_t lba,void *buffer){block_device_t*d=block_default();return d&&buffer?d->read_sector(lba,buffer):-1;}
int block_write_sector(uint32_t lba,const void *buffer){block_device_t*d=block_default();return d&&buffer&&d->write_sector?d->write_sector(lba,buffer):-1;}
