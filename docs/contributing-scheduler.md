# Contributing a scheduler

Implement the `scheduler_t` interface with `init`, `tick`, and `current` callbacks. Keep process states explicit and ensure a READY process is not lost when a running process yields. Register the scheduler during kernel initialization and test quantum behavior with deterministic serial traces.

```c
static int init(void) { return 0; }
static void tick(void) { }
static pcb_t *current(void) { return 0; }
scheduler_t custom = { "custom", init, tick, current };
```

Schedulers must not assume a particular device driver and must document whether context switching is cooperative or preemptive.
