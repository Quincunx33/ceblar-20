#ifndef CEBLAR_SCHEDULER_H
#define CEBLAR_SCHEDULER_H
#include "types.h"
struct address_space;
enum {PROCESS_READY=0,PROCESS_RUNNING=1,PROCESS_BLOCKED=2,PROCESS_ZOMBIE=3};
typedef struct cpu_state {uint32_t eax,ebx,ecx,edx,esi,edi,ebp,esp,eip,eflags;} cpu_state_t;
typedef struct pcb {pid_t pid;uint32_t state;uint32_t ticks;const char*name;cpu_state_t cpu_state;uint8_t kernel_stack[4096];struct pcb*next;uint32_t entry_point;uint32_t user_stack;uint8_t user_mode;struct address_space*address_space;pid_t parent_pid;int32_t exit_code;} pcb_t;
typedef struct scheduler {const char*name;int(*init)(void);void(*tick)(void);pcb_t*(*current)(void);} scheduler_t;
typedef void(*process_entry_fn)(void);
pcb_t*process_create(const char*name);pcb_t*process_create_entry(const char*name,process_entry_fn entry);pcb_t*process_create_user(const char*name,uint32_t entry);void process_enter_user(pcb_t*p);int process_exit(pid_t pid);int process_wait(pid_t child,int32_t*status);void context_switch(pcb_t*from,pcb_t*to);int process_fork(void);void scheduler_yield(void);
int scheduler_register(scheduler_t*s);void scheduler_tick(void);pcb_t*scheduler_current(void);
#endif
