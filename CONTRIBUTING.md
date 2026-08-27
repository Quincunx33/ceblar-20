# Contributing to ceblar-20

Contributions are welcome for drivers, schedulers, file systems, documentation, and tests. Keep changes small, freestanding, and independently reviewable.

## Adding a component

Start with a public header in `include/`, place implementation under the matching subsystem directory, and add a focused QEMU or unit-level test. Drivers should expose initialization and clearly define I/O ownership. Scheduler implementations should conform to `scheduler_t`; file systems should integrate through the VFS boundary.

## Code style

Use C11-compatible freestanding C, Linux-style indentation, fixed-width integer types, explicit casts at hardware boundaries, and no host-library dependencies. Treat compiler warnings as errors. Every exported function requires a short API comment in the corresponding header or documentation.

## Review process

A pull request should explain its design, build successfully with `make`, pass the Multiboot2 and QEMU smoke checks, and update documentation when a public API changes. Reviewers should check memory ownership, interrupt safety, privilege boundaries, and failure paths.
