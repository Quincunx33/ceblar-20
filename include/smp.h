#ifndef CEBLAR_SMP_H
#define CEBLAR_SMP_H
#include <stdint.h>
void smp_init(void);
uint32_t smp_cpu_count(void);
int smp_apic_available(void);
#endif
