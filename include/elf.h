#ifndef CEBLAR_ELF_H
#define CEBLAR_ELF_H
#include <stdint.h>
#include "mm.h"
typedef struct elf32_header {uint8_t ident[16];uint16_t type,machine;uint32_t version,entry,phoff,shoff,flags;uint16_t ehsize,phentsize,phnum,shentsize,shnum,shstrndx;} __attribute__((packed)) elf32_header_t;
typedef struct elf32_program {uint32_t type,offset,vaddr,paddr,filesz,memsz,flags,align;} __attribute__((packed)) elf32_program_t;
int elf32_load(const void *image,uint32_t size,address_space_t *space,uint32_t *entry);
#endif
