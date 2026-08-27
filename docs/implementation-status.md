# Implementation status

The current release is a **bootable educational kernel MVP**. It boots through GRUB Multiboot2 into 32-bit protected mode, initializes VGA and serial output, installs a GDT and IDT, programs the PIT, probes the keyboard port, initializes memory and VFS interfaces, registers and loads the example module, and exposes an extensible syscall dispatcher.

The repository contains public interfaces and initial implementations for PMM, VMM, heap allocation, PCB creation, context-switch boundaries, round-robin scheduling, VFS descriptors, FAT32 mounting boundary, user init, and minimal libc helpers.

Production-grade features intentionally remain follow-up work: full interrupt stubs and PIC remapping, a bitmap backed by the Multiboot memory map, page-table activation, a reclaimable heap, real preemptive context switching, ring-3 transitions and ELF `exec`, block-device-backed FAT32 I/O, dynamic ELF module relocation/unloading, TCP/IP, and comprehensive automated runtime assertions. These boundaries are documented rather than represented as falsely complete functionality.

## Verified commands

```sh
make clean
make
 grub-file --is-x86-multiboot2 build/kernel.elf
 timeout 8s qemu-system-i386 -cdrom build/ceblar-20.iso -display none -serial stdio -monitor none -no-reboot
```

The build completes, the Multiboot2 check succeeds, and QEMU reaches the kernel with serial output showing `module: example loaded`.
