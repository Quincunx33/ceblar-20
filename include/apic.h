#ifndef CEBLAR_APIC_H
#define CEBLAR_APIC_H
#include <stdint.h>
void apic_init(void);
uint32_t apic_id(void);
void apic_eoi(void);
#endif
