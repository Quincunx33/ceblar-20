#ifndef CEBLAR_JOURNAL_H
#define CEBLAR_JOURNAL_H
#include <stdint.h>
int journal_init(uint32_t start_lba,uint32_t slots);int journal_replay(void);int journal_write(uint32_t target_lba,const void*sector);int journal_clear(void);int journal_is_ready(void);
#endif
