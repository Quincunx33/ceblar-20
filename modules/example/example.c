#include "module.h"
#include "kernel.h"
static int example_init(void){kputs("module: example loaded\n");return 0;} static void example_exit(void){}
MODULE_DEFINE("example","0.1.0","ceblar contributors","MIT",example_init,example_exit);
