#include <stdint.h>
#include "acpi.h"
#include "drivers.h"
#include "kernel.h"

typedef struct rsdp_v1 { char signature[8]; uint8_t checksum; char oemid[6]; uint8_t revision; uint32_t rsdt; } __attribute__((packed)) rsdp_v1_t;
static int signature(const uint8_t*p){static const char s[8]={'R','S','D',' ','P','T','R',' '};for(uint32_t i=0;i<8;i++)if(p[i]!=(uint8_t)s[i])return 0;return 1;}
static int valid(const uint8_t*p){uint8_t sum=0;for(uint32_t i=0;i<20u;i++)sum=(uint8_t)(sum+p[i]);return sum==0;}
static rsdp_v1_t*scan(uint32_t begin,uint32_t end){for(uint32_t p=begin;p+20u<=end;p+=16u){uint8_t*q=(uint8_t*)p;if(signature(q)&&valid(q))return(rsdp_v1_t*)q;}return 0;}
void acpi_init(void){rsdp_v1_t*r=0;uint16_t ebda=(uint16_t)(*(volatile uint16_t*)0x40e);if(ebda)r=scan((uint32_t)ebda*16u,(uint32_t)ebda*16u+1024u);if(!r)r=scan(0x000e0000u,0x00100000u);if(r)kprintf("acpi: RSDP found revision %u rsdt=0x%x\n",r->revision,r->rsdt);else serial_write("acpi: RSDP not found; legacy platform mode\n");}
