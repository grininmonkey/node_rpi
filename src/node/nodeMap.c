#include "../../include/nodeMap.h"

//----------------------------------------------------------
// Hash
//----------------------------------------------------------
unsigned hash(const char *str) {
    unsigned h = 5381;
    while (*str) h = ((h << 5) + h) + (unsigned char)(*str++);
    return h % MAP_SIZE;
}

//----------------------------------------------------------
// Initialize PointerMap
//----------------------------------------------------------
PointerMap *map_create() {
    return calloc(1, sizeof(PointerMap));
}
//----------------------------------------------------------
// Add entry to PointerMap
//----------------------------------------------------------
void map_add(PointerMap *map, const char *key, void *ptr, ValueType type, bool canUpdate) {
    unsigned h = hash(key);
    MapEntry *entry = malloc(sizeof(MapEntry));
    entry->key = strdup(key);
    entry->ptr = ptr;
    entry->canUpdate = canUpdate;
    entry->type = type;
    entry->next = map->buckets[h];
    map->buckets[h] = entry;
}

//----------------------------------------------------------
// Get stored pointer / value with type check
//----------------------------------------------------------
void *map_get(PointerMap *map, const char *key, ValueType expected_type) {
    unsigned h = hash(key);
    for (MapEntry *e = map->buckets[h]; e; e = e->next) {
        if (strcmp(e->key, key) == 0) {
            if (e->type == expected_type) {
                return e->ptr;
            } else {
                fprintf(stderr, "Type mismatch for key '%s'\n", key);
                return NULL;
            }
        }
    }
    return NULL;
}

//----------------------------------------------------------
// Get PointerMap Entry (full struct)
//----------------------------------------------------------
void *map_get_entry(PointerMap *map, const char *key) {
    unsigned h = hash(key);
    MapEntry *entry = map->buckets[h];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry;  
        }
        entry = entry->next;
    }
    return NULL;  
}

//----------------------------------------------------------
// Clean Up Function
//----------------------------------------------------------
void map_destroy(PointerMap *map) {
    for (int i = 0; i < MAP_SIZE; ++i) {
        MapEntry *e = map->buckets[i];
        while (e) {
            MapEntry *next = e->next;
            free(e->key);
            free(e);
            e = next;
        }
    }
    free(map);
}

