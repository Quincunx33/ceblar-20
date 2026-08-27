#include <stdint.h>
#include "smp.h"
#include "acpi.h"
#include "drivers.h"
#include "kernel.h"

static uint32_t cpu_count=1;static int apic;
static void cpuid(uint32_t leaf,uint32_t*a,uint32_t*b,uint32_t*c,uint32_t*d){__asm__ volatile("cpuid":"=a"(*a),"=b"(*b),"=c"(*c),"=d"(*d):"a"(leaf));}
void smp_init(void){uint32_t a,b,c,d;cpuid(0,&a,&b,&c,&d);if(a>=1){cpuid(1,&a,&b,&c,&d);apic=(d>>9)&1u;}cpu_count=acpi_cpu_count();if(cpu_count==0)cpu_count=1;kprintf("smp: BSP online, CPUID APIC %s; MADT CPUs=%u; secondary startup pending\n",apic?"available":"unavailable",cpu_count);}
uint32_t smp_cpu_count(void){return cpu_count;}
int smp_apic_available(void){return apic;}
