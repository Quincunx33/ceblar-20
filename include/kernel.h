#ifndef CEBLAR_KERNEL_H
#define CEBLAR_KERNEL_H
#include "types.h"
void kmain(uint32_t magic, uint32_t mbi_addr);
void panic(const char *message);
void kputs(const char *s);
void kprintf(const char *fmt, ...);
#endif
