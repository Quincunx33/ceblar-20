#include <stdint.h>
#include "syscall.h"
#include "userlib.h"

static int32_t call3(uint32_t number,uint32_t a,uint32_t b,uint32_t c){int32_t result;__asm__ volatile("int $0x80":"=a"(result):"a"(number),"b"(a),"c"(b),"d"(c):"memory");return result;}
int32_t u_read(int fd,void*buffer,uint32_t length){return call3(SYS_READ,(uint32_t)fd,(uint32_t)buffer,length);}
int32_t u_write(int fd,const void*buffer,uint32_t length){return call3(SYS_WRITE,(uint32_t)fd,(uint32_t)buffer,length);}
int32_t u_open(const char*path,uint32_t flags){return call3(SYS_OPEN,(uint32_t)path,flags,0);}
int32_t u_close(int fd){return call3(SYS_CLOSE,(uint32_t)fd,0,0);}
int32_t u_fork(void){return call3(SYS_FORK,0,0,0);}
void u_exit(int32_t status){(void)call3(SYS_EXIT,(uint32_t)status,0,0);for(;;)__asm__ volatile("hlt");}
int32_t u_getpid(void){return call3(SYS_GETPID,0,0,0);}
int32_t u_yield(void){return call3(SYS_YIELD,0,0,0);}
int32_t u_sleep(uint32_t milliseconds){return call3(SYS_SLEEP,milliseconds,0,0);}
int32_t u_wait(int32_t pid,int32_t*status){return call3(SYS_WAIT,(uint32_t)pid,(uint32_t)status,0);}
int32_t u_exec(const char*path){return call3(SYS_EXEC,(uint32_t)path,0,0);}
void*u_mmap(void*address,uint32_t length,uint32_t flags){return(void*)call3(SYS_MMAP,(uint32_t)address,length,flags);}
int32_t u_uname(char*buffer){return call3(SYS_UNAME,(uint32_t)buffer,0,0);}
int32_t u_munmap(void*address,uint32_t length){return call3(SYS_MUNMAP,(uint32_t)address,length,0);}
