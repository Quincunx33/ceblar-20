#include <stdint.h>
#include "ioapic.h"
#include "smp.h"
#include "drivers.h"
#include "kernel.h"

#define IOAPIC_BASE 0xfec00000u
static volatile uint32_t*ioapic=(volatile uint32_t*)IOAPIC_BASE;static int available;
static uint32_t raw_read(uint8_t reg){ioapic[0]=reg;return ioapic[4];} uint32_t ioapic_read(uint8_t reg){if(!available)return 0xffffffffu;return raw_read(reg);}
void ioapic_write(uint8_t reg,uint32_t value){if(!available)return;ioapic[0]=reg;ioapic[4]=value;}
void ioapic_init(void){uint32_t version;if(!smp_apic_available()){serial_write("ioapic: APIC unavailable; legacy PIC retained\n");return;}version=raw_read(1);if(version==0xffffffffu||version==0){serial_write("ioapic: controller not responding; legacy PIC retained\n");return;}available=1;version=ioapic_read(1);kprintf("ioapic: controller ready version=0x%x max-redirection=%u\n",version&0xffu,(version>>16)&0xffu);ioapic_route_irq(0,32,0,1);serial_write("ioapic: IRQ0 redirection prepared and masked\n");}
int ioapic_available(void){return available;} void ioapic_route_irq(uint8_t irq,uint8_t vector,uint8_t destination,uint8_t masked){uint8_t low=(uint8_t)(0x10u+irq*2u);uint8_t high=(uint8_t)(low+1u);if(!available||irq>23u)return;ioapic_write(high,(uint32_t)destination<<24);ioapic_write(low,(masked?0x00010000u:0u)|vector);}
