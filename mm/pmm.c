#include <stdint.h>
#include "mm.h"
#include "drivers.h"
#include "spinlock.h"
#define MAX_PHYS_PAGES 65536u
#define BITMAP_BYTES (MAX_PHYS_PAGES / 8u)
#define TAG_MMAP 6u
#define MMAP_AVAILABLE 1u
static uint8_t page_bitmap[BITMAP_BYTES]; static uint16_t page_refs[MAX_PHYS_PAGES]; static uint32_t total_pages_count, used_pages_count; static spinlock_t pmm_lock;
extern uint8_t __kernel_start, __kernel_end;
static void bit_set(uint32_t p){page_bitmap[p>>3]|=(uint8_t)(1u<<(p&7));} static void bit_clear(uint32_t p){page_bitmap[p>>3]&=(uint8_t)~(1u<<(p&7));} static int bit_test(uint32_t p){return (page_bitmap[p>>3]>>(p&7))&1u;}
static void serial_number(uint32_t n){char b[11];int i=10;b[i]=0;do{b[--i]=(char)('0'+n%10);n/=10;}while(n);serial_write(&b[i]);}
static void reserve_range(uint32_t begin,uint32_t end){begin/=PAGE_SIZE;end=(end+PAGE_SIZE-1)/PAGE_SIZE;if(end>MAX_PHYS_PAGES)end=MAX_PHYS_PAGES;for(uint32_t p=begin;p<end;p++)if(!bit_test(p)){bit_set(p);used_pages_count++;}}
void pmm_init(uint32_t mbi){for(uint32_t i=0;i<BITMAP_BYTES;i++)page_bitmap[i]=0xff;for(uint32_t i=0;i<MAX_PHYS_PAGES;i++)page_refs[i]=0;total_pages_count=0;used_pages_count=0;uint8_t *base=(uint8_t*)mbi;uint32_t total_size=*(uint32_t*)base;for(uint32_t off=8;off+8<=total_size;){uint32_t type=*(uint32_t*)(base+off);uint32_t size=*(uint32_t*)(base+off+4);if(type==TAG_MMAP){uint32_t entry_size=*(uint32_t*)(base+off+8);for(uint32_t e=off+16;e<off+size;e+=entry_size){uint64_t addr=*(uint64_t*)(base+e);uint64_t len=*(uint64_t*)(base+e+8);uint32_t first=(uint32_t)(addr/PAGE_SIZE);uint32_t pages=(uint32_t)(len/PAGE_SIZE);if(*(uint32_t*)(base+e+16)==MMAP_AVAILABLE){for(uint32_t p=first;p<first+pages&&p<MAX_PHYS_PAGES;p++){if(bit_test(p)){bit_clear(p);if(total_pages_count<MAX_PHYS_PAGES)total_pages_count++;}}}}}if(size<8)break;off=(off+size+7)&~7u;}if(!total_pages_count){total_pages_count=MAX_PHYS_PAGES;for(uint32_t p=0;p<total_pages_count;p++)bit_clear(p);}reserve_range(0,(uint32_t)&__kernel_end);serial_write("pmm: total pages ");serial_number(total_pages_count);serial_write(", free pages ");serial_number(total_pages_count-used_pages_count);serial_write("\n");}
uint32_t pmm_alloc_page(void){uint32_t flags,p,answer=0;spin_lock_irqsave(&pmm_lock,&flags);for(p=1;p<total_pages_count;p++)if(!bit_test(p)){bit_set(p);page_refs[p]=1;used_pages_count++;answer=p*PAGE_SIZE;break;}spin_unlock_irqrestore(&pmm_lock,flags);return answer;}
void pmm_retain_page(uint32_t a){uint32_t flags,p=a/PAGE_SIZE;spin_lock_irqsave(&pmm_lock,&flags);if(p>0&&p<total_pages_count&&bit_test(p)&&page_refs[p]!=0xffffu)page_refs[p]++;spin_unlock_irqrestore(&pmm_lock,flags);}
void pmm_free_page(uint32_t a){uint32_t flags,p=a/PAGE_SIZE;spin_lock_irqsave(&pmm_lock,&flags);if(p>0&&p<total_pages_count&&bit_test(p)){if(page_refs[p]>1)page_refs[p]--;else{page_refs[p]=0;bit_clear(p);if(used_pages_count)used_pages_count--;}}spin_unlock_irqrestore(&pmm_lock,flags);}
uint32_t pmm_total_pages(void){return total_pages_count;} uint32_t pmm_used_pages(void){return used_pages_count;} uint32_t pmm_free_pages(void){return total_pages_count-used_pages_count;}
