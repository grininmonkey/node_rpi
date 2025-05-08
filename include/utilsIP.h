#ifndef UTILS_IP_H
#define UTILS_IP_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>

enum IpVersion {
    IP_INVALID = 0,
    IP_V4 = 4,
    IP_V6 = 6
};

enum IpVersion detect_ip_version(const char *addr);
bool is_ipv6_nonzero(const struct in6_addr *addr);
const char *ip_to_string(int af, const void *addr, char *buffer, size_t buffer_size);
int is_ipv6_addr_set(struct in6_addr *addr);
bool is_ipv6_link_local(const struct in6_addr *addr);

#endif