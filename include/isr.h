#ifndef CEBLAR_ISR_H
#define CEBLAR_ISR_H
#include <stdint.h>
typedef struct registers {uint32_t edi,esi,ebp,esp,ebx,edx,ecx,eax;uint32_t int_no,err_code,eip,cs,eflags,useresp,ss;} registers_t;
typedef void (*irq_handler_t)(registers_t *r);
void idt_init(void); void register_irq_handler(uint8_t irq,irq_handler_t handler); void pic_unmask(uint8_t irq); void interrupts_enable(void); void isr_handler(registers_t *r); void irq_handler(registers_t *r); void interrupt_dispatch(registers_t *r);
#endif
