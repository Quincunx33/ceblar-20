#include "kernel.h"
#include "drivers.h"
#include "mm.h"
#include "module.h"
#include "scheduler.h"
#include "syscall.h"
#include "vfs.h"
#include "isr.h"
#include "elf.h"
extern void timer_irq(void *registers); extern void keyboard_irq(void *registers);
extern kernel_module_t module_example_init; extern const uint8_t user_image[]; extern const uint32_t user_image_size;
static void banner(void) { vga_write("ceblar-20 modular kernel\n"); vga_write("status: booted in 32-bit protected mode\n"); }
void kmain(uint32_t magic, uint32_t mbi_addr) {
    (void)mbi_addr; vga_init(); serial_init(); banner();
    if (magic != 0x36d76289) { panic("invalid Multiboot2 magic"); }
    gdt_init(); tss_init(0); idt_init(); pmm_init(mbi_addr); vmm_init(); if(vmm_clone_selftest()!=0)panic("address-space clone self-test failed"); serial_write("vmm: clone self-test passed\n"); heap_init();
    pit_init(100); keyboard_init(); register_irq_handler(0,(irq_handler_t)timer_irq); register_irq_handler(1,(irq_handler_t)keyboard_irq); pic_unmask(0); pic_unmask(1); interrupts_enable(); vfs_init(); syscall_init();
    module_register(&module_example_init); module_load_all(); vga_write("subsystems: memory, interrupts, drivers, modules, scheduler, syscall, vfs\n");
    pcb_t *user=process_create_user("init",0); uint32_t entry=0; if(!user||elf32_load(user_image,user_image_size,user->address_space,&entry)!=0)panic("embedded ELF load failed"); user->entry_point=entry; user->cpu_state.eip=entry; serial_write("userspace: launching ring3 init\n"); process_enter_user(user);
}
