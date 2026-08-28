#include <stdint.h>
#include "ioapic.h"
#include "smp.h"
#include "acpi.h"
#include "drivers.h"
#include "kernel.h"

#define IOAPIC_DEFAULT_BASE 0xfec00000u
#define IOAPIC_MASK 0x00010000u
#define IOAPIC_TRIGGER_LEVEL 0x00008000u
#define IOAPIC_POLARITY_LOW 0x00002000u
static volatile uint32_t*ioapic=(volatile uint32_t*)IOAPIC_DEFAULT_BASE;static int available;static uint32_t gsi_base,max_redirection;
static uint32_t raw_read(uint8_t reg){ioapic[0]=reg;return ioapic[4];}uint32_t ioapic_read(uint8_t reg){if(!available)return 0xffffffffu;return raw_read(reg);}
void ioapic_write(uint8_t reg,uint32_t value){if(!available)return;ioapic[0]=reg;ioapic[4]=value;}
void ioapic_init(void){uint32_t version,base;if(!smp_apic_available()){serial_write("ioapic: APIC unavailable; legacy PIC retained\n");return;}base=acpi_ioapic_base();if(base==0||base!=IOAPIC_DEFAULT_BASE){serial_write("ioapic: unsupported controller base; legacy PIC retained\n");return;}ioapic=(volatile uint32_t*)(uintptr_t)base;version=raw_read(1);if(version==0xffffffffu||version==0){serial_write("ioapic: controller not responding; legacy PIC retained\n");return;}available=1;gsi_base=acpi_ioapic_gsi_base();max_redirection=(version>>16)&0xffu;kprintf("ioapic: controller ready version=0x%x max-redirection=%u gsi-base=%u\n",version&0xffu,max_redirection,gsi_base);ioapic_route_irq(0,32,0,1);ioapic_route_irq(1,33,0,1);serial_write("ioapic: IRQ0/IRQ1 routes prepared and masked\n");}
int ioapic_available(void){return available;}void ioapic_route_irq(uint8_t irq,uint8_t vector,uint8_t destination,uint8_t masked){uint32_t gsi;uint16_t flags;uint32_t pin;uint8_t low,high;uint32_t value;if(!available||acpi_irq_to_gsi(irq,&gsi,&flags)<0||gsi<gsi_base)return;pin=gsi-gsi_base;if(pin>max_redirection)return;low=(uint8_t)(0x10u+pin*2u);high=(uint8_t)(low+1u);value=(masked?IOAPIC_MASK:0u)|vector;if((flags&3u)==3u)value|=IOAPIC_POLARITY_LOW;if((flags&12u)==12u)value|=IOAPIC_TRIGGER_LEVEL;ioapic_write(high,(uint32_t)destination<<24);ioapic_write(low,value);}
void ioapic_enable_irq(uint8_t irq){ioapic_route_irq(irq,(uint8_t)(32u+irq),0,0);}void ioapic_disable_irq(uint8_t irq){ioapic_route_irq(irq,(uint8_t)(32u+irq),0,1);}
