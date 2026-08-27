#include <stdint.h>
#include "drivers.h"
#include "scheduler.h"
static volatile uint32_t uptime_ticks; static uint32_t timer_hz=100;
static inline void outb(uint16_t p,uint8_t v){__asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));}
void pit_init(uint32_t hz){if(!hz)hz=100;timer_hz=hz;uint16_t d=(uint16_t)(1193182/hz);outb(0x43,0x36);outb(0x40,d&255);outb(0x40,d>>8);serial_write("timer: initialized successfully\n");}
void timer_irq(void*r){(void)r;uptime_ticks++;if(uptime_ticks==1)serial_write("timer: IRQ0 active\n");}
uint32_t timer_uptime_ms(void){return (uptime_ticks*1000u)/timer_hz;}
void ksleep_ms(uint32_t ms){uint32_t target=uptime_ticks+(ms*timer_hz+999u)/1000u;while(uptime_ticks<target)__asm__ volatile("hlt");}
