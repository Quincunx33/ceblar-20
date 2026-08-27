#include <stdint.h>
#include "mm.h"
#include "spinlock.h"
#define HEAP_BYTES (1024u*1024u)
#define HEAP_MAGIC 0xc3b1a9e7u
#define HEAP_VIRT_START 0xd0000000u
typedef struct block {uint32_t size;uint32_t free;uint32_t magic;struct block*next;} block_t;
static uint8_t heap_area[HEAP_BYTES] __attribute__((aligned(16)));static block_t*head;static uint32_t heap_virt_top=HEAP_VIRT_START;static spinlock_t heap_lock;
static void split(block_t*b,uint32_t n){if(b->size>=n+sizeof(block_t)+8){block_t*r=(block_t*)((uint8_t*)b+sizeof(block_t)+n);r->size=b->size-n-sizeof(block_t);r->free=1;r->magic=HEAP_MAGIC;r->next=b->next;b->next=r;b->size=n;}}
static void merge(void){for(block_t*b=head;b&&b->next;){if(b->free&&b->next->free){b->size+=sizeof(block_t)+b->next->size;b->next=b->next->next;}else b=b->next;}}
static int grow(void){uint32_t page=pmm_alloc_page();if(!page)return -1;vmm_map(heap_virt_top,page,2);block_t*n=(block_t*)heap_virt_top;n->size=PAGE_SIZE-sizeof(block_t);n->free=1;n->magic=HEAP_MAGIC;n->next=0;heap_virt_top+=PAGE_SIZE;if(!head)head=n;else{block_t*t=head;while(t->next)t=t->next;t->next=n;}return 0;}
void heap_init(void){head=(block_t*)heap_area;head->size=HEAP_BYTES-sizeof(block_t);head->free=1;head->magic=HEAP_MAGIC;head->next=0;}
void*kmalloc(usize_t n){uint32_t flags;void*result=0;if(!head)heap_init();if(!n)return 0;n=(n+7u)&~7u;spin_lock_irqsave(&heap_lock,&flags);for(;;){for(block_t*b=head;b;b=b->next)if(b->free&&b->size>=n){split(b,n);b->free=0;result=(uint8_t*)b+sizeof(block_t);goto done;}if(grow()!=0)goto done;}done:spin_unlock_irqrestore(&heap_lock,flags);return result;}
void kfree(void*p){uint32_t flags;block_t*b;if(!p)return;b=(block_t*)((uint8_t*)p-sizeof(block_t));spin_lock_irqsave(&heap_lock,&flags);if(b->magic==HEAP_MAGIC&&!b->free){b->free=1;merge();}spin_unlock_irqrestore(&heap_lock,flags);}
void*krealloc(void*p,usize_t n){if(!p)return kmalloc(n);if(!n){kfree(p);return 0;}block_t*b=(block_t*)((uint8_t*)p-sizeof(block_t));if(b->magic!=HEAP_MAGIC||b->free)return 0;if(b->size>=n){split(b,n);return p;}void*q=kmalloc(n);if(!q)return 0;uint8_t*a=p,*c=q;for(uint32_t i=0;i<b->size;i++)c[i]=a[i];kfree(p);return q;}
