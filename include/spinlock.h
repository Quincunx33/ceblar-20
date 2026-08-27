#ifndef CEBLAR_SPINLOCK_H
#define CEBLAR_SPINLOCK_H
#include <stdint.h>

typedef struct { volatile uint32_t value; } spinlock_t;
uint32_t irq_save_disable(void);
void irq_restore(uint32_t flags);
void spin_lock_irqsave(spinlock_t *lock, uint32_t *flags);
void spin_unlock_irqrestore(spinlock_t *lock, uint32_t flags);

#endif
