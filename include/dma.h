#ifndef CEBLAR_DMA_H
#define CEBLAR_DMA_H
#include <stdint.h>

typedef struct dma_buffer { void *virtual_address; uint32_t physical_address; uint32_t length; uint32_t pages; } dma_buffer_t;
void dma_init(void);
int dma_alloc(uint32_t length, dma_buffer_t *buffer);
void dma_free(dma_buffer_t *buffer);

#endif
