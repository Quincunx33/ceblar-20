#include <stdint.h>
#include "drivers.h"
static volatile uint16_t *const video=(uint16_t*)0xb8000; static uint8_t row,col,color=0x07;
static inline void outb(uint16_t p,uint8_t v){__asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));}
void vga_set_color(uint8_t fg,uint8_t bg){color=(uint8_t)((bg<<4)|(fg&15));}
void vga_set_cursor(uint8_t r,uint8_t c){uint16_t pos=(uint16_t)r*80u+c;outb(0x3d4,0x0f);outb(0x3d5,(uint8_t)pos);outb(0x3d4,0x0e);outb(0x3d5,(uint8_t)(pos>>8));}
void vga_scroll(void){for(uint32_t r=1;r<25;r++)for(uint32_t c=0;c<80;c++)video[(r-1)*80+c]=video[r*80+c];for(uint32_t c=0;c<80;c++)video[24*80+c]=((uint16_t)color<<8)|' ';row=24;}
void vga_clear(void){for(uint32_t i=0;i<80*25;i++)video[i]=((uint16_t)color<<8)|' ';row=col=0;vga_set_cursor(row,col);}
void vga_init(void){vga_clear();}
void vga_putc(char c){if(c=='\n'){col=0;row++;}else if(c=='\r'){col=0;}else if(c=='\t'){col=(uint8_t)((col+8)&~7u);}else{video[row*80+col]=((uint16_t)color<<8)|(uint8_t)c;if(++col>=80){col=0;row++;}}if(row>=25)vga_scroll();vga_set_cursor(row,col);}
void vga_write(const char*s){while(*s)vga_putc(*s++);}
