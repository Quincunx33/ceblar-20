#ifndef CEBLAR_VFS_H
#define CEBLAR_VFS_H
#include "types.h"
typedef struct vfs_file { const char *name; uint32_t flags; uint32_t position; } vfs_file_t;
void vfs_init(void); int vfs_open(const char *name, uint32_t flags); int vfs_close(int fd); ssize_t vfs_read(int fd, void *buf, usize_t n); ssize_t vfs_write(int fd, const void *buf, usize_t n);
#endif
