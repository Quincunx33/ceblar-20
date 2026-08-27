# ceblar-20 roadmap

| Release | Scope | Status |
|---|---|---|
| v0.3 | IDT/PIC/ISR, PMM, VMM, heap, scheduling foundations, syscalls, VGA/serial, keyboard, timer, modules, TSS foundation, libc, shell, and build tooling | Complete |
| v0.4 | ELF32 PT_LOAD loader, ring-3 `iret` userspace, isolated address spaces, CR3/TSS switching, PMM-backed user stack, eager fork clone, and `SYS_WRITE` smoke test | Complete |
| v0.5 | Live IRQ0 preemption, ATA PIO LBA28, raw VFS `hda` fd 3, FAT32 read-only mount/8.3 reads/readdir, disk-backed `SYS_EXEC`, blocking wait queues, page-walk user-pointer validation, and shell `ps`/`exec` | Implemented and smoke-tested; hardening continues |
| v0.6 | Copy-on-Write fork, complete demand-paged `mmap`, richer process/file/syscall ABI, descriptor inheritance, and stronger resource reclamation | Foundation present; planned next |
| v0.7 | `e1000`/`8139` network module, Ethernet, ARP, IPv4, UDP/TCP foundations, and socket syscalls | Planned |
| v0.8 | ELF dynamic linking, shared libraries, process environment, and a larger userspace runtime | Planned |
| v1.0 | Self-hosting development environment, kernel build tools, reproducible image creation, and sustained hardware support | Long-term |

The v0.5 implementation is intentionally conservative: ATA and FAT32 are polling-based and read-only at the filesystem layer, while raw fd 3 exposes bounded sector I/O. The current boot proof remains the embedded ring-3 image and `user: hello`; disk-backed exec is available through the VFS/syscall path and should be tested with the generated FAT32 fixture.
