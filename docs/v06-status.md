# ceblar-20 v0.6 status

## Implemented in this revision

v0.6 now replaces the normal fork path’s eager address-space copy with COW page sharing. Physical pages have reference counts; parent and child mappings become read-only COW mappings; a write fault allocates a private page, copies the original contents through a temporary kernel mapping, updates the faulting PTE, and releases the shared reference. The boot-time COW self-test verifies that the child can diverge without changing the parent.

The VM syscall path now supports page-aligned range `mmap` with automatic placement when the requested address is zero, overlap detection, rollback on allocation failure, protection flags, and range `munmap`. The VMM exposes per-page user unmapping and releases physical references, including COW references. User-pointer validation continues to walk every page before syscall I/O.

Process lifecycle handling now copies the per-process descriptor table at fork, synchronizes descriptor context on scheduler activation, releases non-standard descriptors at exit, and reclaims a zombie’s address space and descriptors when its parent successfully waits. The existing frame-aware wait wakeup path remains in place. The network tree now provides an IPv4 checksum routine, Ethernet/IPv4/UDP packet structures, a bounded loopback queue, and a serial-visible network initialization marker.

Finally, the FAT32 fixture now contains both `HELLO.TXT` and a distinct `HELLO.ELF`. QEMU schedules that disk-loaded ELF in ring 3 and captures `disk: hello`, proving the VFS/FAT32/ELF/process pipeline end to end.

## Verification

`make clean && make -j2 && grub-file --is-x86-multiboot2 build/kernel.elf && make test` succeeds. The no-disk and attached-FAT32 QEMU runs both retain `user: hello`, `vmm: COW self-test passed`, worker alternation, and worker completion. The attached-disk run additionally emits `fat32: HELLO.TXT read ok`, `exec: disk ELF queued`, and `disk: hello`.

## Remaining boundaries

This remains an educational 32-bit kernel. The network code is a loopback protocol foundation, not an `e1000`/`8139` hardware driver or a complete IP/UDP/TCP socket implementation. The mmap ABI is anonymous and page-backed; it does not yet support file mappings, `munmap` interval coalescing, or a VMA object model. Process-table slots are not yet recycled, orphan adoption is minimal, and descriptor sharing uses copied descriptor state rather than reference-counted open-file descriptions. These are the next hardening targets after the v0.6 proof.
