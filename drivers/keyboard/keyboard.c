#include <stdint.h>
#include "drivers.h"
#define KEY_BUF_SIZE 64
static char key_buf[KEY_BUF_SIZE]; static uint8_t key_head,key_tail; static const char map[128]={0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v','b','n','m',',','.','/',0,'*',0,' ','_'};
static inline uint8_t inb(uint16_t p){uint8_t v;__asm__ volatile("inb %1,%0":"=a"(v):"Nd"(p));return v;}
void keyboard_init(void){(void)inb(0x60);serial_write("keyboard: initialized successfully\n");}
void keyboard_irq(void*r){(void)r;uint8_t sc=inb(0x60);if(sc&0x80)return;if(sc<128&&map[sc]){uint8_t n=(uint8_t)((key_tail+1)%KEY_BUF_SIZE);if(n!=key_head){key_buf[key_tail]=map[sc];key_tail=n;}}}
char keyboard_getc(void){if(key_head==key_tail)return 0;char c=key_buf[key_head];key_head=(uint8_t)((key_head+1)%KEY_BUF_SIZE);return c;}
