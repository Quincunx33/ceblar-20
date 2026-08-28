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
int32_t u_munmap(void*address,uint32_t length){return call3(SYS_MUNMAP,(uint32_t)address,length,0);} int32_t u_getcred(credentials_t*credentials){return call3(SYS_GETCRED,(uint32_t)credentials,0,0);} int32_t u_socket(uint16_t local_port){return call3(SYS_SOCKET,local_port,0,0);} int32_t u_connect(int socket,uint16_t remote_port){return call3(SYS_CONNECT,(uint32_t)socket,remote_port,0);} int32_t u_send(int socket,const void*data,uint32_t length){return call3(SYS_SEND,(uint32_t)socket,(uint32_t)data,length);} int32_t u_socket_close(int socket){return call3(SYS_CLOSE_SOCKET,(uint32_t)socket,0,0);}
int32_t u_kill(int32_t pid,uint32_t signal){return call3(SYS_KILL,(uint32_t)pid,signal,0);}int32_t u_sigaction(uint32_t signal,uint32_t handler,uint32_t mask){return call3(SYS_SIGACTION,signal,handler,mask);}int32_t u_sigreturn(void){return call3(SYS_SIGRETURN,0,0,0);}
int32_t u_futex_wait(uint32_t*word,uint32_t expected){return call3(SYS_FUTEX_WAIT,(uint32_t)word,expected,0);}int32_t u_futex_wait_timeout(uint32_t*word,uint32_t expected,uint32_t timeout_ms){return call3(SYS_FUTEX_WAIT,(uint32_t)word,expected,timeout_ms);}
int32_t u_futex_wake(uint32_t*word,uint32_t count){return call3(SYS_FUTEX_WAKE,(uint32_t)word,count,0);}
