#include "vfs.h"
#include "drivers.h"
static vfs_file_t files[8]; void vfs_init(void){for(int i=0;i<8;i++)files[i].name=0;}
int vfs_open(const char*n,uint32_t f){for(int i=0;i<8;i++)if(!files[i].name){files[i].name=n;files[i].flags=f;files[i].position=0;return i;}return -1;}
int vfs_close(int fd){if(fd<0||fd>=8||!files[fd].name)return -1;files[fd].name=0;return 0;}
ssize_t vfs_read(int fd,void*b,usize_t n){if(fd==0&&b&&n){int c=serial_read();if(c<0)return 0;((char*)b)[0]=(char)c;return 1;}return -1;}
ssize_t vfs_write(int fd,const void*b,usize_t n){if(!b)return -1;if(fd==1){vga_write((const char*)b);return (ssize_t)n;}if(fd==2){serial_write_n((const char*)b,n);return (ssize_t)n;}if(fd<0||fd>=8||!files[fd].name)return -1;files[fd].position+=n;return (ssize_t)n;}
