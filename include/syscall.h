#ifndef CEBLAR_SYSCALL_H
#define CEBLAR_SYSCALL_H
#include "types.h"
typedef struct registers registers_t;
enum {SYS_READ=0,SYS_WRITE=1,SYS_OPEN=2,SYS_CLOSE=3,SYS_FORK=4,SYS_EXIT=5,SYS_GETPID=6,SYS_YIELD=7,SYS_SLEEP=8,SYS_WAIT=9,SYS_EXEC=10,SYS_MMAP=11,SYS_UNAME=12,SYS_COUNT=13};
void syscall_init(void); int32_t syscall_dispatch(uint32_t n,uint32_t a,uint32_t b,uint32_t c); void syscall_from_interrupt(registers_t*r);
#endif
