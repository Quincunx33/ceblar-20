#ifndef CEBLAR_MODULE_H
#define CEBLAR_MODULE_H
#include "types.h"
typedef int(*module_init_fn)(void); typedef void(*module_exit_fn)(void);
typedef struct kernel_module {const char*name;const char*version;const char*author;const char*license;module_init_fn init;module_exit_fn exit;const char*depends[4];int refcount;bool_t loaded;} kernel_module_t;
int module_register(kernel_module_t*m); int module_load_all(void); int module_unload(const char*name); const char*module_list(uint32_t index); usize_t module_count(void);
#define MODULE_DEFINE(n,v,a,l,initfn,exitfn) kernel_module_t module_##initfn={n,v,a,l,initfn,exitfn,{0,0,0,0},0,false}
#endif
