#ifndef MDNS_PUBLISH_SERVICE_H
#define MDSN_PUBLISH_SERVICE_H

#include <stdint.h>
#include <sys/types.h>

int mdns_publish_service(uint16_t port, pid_t t_pid, char *service_name, char *service_type);
void stop_mdns_service();

#endif 