#include <stdint.h>
#include "nvme.h"
#include "pci.h"
#include "drivers.h"

static int present;
int nvme_init(void){pci_device_t d;present=0;if(pci_find_class(0x01u,0x08u,&d)!=0){serial_write("nvme: controller absent\n");return-1;}present=1;serial_write("nvme: controller discovered; admin queue setup pending\n");return 0;}
int nvme_present(void){return present;}
