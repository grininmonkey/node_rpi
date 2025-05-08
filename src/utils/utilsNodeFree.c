
#include "../include/utilsNodeFree.h"


void free_network() {
    SAFE_FREE(node.config.network.httpmDNSserviceType);
    SAFE_FREE(node.config.network.mqBrokermDNSserviceType);
}

void free_influxDB() {
    SAFE_FREE(node.config.influxDB.org);
    SAFE_FREE(node.config.influxDB.token);
    SAFE_FREE(node.config.influxDB.username);
    SAFE_FREE(node.config.influxDB.password);
    SAFE_FREE(node.config.influxDB.precision);
    SAFE_FREE(node.config.influxDB.dbOrBucket);
    SAFE_FREE(node.config.influxDB.mDNSserviceType);
}

void free_config() {
    SAFE_FREE(node.config.id);
    SAFE_FREE(node.config.clusterId);
    SAFE_FREE(node.config.servePath);
    SAFE_FREE(node.config.configFile);
    SAFE_FREE(node.config.i2cBusPath);
    SAFE_FREE(node.config.dataFolderName);
    free_influxDB();
    free_network();
}

void free_service_node(ServiceInfo *node) {
    if (!node) return;
    SAFE_FREE(node->name);
    SAFE_FREE(node->host);
    SAFE_FREE(node->type);
    SAFE_FREE(node->interface);
    SAFE_FREE(node);
}

void free_service_list(bool mutex) {
    if (mutex) pthread_mutex_lock(&node.lock);
    
    ServiceInfo *current = node.config.network.mDNSserviceList;
    while (current) {
        ServiceInfo *next = current->next;
        free_service_node(current);
        current = next;
    }
    node.config.network.mDNSserviceList = NULL;

    if (mutex) pthread_mutex_unlock(&node.lock);
}