#ifndef CEBLAR_USERLIB_H
#define CEBLAR_USERLIB_H
#include <stdint.h>

int32_t u_read(int fd, void *buffer, uint32_t length);
int32_t u_write(int fd, const void *buffer, uint32_t length);
int32_t u_open(const char *path, uint32_t flags);
int32_t u_close(int fd);
int32_t u_fork(void);
void u_exit(int32_t status);
int32_t u_getpid(void);
int32_t u_yield(void);
int32_t u_sleep(uint32_t milliseconds);
int32_t u_wait(int32_t pid, int32_t *status);
int32_t u_exec(const char *path);
void *u_mmap(void *address, uint32_t length, uint32_t flags);
int32_t u_uname(char *buffer);
int32_t u_kill(int32_t pid,uint32_t signal);int32_t u_sigaction(uint32_t signal,uint32_t handler,uint32_t mask);int32_t u_sigreturn(void);
int32_t u_futex_wait(uint32_t *word,uint32_t expected);int32_t u_futex_wait_timeout(uint32_t *word,uint32_t expected,uint32_t timeout_ms);
int32_t u_futex_wake(uint32_t *word,uint32_t count);
int32_t u_munmap(void *address, uint32_t length);

#endif
