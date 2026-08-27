#ifndef CEBLAR_PERCPU_H
#define CEBLAR_PERCPU_H
#include <stdint.h>
struct pcb;
typedef struct cpu_local {uint32_t id;uint32_t apic_id;uint32_t online;uint32_t irq_depth;struct pcb*current;} cpu_local_t;
void percpu_init(uint32_t id,uint32_t apic);
cpu_local_t*percpu_current(void);
void percpu_set_current(struct pcb*p);
void percpu_irq_enter(void);
void percpu_irq_leave(void);
#endif
