#include <stdint.h>
#include "net.h"
#include "drivers.h"

#define NET_QUEUE_SIZE 4u
#define NET_PACKET_SIZE 1536u
#define NET_IPV6_HEADER_SIZE 40u

typedef struct arp_entry { uint32_t address; uint8_t mac[6]; uint8_t valid; } arp_entry_t;
static uint8_t queue[NET_QUEUE_SIZE][NET_PACKET_SIZE]; static uint32_t queue_len[NET_QUEUE_SIZE]; static uint32_t queue_head, queue_tail, queue_count; static arp_entry_t arp[4]; static int ready;
static uint16_t swap16(uint16_t x){return(uint16_t)((x<<8)|(x>>8));}
static uint32_t add_words(uint32_t sum,const uint8_t*p,uint32_t length){while(length>1u){sum+=(uint16_t)(((uint16_t)p[0]<<8)|p[1]);p+=2;length-=2;if(sum>0xffffu)sum=(sum&0xffffu)+(sum>>16);}if(length)sum+=(uint16_t)((uint16_t)p[0]<<8);return sum;}
static uint16_t fold_checksum(uint32_t sum){while(sum>>16)sum=(sum&0xffffu)+(sum>>16);return(uint16_t)~sum;}
uint16_t net_checksum(const void*data,uint32_t length){return fold_checksum(add_words(0,(const uint8_t*)data,length));}
int net_ipv6_selftest(void){uint8_t packet[40];for(uint32_t i=0;i<40u;i++)packet[i]=0;packet[0]=0x60u;return net_ipv6_validate(packet,sizeof(packet))==0&&net_icmpv6_checksum((const net_ipv6_t*)packet,0,0)!=0?0:-1;} void net_init(void){uint32_t i;queue_head=0;queue_tail=0;queue_count=0;ready=1;for(i=0;i<4;i++)arp[i].valid=0;serial_write("net: IPv4/UDP foundation ready (loopback)\n");serial_write("net: IPv6 header/checksum foundation ready\n");if(net_ipv6_selftest()==0)serial_write("net: IPv6 self-test passed\n");}
int net_loopback_send(const void*packet,uint32_t length){uint32_t i;if(!ready||!packet||length==0||length>NET_PACKET_SIZE||queue_count>=NET_QUEUE_SIZE)return-1;for(i=0;i<length;i++)queue[queue_tail][i]=((const uint8_t*)packet)[i];queue_len[queue_tail]=length;queue_tail=(queue_tail+1u)%NET_QUEUE_SIZE;queue_count++;return 0;}
uint32_t net_loopback_received(void){uint32_t length;if(!queue_count)return 0;length=queue_len[queue_head];queue_head=(queue_head+1u)%NET_QUEUE_SIZE;queue_count--;return length;}
uint16_t net_htons(uint16_t value){return swap16(value);}
int net_ipv6_validate(const void*packet,uint32_t length){const net_ipv6_t*h=(const net_ipv6_t*)packet;uint32_t payload;if(!h||length<NET_IPV6_HEADER_SIZE)return-1;if((((const uint8_t*)packet)[0]>>4)!=6u)return-1;payload=(uint32_t)swap16(h->payload_length);return payload>length-NET_IPV6_HEADER_SIZE?-1:0;}
int net_tcp_validate(const net_tcp_t*h,uint32_t length){uint32_t offset;if(!h||length<20u)return-1;offset=((uint32_t)(h->data_offset>>4u))*4u;if(offset<20u||offset>length)return-1;if(h->source==0||h->destination==0)return-1;return 0;} uint16_t net_tcp_checksum(const net_ipv6_t*h,const net_tcp_t*t,uint32_t length){uint8_t pseudo[40];uint32_t sum;if(!h||!t||length>0xffffu)return 0;for(uint32_t i=0;i<16u;i++){pseudo[i]=h->source[i];pseudo[16u+i]=h->destination[i];}pseudo[32]=(uint8_t)(length>>24);pseudo[33]=(uint8_t)(length>>16);pseudo[34]=(uint8_t)(length>>8);pseudo[35]=(uint8_t)length;pseudo[36]=0;pseudo[37]=0;pseudo[38]=0;pseudo[39]=6;sum=add_words(0,pseudo,sizeof(pseudo));return fold_checksum(add_words(sum,(const uint8_t*)t,length));} uint16_t net_icmpv6_checksum(const net_ipv6_t*h,const void*payload,uint32_t length){uint8_t pseudo[40];uint32_t sum;if(!h||(!payload&&length))return 0;for(uint32_t i=0;i<16u;i++){pseudo[i]=h->source[i];pseudo[16u+i]=h->destination[i];}pseudo[32]=(uint8_t)(length>>24);pseudo[33]=(uint8_t)(length>>16);pseudo[34]=(uint8_t)(length>>8);pseudo[35]=(uint8_t)length;pseudo[36]=0;pseudo[37]=0;pseudo[38]=0;pseudo[39]=58;sum=add_words(0,pseudo,sizeof(pseudo));sum=add_words(sum,(const uint8_t*)payload,length);return fold_checksum(sum);}
