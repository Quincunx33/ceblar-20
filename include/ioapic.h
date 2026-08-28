#ifndef CEBLAR_IOAPIC_H
#define CEBLAR_IOAPIC_H
#include <stdint.h>
void ioapic_init(void);
uint32_t ioapic_read(uint8_t reg);
void ioapic_write(uint8_t reg,uint32_t value);
int ioapic_available(void);void ioapic_route_irq(uint8_t irq,uint8_t vector,uint8_t destination,uint8_t masked);void ioapic_enable_irq(uint8_t irq);void ioapic_disable_irq(uint8_t irq);
#endif
