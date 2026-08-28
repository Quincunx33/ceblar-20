#ifndef CEBLAR_NET_H
#define CEBLAR_NET_H
#include <stdint.h>

typedef struct net_eth { uint8_t dst[6]; uint8_t src[6]; uint16_t type; } __attribute__((packed)) net_eth_t;
typedef struct net_ipv4 { uint8_t version_ihl; uint8_t tos; uint16_t length; uint16_t id; uint16_t flags_fragment; uint8_t ttl; uint8_t protocol; uint16_t checksum; uint32_t source; uint32_t destination; } __attribute__((packed)) net_ipv4_t;
typedef struct net_udp { uint16_t source; uint16_t destination; uint16_t length; uint16_t checksum; } __attribute__((packed)) net_udp_t;typedef struct net_tcp {uint16_t source,destination;uint32_t sequence,acknowledgement;uint8_t data_offset,flags;uint16_t window,checksum,urgent;} __attribute__((packed)) net_tcp_t;enum {NET_SOCKET_CLOSED=0,NET_SOCKET_SYN_SENT=1,NET_SOCKET_ESTABLISHED=2,NET_SOCKET_FIN_WAIT=3};typedef struct net_socket {uint16_t local_port,remote_port;uint32_t sequence,acknowledgement;uint8_t state,used;} net_socket_t;typedef struct net_ipv6 {uint32_t version_traffic_flow;uint16_t payload_length;uint8_t next_header;uint8_t hop_limit;uint8_t source[16];uint8_t destination[16];} __attribute__((packed)) net_ipv6_t;

void net_init(void);
uint16_t net_checksum(const void *data, uint32_t length);int net_tcp_validate(const net_tcp_t*header,uint32_t length);uint16_t net_tcp_checksum(const net_ipv6_t*header,const net_tcp_t*tcp,uint32_t length);int net_ipv6_selftest(void);int net_ipv6_validate(const void*packet,uint32_t length);uint16_t net_icmpv6_checksum(const net_ipv6_t*header,const void*payload,uint32_t length);
int net_loopback_send(const void *packet, uint32_t length);
uint32_t net_loopback_received(void);int net_socket_open(net_socket_t*s,uint16_t local_port);int net_socket_connect(net_socket_t*s,uint16_t remote_port);int net_socket_send(net_socket_t*s,const void*data,uint32_t length);int net_socket_close(net_socket_t*s);

#endif
