#ifndef CEBLAR_ACPI_H
#define CEBLAR_ACPI_H
#include <stdint.h>
#define ACPI_MAX_CPUS 16u
#define ACPI_MAX_IRQ_OVERRIDES 16u
void acpi_init(void);
int acpi_madt_present(void);
uint32_t acpi_lapic_base(void);
uint32_t acpi_ioapic_base(void);
uint32_t acpi_ioapic_gsi_base(void);
uint32_t acpi_cpu_count(void);
uint8_t acpi_cpu_apic_id(uint32_t index);
int acpi_irq_to_gsi(uint8_t irq,uint32_t *gsi,uint16_t *flags);
#endif
