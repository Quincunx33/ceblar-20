# Contributing an IRQ-based driver

Create a driver directory under `drivers/`, add a public declaration to `include/drivers.h`, initialize hardware in a dedicated `*_init()` function, and register an IRQ callback through `register_irq_handler()`. The callback must drain device state quickly, acknowledge device-specific status, and defer long work to a scheduler-safe queue.

```c
#include "isr.h"
#include "module.h"
static void my_irq(registers_t *r) { (void)r; }
static int my_init(void) { register_irq_handler(5, my_irq); return 0; }
static void my_exit(void) { }
MODULE_DEFINE("my-driver", "0.1.0", "Contributor", "MIT", my_init, my_exit);
```

Add a QEMU or hardware test, serial initialization output, ownership notes, and failure-path documentation. Do not call host libc or block inside an IRQ callback.
