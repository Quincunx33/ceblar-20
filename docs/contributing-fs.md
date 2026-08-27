# Contributing a filesystem

A filesystem integrates through the VFS boundary rather than calling device ports directly. Define mount, open, read, write, close, and optional directory/stat operations around a block-device abstraction. Validate superblocks before exposing a mount and return negative errors for malformed metadata.

A new filesystem should live under `fs/<name>/`, expose a mount function, document on-disk layout and ownership, and include a read-only QEMU fixture before write support is attempted. The current FAT32 directory is a boundary stub; ATA/block-device support is planned for a later release.
