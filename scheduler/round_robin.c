#include "scheduler.h"
#include "isr.h"
#include "mm.h"
#include "drivers.h"
#include "kernel.h"
#include "vfs.h"
#include "percpu.h"
static int rr_init(void);static void rr_tick(void);static pcb_t*rr_current(void);
static scheduler_t rr={.name="round-robin",.init=rr_init,.tick=rr_tick,.current=rr_current};static pcb_t idle={.pid=0,.state=PROCESS_RUNNING,.name="idle"};static pcb_t*current=&idle;static pcb_t*ready_head;static uint32_t quantum_ticks=1;
static void enqueue(pcb_t*p){if(!p||!p->allocated||p->state==PROCESS_ZOMBIE||!p->cpu_state.esp)return;p->next=0;if(!ready_head){ready_head=p;return;}pcb_t*q=ready_head;while(q->next)q=q->next;q->next=p;}
static pcb_t*next_ready(void){pcb_t*p;while((p=ready_head)!=0){ready_head=p->next;p->next=0;if(p->allocated&&p->state==PROCESS_READY)return p;}return &idle;}
static void activate(pcb_t*p){p->state=PROCESS_RUNNING;if(p->address_space)vmm_switch_address_space(p->address_space);else vmm_switch_address_space(0);tss_set_kernel_stack((uint32_t)&p->kernel_stack[4096]);current=p;percpu_set_current(p);vfs_set_process(p->pid);}
static int rr_init(void){current=&idle;ready_head=0;return 0;}
static void rr_tick(void){if(!current)return;if(++current->ticks<quantum_ticks)return;current->ticks=0;pcb_t*previous=current;if(previous!=&idle&&previous->state==PROCESS_RUNNING){previous->state=PROCESS_READY;enqueue(previous);}pcb_t*n=next_ready();if(n==previous)return;activate(n);context_switch(previous,n);}
static pcb_t*rr_current(void){return current;}
int scheduler_register(scheduler_t*s){if(!s)return -1;rr=*s;return rr.init?rr.init():0;}int scheduler_add(pcb_t*p){if(!p||!p->allocated||!p->cpu_state.esp)return-1;enqueue(p);return 0;}void scheduler_tick(void){if(rr.tick)rr.tick();}
struct registers*scheduler_timer_tick(struct registers*r){process_poll_timeouts();if(!current)return r;current->interrupt_frame=r;if(++current->ticks<quantum_ticks)return r;current->ticks=0;pcb_t*previous=current;if(previous!=&idle&&previous->state==PROCESS_RUNNING){previous->state=PROCESS_READY;enqueue(previous);}pcb_t*n=next_ready();if(n==previous)return r;if(n==&idle){if(previous!=&idle&&previous->state==PROCESS_READY)previous->state=PROCESS_RUNNING;current=(previous->state==PROCESS_RUNNING)?previous:&idle;return r;}activate(n);return n->interrupt_frame?n->interrupt_frame:process_initial_frame(n);} static struct registers*switch_from_current(struct registers*r){pcb_t*previous=current;if(!previous)return r;previous->interrupt_frame=r;if(previous->state==PROCESS_RUNNING)previous->state=PROCESS_BLOCKED;pcb_t*n=next_ready();if(n==&idle){if(previous!=&idle&&previous->state!=PROCESS_BLOCKED&&previous->state!=PROCESS_ZOMBIE)previous->state=PROCESS_RUNNING;current=(previous->state==PROCESS_RUNNING)?previous:&idle;return r;}activate(n);return n->interrupt_frame?n->interrupt_frame:process_initial_frame(n);} struct registers*scheduler_block_current(struct registers*r){return switch_from_current(r);}
void scheduler_set_current(pcb_t*p){if(p){current=p;percpu_set_current(p);p->state=PROCESS_RUNNING;p->interrupt_frame=0;vfs_set_process(p->pid);}}
pcb_t*scheduler_current(void){return rr.current?rr.current():current;}void scheduler_yield(void){if(current)current->ticks=quantum_ticks;scheduler_tick();}
