#ifndef CEBLAR_FAT32_H
#define CEBLAR_FAT32_H
#include <stdint.h>

typedef struct fat32_file { uint32_t first_cluster; uint32_t size; uint32_t position; uint8_t directory; } fat32_file_t;
int fat32_mount(void *device);
int fat32_is_mounted(void);
int fat32_open(const char *path, fat32_file_t *file);
int32_t fat32_read_file(fat32_file_t *file, void *buffer, uint32_t length);
int fat32_readdir(uint32_t index, char *name, uint32_t capacity, uint32_t *size);

#endif
