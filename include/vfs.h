#ifndef CEBLAR_VFS_H
#define CEBLAR_VFS_H
#include "types.h"
#include "fat32.h"
typedef struct vfs_file { const char *name; uint32_t flags; uint32_t position; uint8_t kind; fat32_file_t fat; } vfs_file_t;
#define VFS_FD_RAW_HDA 3
#define VFS_FD_FIRST_FILE 4
void vfs_init(void); void vfs_set_process(pid_t pid); void vfs_inherit_process(pid_t parent, pid_t child); void vfs_release_process(pid_t pid); int vfs_open(const char *name, uint32_t flags); int vfs_close(int fd); ssize_t vfs_read(int fd, void *buf, usize_t n); ssize_t vfs_write(int fd, const void *buf, usize_t n);
#endif
