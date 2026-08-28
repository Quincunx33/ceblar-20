#include <stdint.h>
#include "virtio_blk.h"
#include "pci.h"
#include "dma.h"
#include "mm.h"
#include "drivers.h"
#include "kernel.h"

#define VIRTIO_VENDOR 0x1af4u
#define VIRTIO_BLK_DEVICE 0x1001u
#define VIRTQ_DESC_F_NEXT 1u
#define VIRTQ_DESC_F_WRITE 2u
#define VIRTIO_STATUS_ACK 1u
#define VIRTIO_STATUS_DRIVER 2u
#define VIRTIO_STATUS_DRIVER_OK 4u
#define VIRTIO_STATUS_FAILED 128u
#define VIRTIO_BLK_T_IN 0u
#define VIRTIO_BLK_QUEUE 0u
#define VIRTIO_QUEUE_SIZE 8u
#define VIRTIO_QUEUE_ALIGN 4096u
#define VIRTIO_POLL_LIMIT 1000000u

typedef struct virtq_desc {uint32_t address,length;uint16_t flags,next;} __attribute__((packed)) virtq_desc_t;
typedef struct virtq_avail {uint16_t flags,idx,ring[VIRTIO_QUEUE_SIZE];} __attribute__((packed)) virtq_avail_t;
typedef struct virtq_used_elem {uint32_t id,length;} __attribute__((packed)) virtq_used_elem_t;
typedef struct virtq_used {uint16_t flags,idx;virtq_used_elem_t ring[VIRTIO_QUEUE_SIZE];} __attribute__((packed)) virtq_used_t;
typedef struct virtio_blk_req {uint32_t type,reserved;uint64_t sector;} __attribute__((packed)) virtio_blk_req_t;
static uint16_t io_base;static int present;static dma_buffer_t queue_dma,request_dma;static virtq_desc_t*desc;static volatile virtq_avail_t*avail;static volatile virtq_used_t*used;static virtio_blk_req_t*request;static uint8_t*data;static uint8_t*status;static uint16_t last_used;
static inline void outb(uint16_t port,uint8_t value){__asm__ volatile("outb %0,%1"::"a"(value),"Nd"(port));}static inline void outw(uint16_t port,uint16_t value){__asm__ volatile("outw %0,%1"::"a"(value),"Nd"(port));}static inline uint8_t inb(uint16_t port){uint8_t value;__asm__ volatile("inb %1,%0":"=a"(value):"Nd"(port));return value;}static inline uint32_t inl(uint16_t port){uint32_t value;__asm__ volatile("inl %1,%0":"=a"(value):"Nd"(port));return value;}static inline void outl(uint16_t port,uint32_t value){__asm__ volatile("outl %0,%1"::"a"(value),"Nd"(port));}
static uint8_t cfg_status(void){return inb((uint16_t)(io_base+18u));}static void set_status(uint8_t value){outb((uint16_t)(io_base+18u),value);}static void fail(void){set_status((uint8_t)(cfg_status()|VIRTIO_STATUS_FAILED));}
void virtio_blk_init(void){pci_device_t d;pci_bar_t bar;uint32_t qsize,avail_offset,used_offset;if(pci_find_class(0x01u,0x00u,&d)!=0||d.vendor!=VIRTIO_VENDOR||d.device!=VIRTIO_BLK_DEVICE){serial_write("virtio-blk: legacy device absent\n");return;}if(pci_read_bar(&d,0,&bar)!=0||!bar.io||bar.base>0xffffu){serial_write("virtio-blk: usable I/O BAR absent\n");return;}io_base=(uint16_t)bar.base;set_status(0);(void)inl((uint16_t)(io_base+0u));outl((uint16_t)(io_base+4u),0);set_status(VIRTIO_STATUS_ACK|VIRTIO_STATUS_DRIVER);outw((uint16_t)(io_base+14u),VIRTIO_BLK_QUEUE);qsize=(uint16_t)inl((uint16_t)(io_base+12u));if(qsize<VIRTIO_QUEUE_SIZE){fail();serial_write("virtio-blk: queue too small\n");return;}if(dma_alloc(8192u,&queue_dma)!=0||dma_alloc(PAGE_SIZE,&request_dma)!=0){if(queue_dma.physical_address)dma_free(&queue_dma);fail();serial_write("virtio-blk: DMA allocation failed\n");return;}avail_offset=sizeof(virtq_desc_t)*VIRTIO_QUEUE_SIZE;used_offset=(avail_offset+sizeof(virtq_avail_t)+VIRTIO_QUEUE_ALIGN-1u)&~(VIRTIO_QUEUE_ALIGN-1u);desc=(virtq_desc_t*)queue_dma.virtual_address;avail=(volatile virtq_avail_t*)((uint8_t*)queue_dma.virtual_address+avail_offset);used=(volatile virtq_used_t*)((uint8_t*)queue_dma.virtual_address+used_offset);request=(virtio_blk_req_t*)request_dma.virtual_address;data=(uint8_t*)request_dma.virtual_address+64u;status=data+512u;for(uint32_t i=0;i<8192u;i++)((volatile uint8_t*)queue_dma.virtual_address)[i]=0;for(uint32_t i=0;i<VIRTIO_QUEUE_SIZE;i++){desc[i].address=0;desc[i].length=0;desc[i].flags=0;desc[i].next=0;}desc[0].address=request_dma.physical_address;desc[0].length=sizeof(virtio_blk_req_t);desc[0].flags=VIRTQ_DESC_F_NEXT;desc[0].next=1;desc[1].address=request_dma.physical_address+64u;desc[1].length=512u;desc[1].flags=VIRTQ_DESC_F_NEXT|VIRTQ_DESC_F_WRITE;desc[1].next=2;desc[2].address=request_dma.physical_address+576u;desc[2].length=1;desc[2].flags=VIRTQ_DESC_F_WRITE;outl((uint16_t)(io_base+8u),queue_dma.physical_address>>12);set_status(VIRTIO_STATUS_ACK|VIRTIO_STATUS_DRIVER|VIRTIO_STATUS_DRIVER_OK);present=1;last_used=0;serial_write("virtio-blk: legacy queue ready\n");}
int virtio_blk_present(void){return present;}int virtio_blk_read_sector(uint32_t lba,void*buffer){if(!present||!buffer)return-1;request->type=VIRTIO_BLK_T_IN;request->reserved=0;request->sector=lba;*status=0xffu;for(uint32_t i=0;i<512u;i++)data[i]=0;avail->ring[avail->idx%VIRTIO_QUEUE_SIZE]=0;avail->idx++;__asm__ volatile("":::"memory");outw((uint16_t)(io_base+16u),VIRTIO_BLK_QUEUE);for(uint32_t wait=0;wait<VIRTIO_POLL_LIMIT;wait++){if(used->idx!=last_used){last_used=used->idx;if(*status!=0)return-1;for(uint32_t i=0;i<512u;i++)((uint8_t*)buffer)[i]=data[i];return 0;}__asm__ volatile("pause");}return-1;}
