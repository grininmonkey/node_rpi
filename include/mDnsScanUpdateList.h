#ifndef MDNS_SCAN_UPDATE_LIST_H
#define MDNS_SCAN_UPDATE_LIST_H

#include <stdint.h>

void add_service(const char *name, const char *host, const char *type, const char *addr, uint16_t port, const char *interface);
void remove_service(const char *name, const char *host, const char *type);

#endif