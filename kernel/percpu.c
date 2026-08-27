#include "percpu.h"
static cpu_local_t boot_cpu;
void percpu_init(uint32_t id,uint32_t apic){boot_cpu.id=id;boot_cpu.apic_id=apic;boot_cpu.online=1;boot_cpu.irq_depth=0;boot_cpu.current=0;}
cpu_local_t*percpu_current(void){return &boot_cpu;}
void percpu_set_current(struct pcb*p){boot_cpu.current=p;}
void percpu_irq_enter(void){boot_cpu.irq_depth++;}
void percpu_irq_leave(void){if(boot_cpu.irq_depth)boot_cpu.irq_depth--;}
