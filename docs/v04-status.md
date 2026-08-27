# ceblar-20 v0.4 status

## Completed in this revision

The v0.4 userspace foundation now launches an embedded ELF32 image through a real ring-3 `iret` transition. The image executes `INT 0x80` with `SYS_WRITE`, the kernel dispatches the syscall using the corrected `pusha` interrupt-frame ABI, writes `user: hello` through the serial-backed VFS path, and safely returns to ring 3.

The address-space layer includes shared kernel mappings, isolated user page tables, CR3 switching, PMM-backed user stacks, user code/data descriptors, and a TSS with `esp0`. Fork now performs a deep page-by-page clone of mapped user pages into newly allocated physical pages. A boot-time clone self-test writes a marker in the parent, verifies the child copy, modifies the child, and verifies parent isolation.

## Verification

```sh
make clean
make -j2
grub-file --is-x86-multiboot2 build/kernel.elf
make test
make size
```

The QEMU smoke test reaches all of the following markers:

```text
gdt: initialized successfully
tss: initialized successfully
timer: IRQ0 active
pmm: total pages 32639, free pages 31845
vmm: initialized successfully
vmm: clone self-test passed
syscall: initialized successfully
userspace: launching ring3 init
user: hello
```

## Remaining architectural work

The embedded image is a deterministic smoke-test ELF, not a disk-backed `/bin/init`. `SYS_EXEC` still needs a VFS file source and full image replacement. `wait` currently reports already-zombie children rather than blocking on a wait queue. Fork uses eager copying rather than copy-on-write. User-pointer checks currently enforce canonical low-user ranges but do not yet walk every page before a multi-page operation. These are the next hardening steps for v0.5/v0.6.
