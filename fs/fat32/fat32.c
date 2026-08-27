#include "vfs.h"
/* FAT32 block-device parsing is intentionally isolated here for a later disk driver. */
int fat32_mount(void *device){return device ? 0 : -1;}
