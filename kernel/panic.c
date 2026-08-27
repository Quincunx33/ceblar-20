#include "kernel.h"
#include "drivers.h"
void panic(const char *message) { vga_write("KERNEL PANIC: "); vga_write(message); vga_write("\n"); serial_write("KERNEL PANIC: "); serial_write(message); for (;;) __asm__ volatile("cli; hlt"); }
void kputs(const char *s) { vga_write(s); serial_write(s); }
