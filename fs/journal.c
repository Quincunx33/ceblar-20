#include <stdint.h>
#include "journal.h"
#include "block.h"
#include "drivers.h"

#define JOURNAL_MAGIC 0x434a4e4cu
#define JOURNAL_COMMITTED 0xa5u
#define JOURNAL_MAX_SLOTS 64u
typedef struct journal_record {uint32_t magic,sequence,target_lba,checksum;uint8_t committed;uint8_t reserved[3];uint8_t data[512];} __attribute__((packed)) journal_record_t;
static uint32_t base,slot_count,sequence;static int ready;static uint32_t checksum(const journal_record_t*r){uint32_t h=2166136261u;for(uint32_t i=0;i<sizeof(r->data);i++){h^=r->data[i];h*=16777619u;}h^=r->target_lba;h*=16777619u;h^=r->sequence;return h;}
int journal_init(uint32_t start_lba,uint32_t slots){base=start_lba;slot_count=slots>JOURNAL_MAX_SLOTS?JOURNAL_MAX_SLOTS:slots;ready=block_default()!=0&&slot_count!=0;sequence=1;if(ready)serial_write("journal: recovery log ready\n");else serial_write("journal: no writable block device\n");return ready?0:-1;}
int journal_is_ready(void){return ready;}
int journal_replay(void){journal_record_t r;int applied=0;if(!ready)return-1;for(uint32_t i=0;i<slot_count;i++){if(block_read_sector(base+i,&r)!=0)continue;if(r.magic!=JOURNAL_MAGIC||r.committed!=JOURNAL_COMMITTED||r.checksum!=checksum(&r))continue;if(block_write_sector(r.target_lba,r.data)==0){applied++;r.committed=0;(void)block_write_sector(base+i,&r);}}return applied;}
int journal_write(uint32_t target_lba,const void*sector){journal_record_t r;if(!ready||!sector||!block_default()->write_sector)return-1;r.magic=JOURNAL_MAGIC;r.sequence=sequence++;r.target_lba=target_lba;r.committed=JOURNAL_COMMITTED;for(uint32_t i=0;i<512u;i++)r.data[i]=((const uint8_t*)sector)[i];r.checksum=checksum(&r);if(block_write_sector(base+(r.sequence%slot_count),&r)!=0)return-1;if(block_write_sector(target_lba,sector)!=0)return-1;r.committed=0;return block_write_sector(base+(r.sequence%slot_count),&r);}
int journal_clear(void){journal_record_t r;if(!ready)return-1;for(uint32_t i=0;i<slot_count;i++){for(uint32_t j=0;j<sizeof(r);j++)((uint8_t*)&r)[j]=0;if(block_write_sector(base+i,&r)!=0)return-1;}return 0;}
