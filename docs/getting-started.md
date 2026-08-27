# Getting started

## Prerequisites

Install a freestanding `i686-elf` GCC toolchain, NASM, GNU binutils, GRUB's `grub-mkrescue`, `xorriso`, and `qemu-system-i386`. The exact package names depend on the host distribution.

## Build the skeleton

From the repository root:

```sh
chmod +x tools/build.sh tools/run_qemu.sh
./tools/build.sh
```

The `check` target reports which tools are available. In Step 1, `make all` creates `build/STATUS`; it does not produce an ISO because boot code is intentionally introduced in Step 2.

## Run later bootable steps

After Step 2 adds the Multiboot2 entry point and ISO rule, run:

```sh
./tools/build.sh
./tools/run_qemu.sh
```

QEMU will then boot the generated `build/ceblar-20.iso`.

## Development rule

Each implementation step should keep the project freestanding, compile with warnings treated as errors, document public APIs, and include a reproducible QEMU test command.
