#ifndef CEBLAR_PCI_H
#define CEBLAR_PCI_H
#include <stdint.h>

typedef struct pci_device { uint8_t bus; uint8_t slot; uint8_t function; uint16_t vendor; uint16_t device; uint8_t class_code; uint8_t subclass; uint8_t prog_if; uint8_t header_type; } pci_device_t;
void pci_init(void);
uint32_t pci_config_read32(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);
int pci_find_class(uint8_t class_code, uint8_t subclass, pci_device_t *out);

#endif
