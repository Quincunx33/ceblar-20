# ceblar-20 production-oriented roadmap

## Target

The target is a maintainable, secure, multi-process 32-bit x86 operating-system platform with Linux-like subsystem boundaries and a usable userland. It is not a promise to reproduce the entire Linux kernel feature set; each milestone must have working code, tests, and documented limits.

## Current baseline

The kernel boots through GRUB Multiboot2, initializes GDT/TSS/IDT/PIC, discovers legacy ACPI RSDP and PCI bus-0 functions, manages physical pages with reference counts, maps isolated address spaces, resolves COW write faults, and provides interrupt-safe PMM and heap locks. IRQ0 frame-aware preemption, bootstrap per-CPU state, address-space reference counting, a shared-address-space kernel-thread constructor, ATA PIO, FAT32 reads, VFS descriptors, ring-3 ELF loading, `fork`, `wait`, `exec`, range `mmap`/`munmap`, and a loopback IPv4/UDP foundation are present. Futex wait now validates and compares the userspace word before blocking, with a deterministic boot self-test covering mismatch and equal-value paths. QEMU tests cover both an ISO-only boot and an attached FAT32 disk with a ring-3 ELF payload.

## Milestones

| Milestone | Primary deliverables | Quality gate | Status |
|---|---|---|---|
| M1 core safety | Locking policy, allocator validation, VMA metadata, page reclamation, syscall ABI review | Strict build, fault-injection tests, long QEMU run | Foundation implemented; stress and fault-injection work remains |
| M2 process platform | Reusable PID table, threads, signals, futex-like synchronization, robust wait/reap | Parent/child stress suite and race-oriented tests | In progress; generation-safe fixed-slot PID reuse, VFS slot compatibility, futex compare-before-block, shared-address-space kernel-thread API, signal-action ABI, and saved-frame/sigreturn self-tests are implemented, but end-to-end user handler execution, full thread lifecycle, and stress coverage remain |
| M3 SMP and hardware | APIC/IOAPIC, per-CPU state, SMP startup, PCI BARs, ACPI MADT, DMA-safe buffers | `-smp 2/4` QEMU and hardware matrix | ACPI MADT CPU/LAPIC/IOAPIC/GSI-override parsing and bounded PCI BAR discovery are implemented; BSP-only execution, AP startup, active routing, BAR mapping, and hardware drivers remain |
| M4 storage | Virtio/AHCI/NVMe, buffered I/O, journaled filesystem, permissions, crash recovery | Power-loss simulation and filesystem consistency tests | ATA/FAT32 baseline remains; PCI BAR groundwork and a QEMU legacy Virtio-blk queue smoke test are implemented, but validated sector I/O, generic block integration, AHCI/NVMe, caching, journaling, permissions, and crash recovery remain |
| M5 networking | Virtio-net/e1000, Ethernet RX/TX, ARP, IPv4/IPv6, UDP/TCP, sockets | Packet tests, network namespace tests, fuzzing | Loopback protocol foundation exists; hardware networking remains |
| M6 security | NX/W^X where supported, ASLR, capability/credential model, module policy, audit logging | Negative syscall tests, fuzzing, privilege-boundary review | User-pointer validation, signal masks/action ABI, saved-frame restoration, SIGKILL-equivalent termination, and basic ownership hardening exist; capability security, ASLR, NX/KASLR, auditing, and full end-to-end signal qualification remain |
| M7 userland | libc, init/service manager, shell, utilities, package/update system, installer | Reproducible image and end-to-end user workflows | Syscall wrappers and diagnostic shell exist; full userland remains |
| M8 release | CI, static analysis, fuzzing, stress, crash reports, signed artifacts, recovery path | Reproducible release and documented support policy | CI regression and artifact upload configured; release hardening remains |

## Non-negotiable engineering rules

Every new subsystem must define ownership, lifetime, locking context, interrupt context, failure behavior, and a deterministic test. No feature is marked complete solely because it compiles. Hardware-specific code must fail safely when the device is absent, and user-controlled lengths, addresses, filenames, packet fields, and disk offsets must be validated before access.

The current kernel remains an experimental 32-bit platform until M3–M8 are substantially complete. The roadmap deliberately distinguishes a verified educational implementation from production readiness.
