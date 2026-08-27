#include <stdint.h>
#include "scheduler.h"
#include "mm.h"
#include "drivers.h"
#define MAX_PROCESSES 32
#define USER_STACK_TOP 0x00bff000u
static pcb_t processes[MAX_PROCESSES];static unsigned int process_count;
static pcb_t*new_pcb(const char*name){if(process_count>=MAX_PROCESSES)return 0;pcb_t*p=&processes[process_count];p->pid=(pid_t)(process_count+1u);p->state=PROCESS_READY;p->ticks=0;p->name=name;p->next=0;p->entry_point=0;p->user_stack=0;p->user_mode=0;p->address_space=vmm_create_address_space();p->parent_pid=0;p->exit_code=0;process_count++;return p;}
pcb_t*process_create_entry(const char*name,process_entry_fn entry){pcb_t*p=new_pcb(name);if(!p)return 0;p->entry_point=(uint32_t)entry;p->cpu_state.eip=(uint32_t)entry;p->cpu_state.eflags=0x202u;if(entry){uint32_t top=(uint32_t)&p->kernel_stack[4096]-4;*(uint32_t*)top=(uint32_t)entry;p->cpu_state.esp=top;}return p;}
pcb_t*process_create(const char*name){return process_create_entry(name,0);}
pcb_t*process_create_user(const char*name,uint32_t entry){pcb_t*p=new_pcb(name);if(!p||!p->address_space)return 0;uint32_t page=pmm_alloc_page();if(!page)return 0;if(vmm_map_user(p->address_space,USER_STACK_TOP-PAGE_SIZE,page,PAGE_WRITE)!=0){pmm_free_page(page);return 0;}p->entry_point=entry;p->user_stack=USER_STACK_TOP;p->user_mode=1;p->cpu_state.eip=entry;p->cpu_state.esp=USER_STACK_TOP;p->cpu_state.eflags=0x202u;return p;}
void process_enter_user(pcb_t*p){if(!p||!p->user_mode||!p->address_space)return;vmm_switch_address_space(p->address_space);tss_set_kernel_stack((uint32_t)&p->kernel_stack[4096]);p->state=PROCESS_RUNNING;enter_user_mode(p->entry_point,p->user_stack);}
int process_exit(pid_t pid){for(unsigned int i=0;i<process_count;i++)if(processes[i].pid==pid){processes[i].state=PROCESS_ZOMBIE;processes[i].exit_code=0;return 0;}return -1;}
int process_wait(pid_t child,int32_t*status){for(unsigned int i=0;i<process_count;i++)if(processes[i].pid==child&&processes[i].state==PROCESS_ZOMBIE){if(status)*status=processes[i].exit_code;return(int)child;}return -1;}
int process_fork(void){pcb_t*parent=scheduler_current();if(!parent)parent=process_count?&processes[0]:process_create("init");pcb_t*child=new_pcb("fork-child");if(!child||!parent)return -1;child->cpu_state=parent->cpu_state;child->cpu_state.eax=0;child->entry_point=parent->entry_point;child->user_mode=parent->user_mode;child->user_stack=parent->user_stack;child->parent_pid=parent->pid;if(parent->address_space){address_space_t*clone=vmm_clone_address_space(parent->address_space);if(!clone){process_exit(child->pid);return -1;}vmm_destroy_address_space(child->address_space);child->address_space=clone;}return(int)child->pid;}
