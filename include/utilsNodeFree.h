#ifndef UTILS_NODE_FREE_H
#define UTILS_NODE_FREE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdbool.h>
#include "node.h"

void free_config();
void free_network();
void free_service_node(ServiceInfo *node);
void free_service_list(bool mutex);

#endif