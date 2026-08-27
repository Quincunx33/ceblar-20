# ceblar-20

**ceblar-20** is a modular, open-source x86 kernel framework written in C and x86 Assembly. Its long-term goal is to let contributors add drivers, file systems, schedulers, and other kernel components through documented interfaces.

> **Current status:** Step 1 — project skeleton and build system. Boot code and kernel functionality will be added incrementally in later steps.

## Project layout

| Directory | Purpose |
|---|---|
| `arch/x86/` | x86 boot, descriptor tables, paging, and context-switch code |
| `arch/arm/` | Reserved for future ARM support |
| `kernel/` | Kernel core and system-call code |
| `mm/` | Physical memory, virtual memory, and heap managers |
| `drivers/` | Pluggable device drivers |
| `fs/` | Virtual file system and file-system implementations |
| `modules/` | Module metadata, loader, and example modules |
| `scheduler/` | Replaceable scheduler implementations |
| `userspace/` | Initial user-space programs and minimal libc |
| `include/` | Public kernel and module headers |
| `tools/` | Build, run, and debugging helpers |
| `docs/` | Developer documentation |
| `.github/` | CI and contribution templates |

## Planned toolchain

The target build uses an `i686-elf` GCC cross-compiler, NASM, GNU `ld`, GRUB utilities, `xorriso`, and QEMU. A cross-compiler is preferred because it prevents host-system headers and ABI assumptions from entering a freestanding kernel build.

## Step 1

Step 1 establishes the directory layout, build variables, source discovery rules, ISO output paths, and QEMU runner. There is intentionally no bootable kernel yet; the first boot implementation arrives in Step 2.

## License

MIT. The license file will be added before the first public release.
