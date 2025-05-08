#ifndef UTILS_BUILD_JSON_FROM_MAP_H
#define UTILS_BUILD_JSON_FROM_MAP_H 

#include "node.h"
#include <jansson.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

json_t *build_json_from_map(PointerMap *map);
void add_path_to_json(json_t *root, const char *key, const char *required_prefix, void *ptr, ValueType type);


#endif