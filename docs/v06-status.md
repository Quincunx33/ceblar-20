# ceblar-20 v0.6 status

## Release direction

v0.6 is the virtual-memory and process-lifecycle hardening release built on the v0.5 foundation. Its principal goal is to replace eager fork copying and one-page mapping primitives with resource-efficient, validated process memory operations.

## Planned deliverables

The release is planned to add copy-on-write page sharing with per-page reference counts, read-only parent/child mappings, a page-fault COW handler, and rollback-safe fork failure paths. The memory subsystem should also grow a complete `mmap`/`munmap` contract with alignment, overlap, protection, ownership, and unmapping rules rather than exposing only a single anonymous page mapping.

Process work includes descriptor inheritance and cleanup, complete zombie reaping, parent reparenting, richer wait semantics, and stable process enumeration. The syscall ABI should gain explicit error constants, validated pathname handling, safer `exec` replacement, and additional file and memory calls. These changes must preserve the ring-3 interrupt-frame ABI and the v0.5 preemption regression.

## Current foundation

v0.5 already provides isolated address spaces, deep eager fork cloning, page-walk user-pointer validation, a bounded single-page `SYS_MMAP` primitive, an ELF32 loader, IRQ0 frame-aware preemption, FAT32 reads, disk-backed exec code, and wait-queue state transitions. Those components are intentionally documented as foundations rather than final v0.6 behavior.

## Acceptance criteria

A v0.6 release should demonstrate that parent and child share physical pages until a write fault, that a COW write produces a private page, that invalid mappings are rejected without corrupting neighboring regions, that wait reaps child resources, and that the existing no-disk and FAT32 QEMU smoke tests still emit `user: hello`.
