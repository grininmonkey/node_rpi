#include <stdlib.h>
#include "../../../include/node.h"
#include "../../../include/utilsIP.h"
#include "../../../include/utilsNodeFree.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// InfluxDB service helper (should be child of parent lock)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool update_influxDB_service(const char *type) {
    if (
        node.config.influxDB.useServiceScan &&
        SAFE_STRCMP(type, node.config.influxDB.mDNSserviceType) == 0)
    {
        return true;
    }
    return false;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Add to service list
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void add_service(const char *name, const char *host, const char *type, const char *addr, uint16_t port, const char *interface) {
    pthread_mutex_lock(&node.lock);

    bool updateInfluxDB = update_influxDB_service(type);
    ServiceInfo *s = node.config.network.mDNSserviceList;
    ServiceInfo *found = NULL;

    //------------------------------------------------
    // Check if already exists
    //------------------------------------------------
    while (s) {
        if (SAFE_STRCMP(s->name, name) == 0) {
            found = s;
            break;
        }
        s = s->next;
    }

    enum IpVersion ipver = detect_ip_version(addr);
    if (ipver == IP_INVALID) {
        pthread_mutex_unlock(&node.lock);
        return;
    }

    //------------------------------------------------
    // Ignore localhost/loopback addresses
    //------------------------------------------------
    if ((ipver == IP_V4 && SAFE_STRCMP(addr, "127.0.0.1") == 0) ||
        (ipver == IP_V6 && (SAFE_STRCMP(addr, "::") == 0 || SAFE_STRCMP(addr, "::1") == 0))) {
        pthread_mutex_unlock(&node.lock);
        return;
    }

    //------------------------------------------------
    // If found, update appropriate field
    //------------------------------------------------
    if (found) {
        if (ipver == IP_V4) {
            inet_pton(AF_INET, addr, &found->ip4address);
        } else if (ipver == IP_V6) {
            inet_pton(AF_INET6, addr, &found->ip6address);
        }

        if (updateInfluxDB) {
            node.config.influxDB.serviceInfo = *found;
            node.config.influxDB.cfgVersion++;
        }

        pthread_mutex_unlock(&node.lock);
        return;
    }

    //------------------------------------------------
    // Not found — allocate and insert new node
    //------------------------------------------------
    ServiceInfo *new_service = calloc(1, sizeof(ServiceInfo));
    if (!new_service) {
        perror("calloc failed");
        pthread_mutex_unlock(&node.lock);
        return;
    }    

    new_service->port       = port;
    new_service->name       = strdup(name);
    new_service->host       = strdup(host);
    new_service->type       = strdup(type);
    new_service->interface  = strdup(interface);

    if (ipver == IP_V4) {
        inet_pton(AF_INET, addr, &new_service->ip4address);
    } else if (ipver == IP_V6) {
        inet_pton(AF_INET6, addr, &new_service->ip6address);
    }

    new_service->next = node.config.network.mDNSserviceList;
    node.config.network.mDNSserviceList = new_service;

    if (updateInfluxDB) {
        node.config.influxDB.serviceInfo = *new_service;
        node.config.influxDB.cfgVersion++;
    }

    pthread_mutex_unlock(&node.lock);  
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Remove from service list
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void remove_service(const char *name, const char *host, const char *type) {
    pthread_mutex_lock(&node.lock);

    ServiceInfo **curr = &node.config.network.mDNSserviceList;

    while (*curr) {
        if (SAFE_STRCMP((*curr)->name, name) == 0) {
            ServiceInfo *to_delete = *curr;
            *curr = (*curr)->next;
            free_service_node(to_delete);
            if (update_influxDB_service(type)){
                node.config.influxDB.serviceInfo.ip4address = 0;  
                memset(&node.config.influxDB.serviceInfo.ip6address, 0, sizeof(struct in6_addr));
            }            
        } else {
            curr = &((*curr)->next);
        }
    }

    pthread_mutex_unlock(&node.lock);
}
