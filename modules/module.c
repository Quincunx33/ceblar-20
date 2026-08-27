#include "module.h"
static kernel_module_t*mods[32];static usize_t count;
int module_register(kernel_module_t*m){if(!m||count>=32)return -1;mods[count++]=m;return 0;} static int dep_loaded(const char*n){for(usize_t i=0;i<count;i++)if(mods[i]->loaded&&mods[i]->name&&n&&mods[i]->name[0]==n[0])return 1;return 0;}
int module_load_all(void){for(usize_t i=0;i<count;i++){kernel_module_t*m=mods[i];int ok=1;for(int d=0;d<4&&m->depends[d];d++)if(!dep_loaded(m->depends[d]))ok=0;if(!ok)return -1;if(!m->loaded&&m->init&&m->init()!=0)return -1;m->loaded=true;}return 0;}
int module_unload(const char*n){for(usize_t i=0;i<count;i++)if(mods[i]->name&&n){const char*a=mods[i]->name;uint32_t j=0;while(a[j]&&n[j]&&a[j]==n[j])j++;if(!a[j]&&!n[j]){if(mods[i]->refcount>0)return -1;if(mods[i]->exit)mods[i]->exit();mods[i]->loaded=false;return 0;}}return -1;}
const char*module_list(uint32_t i){return i<count?mods[i]->name:0;} usize_t module_count(void){return count;}
