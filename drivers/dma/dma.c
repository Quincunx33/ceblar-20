#include <stdint.h>
#include "dma.h"
#include "mm.h"
#include "drivers.h"

#define DMA_MAX_ADDRESS 0x00400000u
static int initialized;
void dma_init(void){initialized=1;serial_write("dma: 32-bit contiguous allocator ready\n");}
int dma_alloc(uint32_t length,dma_buffer_t*buffer){uint32_t pages,first=0,p,i;if(!initialized||!buffer||length==0)return-1;pages=(length+PAGE_SIZE-1u)/PAGE_SIZE;for(i=0;i<pages;i++){p=pmm_alloc_page();if(!p||p>=DMA_MAX_ADDRESS||(i&&p!=first+i*PAGE_SIZE)){if(p)pmm_free_page(p);for(uint32_t j=0;j<i;j++)pmm_free_page(first+j*PAGE_SIZE);return-1;}if(i==0)first=p;}buffer->virtual_address=(void*)first;buffer->physical_address=first;buffer->length=length;buffer->pages=pages;return 0;}
void dma_free(dma_buffer_t*buffer){if(!buffer||!buffer->physical_address||buffer->pages==0)return;for(uint32_t i=0;i<buffer->pages;i++)pmm_free_page(buffer->physical_address+i*PAGE_SIZE);buffer->virtual_address=0;buffer->physical_address=0;buffer->length=0;buffer->pages=0;}
