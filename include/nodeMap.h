#ifndef NODE_MAP_H
#define NODE_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

unsigned hash(const char *str);
PointerMap *map_create();
void map_add(PointerMap *map, const char *key, void *ptr, ValueType type);
void *map_get(PointerMap *map, const char *key, ValueType expected_type);
void *map_get_entry(PointerMap *map, const char *key);
void map_destroy(PointerMap *map);

#endif