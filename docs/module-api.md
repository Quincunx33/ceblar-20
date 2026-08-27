# Module API

A module is a statically linked component with metadata and lifecycle callbacks. Dynamic ELF relocation and safe unloading are intentionally deferred because they require a production-grade allocator, symbol table, and reference counting policy.

```c
#include "module.h"
static int init(void) { return 0; }
static void exit(void) { }
MODULE_DEFINE("my-driver", "1.0.0", "Your Name", "MIT", init, exit);
```

The kernel registers the module with `module_register()` and invokes all registered initializers through `module_load_all()`. An initializer returns zero on success and a negative value on failure.

## Subsystem contracts

| Component | Public entry point | Current implementation |
|---|---|---|
| Driver | `*_init`, device operations | VGA, serial, PIT, PS/2 probe |
| Memory | `kmalloc`, `kfree`, `krealloc` | Fixed educational heap plus PMM/VMM hooks |
| Scheduler | `scheduler_t` | Round-robin tick/current interface |
| VFS | `vfs_open/read/write/close` | Descriptor table abstraction |
| Syscalls | `syscall_dispatch` | Extensible syscall table |

Modules must not call host libc, must use fixed-width types, and must document ownership of buffers and device resources.
