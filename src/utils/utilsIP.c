#include "../../include/utilsIP.h"


enum IpVersion detect_ip_version(const char *addr) {
    struct in_addr ipv4;
    struct in6_addr ipv6;

    if (inet_pton(AF_INET, addr, &ipv4) == 1) {
        return IP_V4;
    } else if (inet_pton(AF_INET6, addr, &ipv6) == 1) {
        return IP_V6;
    } else {
        return IP_INVALID;
    }
}

bool is_ipv6_nonzero(const struct in6_addr *addr) {
    static const struct in6_addr zero_addr = {0};
    return memcmp(addr, &zero_addr, sizeof(struct in6_addr)) != 0;
}

const char *ip_to_string(int af, const void *addr, char *buffer, size_t buffer_size) {
    if (inet_ntop(af, addr, buffer, buffer_size) == NULL) {
        perror("inet_ntop");
        return NULL;
    }
    return buffer;
}

int is_ipv6_addr_set(struct in6_addr *addr) {
    static const struct in6_addr zero_addr = IN6ADDR_ANY_INIT;
    return memcmp(addr, &zero_addr, sizeof(struct in6_addr)) != 0;
}


bool is_ipv6_link_local(const struct in6_addr *addr) {
    // Link-local IPv6 addresses start with fe80::/10
    // That means the first byte is 0xfe and top 6 bits of the second byte are 0x80 (i.e., 0x80 to 0xbf)
    return (addr->s6_addr[0] == 0xfe) && ((addr->s6_addr[1] & 0xc0) == 0x80);
}


/*
    // Example usage:
    char buffer[INET6_ADDRSTRLEN];
    printf("IPv4: %s\n", ip_to_string(AF_INET, &node.config.influxDB.serviceInfo.ip4address, buffer, INET_ADDRSTRLEN));
    printf("IPv6: %s\n", ip_to_string(AF_INET6, &node.config.influxDB.serviceInfo.ip6address, buffer, INET6_ADDRSTRLEN));

*/