#include <stdint.h>
#include "nic.h"
#include "pci.h"
#include "drivers.h"
static int present;
int nic_init(void){pci_device_t d;present=0;if(pci_find_class(0x02u,0x00u,&d)!=0){serial_write("nic: physical Ethernet controller absent\n");return-1;}present=1;serial_write("nic: Ethernet controller discovered; RX/TX rings pending\n");return 0;}
int nic_present(void){return present;}
