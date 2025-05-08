
#include "../../include/nodeMap.h"

unsigned hash(const char *str) {
    unsigned h = 5381;
    while (*str) h = ((h << 5) + h) + (unsigned char)(*str++);
    return h % MAP_SIZE;
}

PointerMap* map_create() {
    return calloc(1, sizeof(PointerMap));
}

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


/*

example usage:

MapEntry *entry = map_get(node_map, "node.config.network.mDNSserviceList");
if (entry && entry->type == TYPE_SERVICE_LIST) {
    ServiceNode *list = *(ServiceNode **)entry->ptr;  // Dereference pointer to list
    while (list != NULL) {
        // Use the list node's data (e.g., list->service->service_name)
        printf("Service: %s, Port: %d\n", list->service->service_name, list->service->port);
        list = list->next;
    }
}

---

    static char buffer[1024];  // Assume the rendered result fits here

    MapEntry *entry = map_get(map, key);
    if (entry) {
        const char *replacement = "(null)";
        char value_buf[64];

        // Handle value based on type
        switch (entry->type) {
            case TYPE_STRING:
                replacement = *(char **)entry->ptr ?: "(null)";
                break;
            case TYPE_INT:
                snprintf(value_buf, sizeof(value_buf), "%d", *(int *)entry->ptr);
                replacement = value_buf;
                break;
            case TYPE_FLOAT:
                snprintf(value_buf, sizeof(value_buf), "%.2f", *(float *)entry->ptr);
                replacement = value_buf;
                break;
            default:
                replacement = "(unsupported)";
                break;
        }

        // Copy the replacement into the buffer
        strcpy(output, replacement);
        output += strlen(replacement);
    }

---

int main() {
    MyStruct s = { .a = 5, .b = 1.23f, .c = 3.14159 };

    PointerMap *map = map_create();
    map_add(map, "s->a", &s.a, TYPE_INT);
    map_add(map, "s->b", &s.b, TYPE_FLOAT);
    map_add(map, "s->c", &s.c, TYPE_DOUBLE);

    int *a = map_get(map, "s->a", TYPE_INT);
    float *b = map_get(map, "s->b", TYPE_FLOAT);
    double *c = map_get(map, "s->c", TYPE_DOUBLE);
    char **s = map_get(node_map, "node.config.id", TYPE_STRING);

    if (s && *s) printf("test: %s\n", *s);
    if (a) printf("a = %d\n", *a);
    if (b) printf("b = %.2f\n", *b);
    if (c) printf("c = %.5f\n", *c);

    map_destroy(map);
    return 0;
}
*/