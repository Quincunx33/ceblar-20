#include <stdint.h>
#include "apic.h"
#include "mm.h"
#include "smp.h"
#include "drivers.h"

#define LAPIC_BASE 0xfee00000u
#define LAPIC_ID 0x20u
#define LAPIC_SVR 0xf0u
#define LAPIC_EOI 0xb0u
#define LAPIC_ENABLE 0x100u
static volatile uint32_t*lapic=(volatile uint32_t*)LAPIC_BASE;static int enabled;
static inline uint32_t read_apic_base(void){uint32_t lo,hi;__asm__ volatile("rdmsr":"=a"(lo),"=d"(hi):"c"(0x1bu));(void)hi;return lo&0xfffff000u;}
static inline void write_lapic(uint32_t reg,uint32_t value){lapic[reg/4u]=value;}
void apic_init(void){uint32_t base;if(!smp_apic_available()){serial_write("apic: unavailable, PIC mode retained\n");return;}base=read_apic_base();if(base!=LAPIC_BASE){serial_write("apic: non-default base unsupported, PIC mode retained\n");return;}write_lapic(LAPIC_SVR,lapic[LAPIC_SVR/4u]|LAPIC_ENABLE);enabled=1;serial_write("apic: local APIC enabled at 0xfee00000\n");}
uint32_t apic_id(void){return enabled?lapic[LAPIC_ID/4u]>>24:0;}
void apic_eoi(void){if(enabled)write_lapic(LAPIC_EOI,0);}
