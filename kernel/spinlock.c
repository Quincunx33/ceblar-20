#include <stdint.h>
#include "spinlock.h"

uint32_t irq_save_disable(void){uint32_t flags;__asm__ volatile("pushf; pop %0; cli":"=r"(flags)::"memory");return flags;}
void irq_restore(uint32_t flags){__asm__ volatile("push %0; popf"::"r"(flags):"memory","cc");}
void spin_lock_irqsave(spinlock_t*lock,uint32_t*flags){uint32_t previous;*flags=irq_save_disable();for(;;){previous=0;__asm__ volatile("lock xchg %0,%1":"=r"(previous),"+m"(lock->value):"0"(1):"memory");if(previous==0)return;__asm__ volatile("pause");}}
void spin_unlock_irqrestore(spinlock_t*lock,uint32_t flags){__asm__ volatile("movl $0,%0":"=m"(lock->value)::"memory");irq_restore(flags);}
