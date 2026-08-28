#ifndef CEBLAR_MM_H
#define CEBLAR_MM_H
#include "types.h"
#define PAGE_SIZE 4096u
#define PAGE_PRESENT 1u
#define PAGE_WRITE 2u
#define PAGE_USER 4u
#define PAGE_COW 0x200u
#define VMM_MAX_VMAS 64u
typedef struct vma {uint32_t start;uint32_t end;uint32_t flags;uint8_t used;} vma_t;
void pmm_init(uint32_t multiboot_info_addr); uint32_t pmm_alloc_page(void); void pmm_free_page(uint32_t address); void pmm_retain_page(uint32_t address); uint32_t pmm_total_pages(void); uint32_t pmm_used_pages(void); uint32_t pmm_free_pages(void);
typedef struct address_space {uint32_t page_directory[1024] __attribute__((aligned(4096)));uint32_t user_tables[8][1024] __attribute__((aligned(4096)));uint8_t table_used[8];vma_t vmas[VMM_MAX_VMAS];uint32_t refs;} address_space_t;
void vmm_init(void); void vmm_map(uint32_t virt,uint32_t phys,uint32_t flags); void vmm_unmap(uint32_t virt); uint32_t vmm_get_physical(uint32_t virt); void vmm_page_fault(uint32_t error,uint32_t address);
address_space_t *vmm_create_address_space(void); void vmm_retain_address_space(address_space_t *space); address_space_t *vmm_clone_address_space(address_space_t *source); address_space_t *vmm_clone_address_space_cow(address_space_t *source); void vmm_destroy_address_space(address_space_t *space); void vmm_switch_address_space(address_space_t *space); address_space_t *vmm_current_address_space(void); int vmm_map_user(address_space_t *space,uint32_t virt,uint32_t phys,uint32_t flags); uint32_t vmm_user_alloc(address_space_t *space,uint32_t virt,uint32_t pages,uint32_t flags); int vmm_map_range(address_space_t *space,uint32_t virt,uint32_t pages,uint32_t flags); int vmm_map_lazy_range(address_space_t *space,uint32_t virt,uint32_t pages,uint32_t flags); int vmm_unmap_range(address_space_t *space,uint32_t virt,uint32_t pages); int vmm_unmap_user(address_space_t *space,uint32_t virt); uint32_t vmm_get_user_physical(address_space_t *space,uint32_t virt); int vmm_validate_user_ptr(address_space_t *space,uint32_t virt,uint32_t len,uint32_t write); int vmm_clone_selftest(void); int vmm_cow_selftest(void);
void heap_init(void); void *kmalloc(usize_t size); void kfree(void *ptr); void *krealloc(void *ptr, usize_t size);
#endif
