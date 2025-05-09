#include <stdio.h>
#include <stdlib.h>
#include "../../include/node.h"
#include "../../include/utilsIP.h"
#include "../../include/utilsHasArgument.h"
#include "../../include/utilsSignalHandler.h"
#include "../../include/mDnsScanServiceThread.h"

void client_scan(int argc, char *argv[]) {
    //------------------------------------------------
    // Vars
    //------------------------------------------------
    char ip_buffer[INET6_ADDRSTRLEN], *endptr;
    const char *timeout = get_arg_value(argc, argv, "-t");
    
    MdnsScanParams params = {
        .one_shot = 1,
        .timeout_secs = 3
    };
    
    if (timeout) {
        long val = strtol(timeout, &endptr, 10);
        if (*endptr == '\0')
            params.timeout_secs = val;
    }
    
    //------------------------------------------------
    // mDNA Scan
    //------------------------------------------------
    printf("scanning for %i seconds...\n\n", params.timeout_secs);
    mdns_scan_service_thread(&params);

    //------------------------------------------------
    // Output
    //------------------------------------------------
    ServiceInfo *s = node.config.network.mDNSserviceList;
    while (s) {
        printf("Service:\n");
        printf("  Name:       %s\n", s->name);
        printf("  Host:       %s\n", s->host);
        printf("  Type:       %s\n", s->type);
        printf("  Interface:  %s\n", s->interface);
        printf("  Port:       %u\n", s->port);
        if (s->ip4address != 0) {
            ip_to_string(AF_INET, &s->ip4address, ip_buffer, sizeof(ip_buffer));
            printf("  ip4address: %s\n", ip_buffer);
        } 
        if (is_ipv6_addr_set(&s->ip6address)) {
            ip_to_string(AF_INET6, &s->ip6address, ip_buffer, sizeof(ip_buffer));
            printf("  ip6address: %s\n", ip_buffer);
        }        
        s = s->next;
        printf("\n");
    }

    

}