#include <stdint.h>
#include "syscall.h"
#include "vfs.h"
#include "scheduler.h"
#include "isr.h"
#include "mm.h"
#include "drivers.h"
#include "kernel.h"
static int user_range(uint32_t p,uint32_t n){return p>=0x400000u&&p<0xc0000000u&&n<=0xc0000000u-p;}
static int32_t rd(uint32_t a,uint32_t b,uint32_t c){return user_range(b,c)?vfs_read((int)a,(void*)b,c):-1;} static int32_t wr(uint32_t a,uint32_t b,uint32_t c){return user_range(b,c)?vfs_write((int)a,(const void*)b,c):-1;} static int32_t op(uint32_t a,uint32_t b,uint32_t c){(void)c;return user_range(a,1)?vfs_open((const char*)a,b):-1;} static int32_t cl(uint32_t a,uint32_t b,uint32_t c){(void)b;(void)c;return vfs_close((int)a);} static int32_t fk(uint32_t a,uint32_t b,uint32_t c){(void)a;(void)b;(void)c;return process_fork();} static int32_t ex(uint32_t a,uint32_t b,uint32_t c){(void)a;(void)b;(void)c;pcb_t*p=scheduler_current();return p?process_exit(p->pid):-1;} static int32_t gp(uint32_t a,uint32_t b,uint32_t c){(void)a;(void)b;(void)c;pcb_t*p=scheduler_current();return p?(int32_t)p->pid:-1;} static int32_t yl(uint32_t a,uint32_t b,uint32_t c){(void)a;(void)b;(void)c;scheduler_yield();return 0;} static int32_t sl(uint32_t a,uint32_t b,uint32_t c){(void)b;(void)c;ksleep_ms(a);return 0;} static int32_t wt(uint32_t a,uint32_t b,uint32_t c){(void)c;int32_t status;int r=process_wait((pid_t)a,user_range(b,sizeof(status))?&status:0);if(r>=0&&user_range(b,sizeof(status))){*(int32_t*)b=status;}return r;} static int32_t ee(uint32_t a,uint32_t b,uint32_t c){(void)a;(void)b;(void)c;kprintf("exec: ELF image handoff required\n");return -1;} static int32_t mm(uint32_t a,uint32_t b,uint32_t c){(void)b;if((a&0xfffu)||a<0x400000u||a>=0xc0000000u)return -1;uint32_t p=pmm_alloc_page();if(!p)return -1;vmm_map(a,p,c|PAGE_WRITE|PAGE_USER);return(int32_t)a;} static int32_t un(uint32_t a,uint32_t b,uint32_t c){(void)b;(void)c;const char*s="ceblar-20 0.4.0 x86";uint32_t n=0;while(s[n])n++;if(!user_range(a,n+1))return -1;char*d=(char*)a;for(uint32_t i=0;i<=n;i++)d[i]=s[i];return 0;}
static int32_t(*table[SYS_COUNT])(uint32_t,uint32_t,uint32_t);
void syscall_init(void){table[0]=rd;table[1]=wr;table[2]=op;table[3]=cl;table[4]=fk;table[5]=ex;table[6]=gp;table[7]=yl;table[8]=sl;table[9]=wt;table[10]=ee;table[11]=mm;table[12]=un;serial_write("syscall: initialized successfully\n");}
int32_t syscall_dispatch(uint32_t n,uint32_t a,uint32_t b,uint32_t c){return n<SYS_COUNT&&table[n]?table[n](a,b,c):-1;} void syscall_from_interrupt(registers_t*r){if(r)r->eax=(uint32_t)syscall_dispatch(r->eax,r->ebx,r->ecx,r->edx);}
