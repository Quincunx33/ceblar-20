#include <stdint.h>
#include "kernel.h"
#include "drivers.h"
#include "isr.h"
#include "mm.h"
#include "syscall.h"
#include "scheduler.h"

typedef struct idt_entry { uint16_t base_low; uint16_t selector; uint8_t zero; uint8_t flags; uint16_t base_high; } __attribute__((packed)) idt_entry_t;
typedef struct idt_ptr { uint16_t limit; uint32_t base; } __attribute__((packed)) idt_ptr_t;
static idt_entry_t idt[256]; static idt_ptr_t idtp; static irq_handler_t irq_handlers[16];

#define DECL_ISR(n) extern void isr##n(void)
#define DECL_IRQ(n) extern void irq##n(void)
DECL_ISR(0);DECL_ISR(1);DECL_ISR(2);DECL_ISR(3);DECL_ISR(4);DECL_ISR(5);DECL_ISR(6);DECL_ISR(7);DECL_ISR(8);DECL_ISR(9);DECL_ISR(10);DECL_ISR(11);DECL_ISR(12);DECL_ISR(13);DECL_ISR(14);DECL_ISR(15);DECL_ISR(16);DECL_ISR(17);DECL_ISR(18);DECL_ISR(19);DECL_ISR(20);DECL_ISR(21);DECL_ISR(22);DECL_ISR(23);DECL_ISR(24);DECL_ISR(25);DECL_ISR(26);DECL_ISR(27);DECL_ISR(28);DECL_ISR(29);DECL_ISR(30);DECL_ISR(31);
DECL_IRQ(0);DECL_IRQ(1);DECL_IRQ(2);DECL_IRQ(3);DECL_IRQ(4);DECL_IRQ(5);DECL_IRQ(6);DECL_IRQ(7);DECL_IRQ(8);DECL_IRQ(9);DECL_IRQ(10);DECL_IRQ(11);DECL_IRQ(12);DECL_IRQ(13);DECL_IRQ(14);DECL_IRQ(15); extern void isr128(void);
static void outb(uint16_t p,uint8_t v){__asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));}
static void lidt(idt_ptr_t *p){__asm__ volatile("lidtl (%0)"::"r"(p));}
static void set_gate(uint8_t n,void(*base)(void),uint8_t flags){uint32_t a=(uint32_t)base;idt[n].base_low=(uint16_t)a;idt[n].selector=0x08;idt[n].zero=0;idt[n].flags=flags;idt[n].base_high=(uint16_t)(a>>16);}
static const char *exception_names[33]={"divide error","debug","NMI","breakpoint","overflow","bound range","invalid opcode","device unavailable","double fault","coprocessor segment","invalid TSS","segment not present","stack fault","general protection","page fault","reserved","x87 fault","alignment check","machine check","SIMD fault","virtualization","control protection","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved"};
void pic_remap(void){outb(0x20,0x11);outb(0xa0,0x11);outb(0x21,0x20);outb(0xa1,0x28);outb(0x21,4);outb(0xa1,2);outb(0x21,1);outb(0xa1,1);outb(0x21,0xff);outb(0xa1,0xff);}
void idt_init(void){idt_entry_t z={0};for(unsigned int i=0;i<256;i++)idt[i]=z;void(*isrs[32])(void)={isr0,isr1,isr2,isr3,isr4,isr5,isr6,isr7,isr8,isr9,isr10,isr11,isr12,isr13,isr14,isr15,isr16,isr17,isr18,isr19,isr20,isr21,isr22,isr23,isr24,isr25,isr26,isr27,isr28,isr29,isr30,isr31};void(*irqs[16])(void)={irq0,irq1,irq2,irq3,irq4,irq5,irq6,irq7,irq8,irq9,irq10,irq11,irq12,irq13,irq14,irq15};for(int i=0;i<32;i++)set_gate(i,isrs[i],0x8e);for(int i=0;i<16;i++)set_gate(32+i,irqs[i],0x8e);set_gate(128,isr128,0xee);pic_remap();idtp.limit=sizeof(idt)-1;idtp.base=(uint32_t)idt;lidt(&idtp);__asm__ volatile("cli");serial_write("idt: 256 gates, PIC remapped, IRQ lines masked\n");}
void register_irq_handler(uint8_t irq,irq_handler_t h){if(irq<16)irq_handlers[irq]=h;}
void pic_unmask(uint8_t irq){if(irq<8){uint8_t m;__asm__ volatile("inb %1,%0":"=a"(m):"Nd"((uint16_t)0x21));m&=(uint8_t)~(1u<<irq);outb(0x21,m);}else{uint8_t m;__asm__ volatile("inb %1,%0":"=a"(m):"Nd"((uint16_t)0xa1));m&=(uint8_t)~(1u<<(irq-8));outb(0xa1,m);pic_unmask(2);}}
void interrupts_enable(void){__asm__ volatile("sti");}
void isr_handler(registers_t*r){if(r->int_no<32){serial_write("exception: "); serial_write(exception_names[r->int_no]); kprintf(" vector=%u eip=0x%x cs=0x%x err=0x%x\n",r->int_no,r->eip,r->cs,r->err_code); if(r->int_no==14){vmm_page_fault(r->err_code,0);return;} panic("CPU exception");}}
void irq_handler(registers_t*r){uint8_t irq=(uint8_t)(r->int_no-32);if(irq<16&&irq_handlers[irq])irq_handlers[irq](r);if(irq>=8)outb(0xa0,0x20);outb(0x20,0x20);}
registers_t*interrupt_dispatch(registers_t*r){if(r->int_no<32)isr_handler(r);else if(r->int_no==32){irq_handler(r);return scheduler_timer_tick(r);}else if(r->int_no<48)irq_handler(r);else if(r->int_no==128)return syscall_from_interrupt(r);return r;}
