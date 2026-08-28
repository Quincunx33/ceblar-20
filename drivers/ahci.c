#include <stdint.h>
#include "ahci.h"
#include "pci.h"
#include "drivers.h"

static int present;
int ahci_init(void){pci_device_t d;present=0;if(pci_find_class(0x01u,0x06u,&d)!=0){serial_write("ahci: controller absent\n");return-1;}present=1;serial_write("ahci: controller discovered; DMA command engine pending\n");return 0;}
int ahci_present(void){return present;}
