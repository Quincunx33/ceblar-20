#include <stdint.h>
#include "net.h"
#include "drivers.h"

#define NET_QUEUE_SIZE 4u
#define NET_PACKET_SIZE 1536u

typedef struct arp_entry { uint32_t address; uint8_t mac[6]; uint8_t valid; } arp_entry_t;
static uint8_t queue[NET_QUEUE_SIZE][NET_PACKET_SIZE]; static uint32_t queue_len[NET_QUEUE_SIZE]; static uint32_t queue_head, queue_tail, queue_count; static arp_entry_t arp[4]; static int ready;
static uint16_t swap16(uint16_t x){return(uint16_t)((x<<8)|(x>>8));}
uint16_t net_checksum(const void*data,uint32_t length){const uint8_t*p=(const uint8_t*)data;uint32_t sum=0;while(length>1u){sum+=(uint16_t)(((uint16_t)p[0]<<8)|p[1]);p+=2;length-=2;if(sum>0xffffu)sum=(sum&0xffffu)+(sum>>16);}if(length)sum+=(uint16_t)((uint16_t)p[0]<<8);while(sum>>16)sum=(sum&0xffffu)+(sum>>16);return(uint16_t)~sum;}
void net_init(void){uint32_t i;queue_head=0;queue_tail=0;queue_count=0;ready=1;for(i=0;i<4;i++)arp[i].valid=0;serial_write("net: IPv4/UDP foundation ready (loopback)\n");}
int net_loopback_send(const void*packet,uint32_t length){uint32_t i;if(!ready||!packet||length==0||length>NET_PACKET_SIZE||queue_count>=NET_QUEUE_SIZE)return-1;for(i=0;i<length;i++)queue[queue_tail][i]=((const uint8_t*)packet)[i];queue_len[queue_tail]=length;queue_tail=(queue_tail+1u)%NET_QUEUE_SIZE;queue_count++;return 0;}
uint32_t net_loopback_received(void){uint32_t length;if(!queue_count)return 0;length=queue_len[queue_head];queue_head=(queue_head+1u)%NET_QUEUE_SIZE;queue_count--;return length;}
uint16_t net_htons(uint16_t value){return swap16(value);}
