#include <stdint.h>
#include "vfs.h"
#include "ata.h"
#include "block.h"
#include "drivers.h"
#define VFS_MAX_PROCESSES 32u
static vfs_file_t files[VFS_MAX_PROCESSES][8]; static uint8_t sector[512]; static pid_t active_pid=1;
static uint32_t slot_for_pid(pid_t pid){uint32_t slot=pid&0xffu;if(slot==0)return 0;slot--;return slot<VFS_MAX_PROCESSES?slot:0;}static vfs_file_t*table(void){return files[slot_for_pid(active_pid)];}
void vfs_set_process(pid_t pid){active_pid=pid?pid:1;}
void vfs_init(void){uint32_t p,i;for(p=0;p<VFS_MAX_PROCESSES;p++)for(i=0;i<8;i++){files[p][i].name=0;files[p][i].kind=0;files[p][i].position=0;}if(block_default()){for(p=0;p<VFS_MAX_PROCESSES;p++){files[p][VFS_FD_RAW_HDA].name="hda";files[p][VFS_FD_RAW_HDA].kind=1;}serial_write("vfs: fd3=hda generic block device\n");}}
void vfs_inherit_process(pid_t parent,pid_t child){uint32_t p=slot_for_pid(parent),c=slot_for_pid(child),i;if(p>=VFS_MAX_PROCESSES||c>=VFS_MAX_PROCESSES)return;for(i=0;i<8;i++)files[c][i]=files[p][i];}
void vfs_release_process(pid_t pid){uint32_t p=slot_for_pid(pid),i;if(p>=VFS_MAX_PROCESSES)return;for(i=VFS_FD_FIRST_FILE;i<8u;i++){files[p][i].name=0;files[p][i].kind=0;files[p][i].position=0;}}
int vfs_open(const char*n,uint32_t f){vfs_file_t*t=table();uint32_t i;if(!n)return-1;if(n[0]=='h'&&n[1]=='d'&&n[2]=='a'&&n[3]=='/'&&fat32_is_mounted()){for(i=VFS_FD_FIRST_FILE;i<8u;i++)if(!t[i].name){t[i].name="fat-file";t[i].kind=2;t[i].flags=f;t[i].position=0;if(fat32_open(n+4,&t[i].fat)!=0){t[i].name=0;return-1;}return(int)i;}}if(n[0]=='h'&&n[1]=='d'&&n[2]=='a'&&n[3]==0)return t[VFS_FD_RAW_HDA].name?VFS_FD_RAW_HDA:-1;for(i=VFS_FD_FIRST_FILE;i<8u;i++)if(!t[i].name){t[i].name=n;t[i].flags=f;t[i].position=0;t[i].kind=0;return(int)i;}return-1;}
int vfs_close(int fd){vfs_file_t*t=table();if(fd<VFS_FD_FIRST_FILE||fd>=8||!t[fd].name)return-1;t[fd].name=0;t[fd].kind=0;return 0;}
ssize_t vfs_read(int fd,void*b,usize_t n){vfs_file_t*t=table();uint8_t*out=(uint8_t*)b;uint32_t done=0;if(!b)return-1;if(fd==0&&n){int c=serial_read();if(c<0)return 0;out[0]=(uint8_t)c;return 1;}if(fd==VFS_FD_RAW_HDA&&t[fd].name){while(done<n){uint32_t lba=t[fd].position/512u,off=t[fd].position%512u,take=512u-off;if(take>n-done)take=n-done;if(block_read_sector(lba,sector)!=0)return done?(ssize_t)done:-1;for(uint32_t i=0;i<take;i++)out[done+i]=sector[off+i];done+=take;t[fd].position+=take;}return(ssize_t)done;}if(fd>=VFS_FD_FIRST_FILE&&fd<8&&t[fd].kind==2)return fat32_read_file(&t[fd].fat,b,n);return-1;}
ssize_t vfs_write(int fd,const void*b,usize_t n){vfs_file_t*t=table();const uint8_t*in=(const uint8_t*)b;uint32_t done=0;if(!b)return-1;if(fd==1){for(uint32_t i=0;i<n;i++)vga_putc((char)in[i]);return(ssize_t)n;}if(fd==2){serial_write_n((const char*)b,n);return(ssize_t)n;}if(fd==VFS_FD_RAW_HDA&&t[fd].name){while(done<n){uint32_t lba=t[fd].position/512u,off=t[fd].position%512u,take=512u-off;if(take>n-done)take=n-done;if(off||take<512u){if(block_read_sector(lba,sector)!=0)return done?(ssize_t)done:-1;}for(uint32_t i=0;i<take;i++)sector[off+i]=in[done+i];if(block_write_sector(lba,sector)!=0)return done?(ssize_t)done:-1;done+=take;t[fd].position+=take;}return(ssize_t)done;}return-1;}
