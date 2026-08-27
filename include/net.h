#ifndef CEBLAR_NET_H
#define CEBLAR_NET_H
#include <stdint.h>

typedef struct net_eth { uint8_t dst[6]; uint8_t src[6]; uint16_t type; } __attribute__((packed)) net_eth_t;
typedef struct net_ipv4 { uint8_t version_ihl; uint8_t tos; uint16_t length; uint16_t id; uint16_t flags_fragment; uint8_t ttl; uint8_t protocol; uint16_t checksum; uint32_t source; uint32_t destination; } __attribute__((packed)) net_ipv4_t;
typedef struct net_udp { uint16_t source; uint16_t destination; uint16_t length; uint16_t checksum; } __attribute__((packed)) net_udp_t;

void net_init(void);
uint16_t net_checksum(const void *data, uint32_t length);
int net_loopback_send(const void *packet, uint32_t length);
uint32_t net_loopback_received(void);

#endif
