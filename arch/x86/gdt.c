#include <stdint.h>
#include "drivers.h"
typedef struct {uint16_t limit;uint32_t base;} __attribute__((packed)) gdtr_t;
typedef struct tss {uint32_t prev,esp0,ss0,esp1,ss1,esp2,ss2,cr3,eip,eflags,eax,ecx,edx,ebx,esp,ebp,esi,edi,es,cs,ss,ds,fs,gs,ldt;uint16_t trap,io_base;} __attribute__((packed)) tss_t;
static uint64_t gdt[6];static tss_t kernel_tss;
static uint64_t make_tss_desc(uint32_t base,uint32_t limit){uint64_t d=0;d|=limit&0xffffu;d|=((uint64_t)(base&0xffffffu))<<16;d|=((uint64_t)0x89)<<40;d|=((uint64_t)((limit>>16)&0xfu))<<48;d|=((uint64_t)((base>>24)&0xffu))<<56;return d;}
void tss_set_kernel_stack(uint32_t esp){kernel_tss.esp0=esp;}
void tss_init(uint32_t top){uint32_t current;__asm__ volatile("mov %%esp,%0":"=r"(current));kernel_tss.esp0=top?top:current;kernel_tss.ss0=0x10;kernel_tss.io_base=sizeof(kernel_tss);gdt[5]=make_tss_desc((uint32_t)&kernel_tss,sizeof(kernel_tss)-1);__asm__ volatile("ltr %%ax"::"a"((uint16_t)0x28));serial_write("tss: initialized successfully\n");}
void gdt_init(void){gdt[0]=0;gdt[1]=0x00cf9a000000ffffULL;gdt[2]=0x00cf92000000ffffULL;gdt[3]=0x00cffa000000ffffULL;gdt[4]=0x00cff2000000ffffULL;gdt[5]=0;gdtr_t r={(uint16_t)(sizeof(gdt)-1),(uint32_t)gdt};__asm__ volatile("lgdt %0\n\tmov $0x10, %%ax\n\tmov %%ax, %%ds\n\tmov %%ax, %%es\n\tmov %%ax, %%ss\n\tljmp $0x08, $1f\n1:"::"m"(r):"eax");serial_write("gdt: initialized successfully\n");}
