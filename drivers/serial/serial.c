#include <stdint.h>
#include <stdarg.h>
#include "drivers.h"
static inline void outb(uint16_t p,uint8_t v){__asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));} static inline uint8_t inb(uint16_t p){uint8_t v;__asm__ volatile("inb %1,%0":"=a"(v):"Nd"(p));return v;}
void serial_init(void){outb(0x3f9,0);outb(0x3fb,0x80);outb(0x3f8,3);outb(0x3f9,0);outb(0x3fb,3);outb(0x3fa,0xc7);outb(0x3fc,0xb);}
void serial_write_n(const char*s,uint32_t n){for(uint32_t i=0;i<n;i++){while(!(inb(0x3fd)&0x20)){}outb(0x3f8,(uint8_t)s[i]);}}
void serial_write(const char*s){uint32_t n=0;while(s[n])n++;serial_write_n(s,n);}
int serial_read(void){return (inb(0x3fd)&1)?(int)inb(0x3f8):-1;}
