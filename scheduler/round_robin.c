#include "scheduler.h"
#include "mm.h"
#include "drivers.h"
static scheduler_t rr;static pcb_t idle={.pid=1,.state=PROCESS_RUNNING,.name="idle"};static pcb_t*current=&idle;static pcb_t*ready_head;static uint32_t quantum_ticks=1;
static void enqueue(pcb_t*p){if(!p||p->state==PROCESS_ZOMBIE||!p->cpu_state.esp)return;p->next=0;if(!ready_head){ready_head=p;return;}pcb_t*q=ready_head;while(q->next)q=q->next;q->next=p;}
static pcb_t*next_ready(void){pcb_t*p=ready_head;if(!p)return &idle;ready_head=p->next;p->next=0;return p;}
static int rr_init(void){current=&idle;ready_head=0;return 0;}
static void rr_tick(void){if(!current)return;if(++current->ticks<quantum_ticks)return;current->ticks=0;if(current!=&idle&&current->state==PROCESS_RUNNING){current->state=PROCESS_READY;enqueue(current);}pcb_t*n=next_ready();if(n==current)return;n->state=PROCESS_RUNNING;if(n->address_space)vmm_switch_address_space(n->address_space);else vmm_switch_address_space(0);tss_set_kernel_stack((uint32_t)&n->kernel_stack[4096]);context_switch(current,n);current=n;}
static pcb_t*rr_current(void){return current;}
int scheduler_register(scheduler_t*s){if(!s)return -1;rr=*s;return rr.init?rr.init():0;}void scheduler_tick(void){if(rr.tick)rr.tick();}pcb_t*scheduler_current(void){return rr.current?rr.current():current;}void scheduler_yield(void){if(current)current->ticks=quantum_ticks;scheduler_tick();}
__attribute__((constructor))static void scheduler_bootstrap(void){rr.name="round-robin";rr.init=rr_init;rr.tick=rr_tick;rr.current=rr_current;}
