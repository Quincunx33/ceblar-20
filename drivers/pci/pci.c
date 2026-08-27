#include <stdint.h>
#include "pci.h"
#include "drivers.h"
#include "kernel.h"

static inline void outl(uint16_t port,uint32_t value){__asm__ volatile("outl %0,%1"::"a"(value),"Nd"(port));}
static inline uint32_t inl(uint16_t port){uint32_t value;__asm__ volatile("inl %1,%0":"=a"(value):"Nd"(port));return value;}
static uint32_t address(uint8_t bus,uint8_t slot,uint8_t function,uint8_t offset){return 0x80000000u|((uint32_t)bus<<16)|((uint32_t)(slot&31u)<<11)|((uint32_t)(function&7u)<<8)|(offset&0xfcu);}
uint32_t pci_config_read32(uint8_t bus,uint8_t slot,uint8_t function,uint8_t offset){outl(0xcf8,address(bus,slot,function,offset));return inl(0xcfcu);}
static int read_device(uint8_t bus,uint8_t slot,uint8_t function,pci_device_t*out){uint32_t id=pci_config_read32(bus,slot,function,0);uint32_t classreg;if((id&0xffffu)==0xffffu)return-1;classreg=pci_config_read32(bus,slot,function,8);out->bus=bus;out->slot=slot;out->function=function;out->vendor=(uint16_t)(id&0xffffu);out->device=(uint16_t)(id>>16);out->class_code=(uint8_t)(classreg>>24);out->subclass=(uint8_t)(classreg>>16);out->prog_if=(uint8_t)(classreg>>8);out->header_type=(uint8_t)(pci_config_read32(bus,slot,function,0x0c)>>16);return 0;}
void pci_init(void){pci_device_t d;uint32_t count=0;uint8_t slot;for(slot=0;slot<32u;slot++)if(read_device(0,slot,0,&d)==0){count++;if(d.header_type&0x80u)for(uint8_t f=1;f<8u;f++)if(read_device(0,slot,f,&d)==0)count++;}kprintf("pci: bus0 enumeration found %u device functions\n",count);}
int pci_find_class(uint8_t class_code,uint8_t subclass,pci_device_t*out){uint8_t slot,f;if(!out)return-1;for(slot=0;slot<32u;slot++)for(f=0;f<8u;f++)if(read_device(0,slot,f,out)==0&&out->class_code==class_code&&out->subclass==subclass)return 0;return-1;}
