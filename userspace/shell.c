#include <stdint.h>
#include "drivers.h"
#include "mm.h"
#include "kernel.h"
#include "scheduler.h"
static int same(const char*a,const char*b){uint32_t i=0;while(a[i]&&a[i]==b[i])i++;return a[i]==0&&b[i]==0;} static int prefix(const char*a,const char*b){uint32_t i=0;while(b[i]&&a[i]==b[i])i++;return b[i]==0;}
static void reboot(void){__asm__ volatile("outb %0,%1"::"a"((uint8_t)0xfe),"Nd"((uint16_t)0x64));}
void shell_run(void){char line[64];uint32_t n=0;vga_write("ceblar> ");for(;;){char c=keyboard_getc();if(!c){__asm__ volatile("hlt");continue;}if(c=='\n'){line[n]=0;if(same(line,"help"))kprintf("help uptime meminfo ps exec <path> clear reboot\n");else if(same(line,"uptime"))kprintf("uptime: %u ms\n",timer_uptime_ms());else if(same(line,"meminfo"))kprintf("memory: total=%u used=%u free=%u pages\n",pmm_total_pages(),pmm_used_pages(),pmm_free_pages());else if(same(line,"ps"))process_dump();else if(prefix(line,"exec ")){int rc=process_exec_path(line+5);if(rc)kprintf("exec failed: %d\n",rc);}else if(same(line,"clear"))vga_clear();else if(same(line,"reboot"))reboot();else if(n)kprintf("unknown command: %s\n",line);n=0;vga_write("ceblar> ");}else if(c=='\b'){if(n){n--;vga_putc('\b');}}else if(n<63){line[n++]=c;vga_putc(c);}}}
