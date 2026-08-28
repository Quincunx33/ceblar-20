#ifndef CEBLAR_VIRTIO_BLK_H
#define CEBLAR_VIRTIO_BLK_H
#include <stdint.h>
void virtio_blk_init(void);
int virtio_blk_present(void);
int virtio_blk_read_sector(uint32_t lba,void*buffer);
#endif
