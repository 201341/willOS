/**
 * @file
 * @see https://en.wikipedia.org/wiki/IPv4#Header
 * @see https://www.saminiir.com/lets-code-tcp-ip-stack-2-ipv4-icmpv4/
 */
#ifndef NET_IPV4_H
#define NET_IPV4_H

#include <arpa/inet.h>
#include <net/net.h>
#include <sys/types.h>

// See: https://www.frameip.com/entete-ip/#39-8211-protocole
#define IPV4_PROTO_ICMP 0x1
#define IPV4_PROTO_IGMP 0x2
#define IPV4_PROTO_TCP  0x6
#define IPV4_PROTO_UDP  0x11

#define IPV4_VERSION     4
#define IPV4_DEFAULT_TTL 255
#define IPV4_FLAG_DF     0x4000

#define ICMPV4_TYPE_REPLY       0
#define ICMPV4_TYPE_UNREACHABLE 3
#define ICMPV4_TYPE_REQUEST     8

typedef struct ipv4_header
{
  // TODO: `ihl` and `version` are inverted to "deal" with endianness. It's not
  // the right thing to do but it works so we'll fix that later.
  uint8_t ihl : 4;
  uint8_t version : 4;
  uint8_t service;
  uint16_t len;
  uint16_t id;
  uint16_t flags;
  uint8_t ttl;
  uint8_t proto;
  uint16_t checksum;
  uint32_t src_addr;
  uint32_t dst_addr;
} __attribute__((packed)) ipv4_header_t;

typedef struct icmpv4_echo
{
  uint8_t type;
  uint8_t code;
  uint16_t checksum;
  uint16_t id;
  uint16_t sequence;
  uint8_t* data;
} __attribute__((packed)) icmpv4_echo_t;

typedef struct icmpv4_reply
{
  uint8_t src_ip[4];
  uint16_t sequence;
  uint8_t ttl;
} icmpv4_reply_t;

/**
 * "Pings" an IPv4 address, i.e. sends an ICMPv4 packet to it and waits for an
 * ICMPv4 reply.
 *
 * @param interface the interface to use to send the ICMPv4 request
 * @param ip the IPv4 address to ping
 * @param reply the ICMPv4 reply
 * @return `0` on success, something else otherwise
 */
int ipv4_ping(net_interface_t* interface, uint8_t ip[4], icmpv4_reply_t* reply);

/**
 * Handles IPv4 packets.
 *
 * @param interface the interface that received the IPv4 packet
 * @param data the data containing the IPv4 packet
 * @param len the length of the data
 */
void ipv4_receive_packet(net_interface_t* interface,
                         uint8_t* data,
                         uint32_t len);

/**
 * Computes an Internet checksum as defined in RFC1071.
 *
 * @see https://tools.ietf.org/html/rfc1071
 * @see https://tools.ietf.org/html/rfc1141
 *
 * @param addr the address of the data to compute a checksum to
 * @param count the number of data bytes to use for the checksum
 * @return a checksum value
 */
uint16_t ipv4_checksum(void* addr, int count);

/**
 * Sends an IPv4 packet.
 *
 * @param interface the interface to use to send the IPv4 request
 * @param dst_addr the destination socket address
 * @param protocol the protocol encapsulated in the IP packet
 * @param flags IP flags
 * @param data the data to send over IP
 * @param len the size of the data to send
 */
void ipv4_send_packet(net_interface_t* interface,
                      struct sockaddr_in* dst_addr,
                      uint8_t protocol,
                      uint16_t flags,
                      uint8_t* data,
                      uint32_t len);

#endif
