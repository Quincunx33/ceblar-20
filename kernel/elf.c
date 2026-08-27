#include <stdint.h>
#include "elf.h"
#include "drivers.h"
#define PT_LOAD 1u
#define ELFCLASS32 1u
#define EM_386 3u
static uint32_t down(uint32_t x){return x&~(PAGE_SIZE-1u);} static uint32_t up(uint32_t x){return(x+PAGE_SIZE-1u)&~(PAGE_SIZE-1u);}
static void zero(void*d,uint32_t n){uint8_t*p=d;while(n--)*p++=0;} static void copy(void*d,const void*s,uint32_t n){uint8_t*a=d;const uint8_t*b=s;while(n--)*a++=*b++;}
int elf32_load(const void*image,uint32_t size,address_space_t*space,uint32_t*entry){if(!image||!space||!entry||size<sizeof(elf32_header_t))return -1;const elf32_header_t*h=(const elf32_header_t*)image;if(h->ident[0]!=0x7f||h->ident[1]!='E'||h->ident[2]!='L'||h->ident[3]!='F'||h->ident[4]!=ELFCLASS32||h->machine!=EM_386||h->phentsize!=sizeof(elf32_program_t))return -1;if(h->phoff>size||h->phnum>(size-h->phoff)/sizeof(elf32_program_t))return -1;vmm_switch_address_space(space);for(uint32_t i=0;i<h->phnum;i++){const elf32_program_t*p=(const elf32_program_t*)((const uint8_t*)image+h->phoff+i*sizeof(elf32_program_t));if(p->type!=PT_LOAD)continue;if(p->filesz>p->memsz||p->offset>size||p->filesz>size-p->offset||p->vaddr<0x400000u||p->vaddr+p->memsz< p->vaddr||p->vaddr+p->memsz>=0xc0000000u){vmm_switch_address_space(0);return -1;}uint32_t first=down(p->vaddr),last=up(p->vaddr+p->memsz);for(uint32_t va=first;va<last;va+=PAGE_SIZE){if(vmm_get_physical(va)==0){uint32_t page=pmm_alloc_page();if(!page){vmm_switch_address_space(0);return -1;}if(vmm_map_user(space,va,page,(p->flags&2u)?PAGE_WRITE:0)!=0){pmm_free_page(page);vmm_switch_address_space(0);return -1;}zero((void*)va,PAGE_SIZE);}}copy((void*)p->vaddr,(const uint8_t*)image+p->offset,p->filesz);zero((uint8_t*)p->vaddr+p->filesz,p->memsz-p->filesz);}*entry=h->entry;vmm_switch_address_space(0);return 0;}
