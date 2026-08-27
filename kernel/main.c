#include "kernel.h"
#include "drivers.h"
#include "mm.h"
#include "module.h"
#include "scheduler.h"
#include "syscall.h"
#include "vfs.h"
#include "isr.h"
#include "elf.h"
#include "ata.h"
#include "fat32.h"
#include "net.h"
#include "pci.h"
#include "acpi.h"
#include "dma.h"
#include "smp.h"
#include "apic.h"
extern void timer_irq(void *registers); extern void keyboard_irq(void *registers);
extern kernel_module_t module_example_init; extern const uint8_t user_image[]; extern const uint32_t user_image_size;
static void banner(void) { vga_write("ceblar-20 modular kernel\n"); vga_write("status: booted in 32-bit protected mode\n"); }
static void worker_body(void){__asm__ volatile("sti");serial_write("scheduler: worker online\n");pcb_t*p=scheduler_current();for(uint32_t i=0;i<3;i++){kprintf("worker pid=%u tick=%u\n",p?p->pid:0,timer_uptime_ms());ksleep_ms(500);}serial_write("worker: sleep cycle complete\n");if(p)process_exit(p->pid);for(;;){__asm__ volatile("hlt");}}
static void run_preemption_test(void){__asm__ volatile("cli");pcb_t*a=process_create_entry("worker-a",worker_body);pcb_t*b=process_create_entry("worker-b",worker_body);if(!a||!b||scheduler_add(a)!=0||scheduler_add(b)!=0)panic("preemption test setup failed");serial_write("scheduler: workers queued for preemption test\n");} static void disk_exec_smoke(void){pcb_t*p=process_create_from_path("disk-init","hda/HELLO.ELF");if(p&&scheduler_add(p)==0)serial_write("exec: disk ELF queued\n");} static void storage_smoke(void){if(fat32_is_mounted()){fat32_file_t f;uint8_t b[64];int32_t n;if(fat32_open("HELLO.TXT",&f)==0){n=fat32_read_file(&f,b,sizeof(b));if(n>0)serial_write("fat32: HELLO.TXT read ok\n");}}}
void kmain(uint32_t magic, uint32_t mbi_addr) {
    (void)mbi_addr; vga_init(); serial_init(); banner();
    if (magic != 0x36d76289) { panic("invalid Multiboot2 magic"); }
    gdt_init(); tss_init(0); idt_init(); acpi_init(); smp_init(); pmm_init(mbi_addr); vmm_init(); apic_init(); if(vmm_clone_selftest()!=0)panic("address-space clone self-test failed"); serial_write("vmm: clone self-test passed\n"); if(vmm_cow_selftest()!=0)panic("COW self-test failed"); serial_write("vmm: COW self-test passed\n"); heap_init();
    pit_init(100); pci_init(); dma_init(); keyboard_init(); register_irq_handler(0,(irq_handler_t)timer_irq); register_irq_handler(1,(irq_handler_t)keyboard_irq); pic_unmask(0); pic_unmask(1); ata_init(); fat32_mount(0); storage_smoke(); vfs_init(); net_init(); syscall_init(); run_preemption_test();
    module_register(&module_example_init); module_load_all(); vga_write("subsystems: memory, interrupts, drivers, modules, scheduler, syscall, vfs\n");
    pcb_t *user=process_create_user("init",0); uint32_t entry=0; if(!user||elf32_load(user_image,user_image_size,user->address_space,&entry)!=0)panic("embedded ELF load failed"); user->entry_point=entry; user->cpu_state.eip=entry; disk_exec_smoke(); serial_write("userspace: launching ring3 init\n"); process_enter_user(user);
}
