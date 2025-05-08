
#include "../../include/utilsIP.h"
#include "../../include/utilsBuildJsonFromMap.h"

//--------------------------------------------------------------------------
// Get or create a JSON object for the given key in the parent object
//--------------------------------------------------------------------------
static json_t *get_or_create_object(json_t *parent, const char *key) {
    json_t *obj = json_object_get(parent, key);
    if (!obj || !json_is_object(obj)) {
        obj = json_object();
        json_object_set_new(parent, key, obj);
    }
    return obj;
}

//--------------------------------------------------------------------------
// Add ServiceList to JSON object
//--------------------------------------------------------------------------
void add_service_list_to_json(json_t *root, const char *key, void *ptr) {
    ServiceInfo *service_list = *(ServiceInfo **)ptr;
    json_t *service_array = json_array();  

    ServiceInfo *current_service = service_list;
    while (current_service != NULL) {
        json_t *service_obj = json_object();

        // Add fields to the service JSON object
        json_object_set_new(service_obj, "name", json_string(current_service->name ? current_service->name : ""));
        json_object_set_new(service_obj, "host", json_string(current_service->host ? current_service->host : ""));
        json_object_set_new(service_obj, "type", json_string(current_service->type ? current_service->type : ""));
        json_object_set_new(service_obj, "interface", json_string(current_service->interface ? current_service->interface : ""));
        json_object_set_new(service_obj, "port", json_integer(current_service->port));
        
        // Convert IPv4 and IPv6 addresses to string representations
        char ip_buffer[INET6_ADDRSTRLEN];
        if (current_service->ip4address != 0) {
            ip_to_string(AF_INET, &current_service->ip4address, ip_buffer, sizeof(ip_buffer));
            json_object_set_new(service_obj, "ip4address", json_string(ip_buffer));
        } else {
            ip_to_string(AF_INET6, &current_service->ip6address, ip_buffer, sizeof(ip_buffer));
            json_object_set_new(service_obj, "ip6address", json_string(ip_buffer));
        }

        // Add this service object to the service array
        json_array_append_new(service_array, service_obj);

        // Move to the next service in the list
        current_service = current_service->next;
    }

    json_object_set_new(root, key, service_array);
 
}
//--------------------------------------------------------------------------
// Recursively insert value based on key path
//--------------------------------------------------------------------------
void add_path_to_json(json_t *root, const char *key, const char *required_prefix, void *ptr, ValueType type) {
    size_t prefix_len = strlen(required_prefix);

    // Only proceed if the key starts with the required prefix
    if (strncmp(key, required_prefix, prefix_len) != 0)
        return;

    // Skip prefix and optional dot
    const char *stripped_key = key + prefix_len;
    if (*stripped_key == '.')
        stripped_key++;

    char *temp = strdup(stripped_key);
    if (!temp) return;

    char *tok = strtok(temp, ".");
    json_t *curr = root;

    char *next_tok;
    while ((next_tok = strtok(NULL, ".")) != NULL) {
        curr = get_or_create_object(curr, tok);
        tok = next_tok;
    }

    // Leaf key
    if (tok) {
        switch (type) {
            case TYPE_BOOL:
                json_object_set_new(curr, tok, json_boolean(*(bool *)ptr));
                break;
            case TYPE_INT:
                json_object_set_new(curr, tok, json_integer(*(int *)ptr));
                break;
            case TYPE_UINT8:
                json_object_set_new(curr, tok, json_integer(*(uint8_t *)ptr));  
                break;                
            case TYPE_UINT16:
                json_object_set_new(curr, tok, json_integer(*(uint16_t *)ptr));
                break;                
            case TYPE_FLOAT:
                json_object_set_new(curr, tok, json_real(*(float *)ptr));
                break;
            case TYPE_DOUBLE:
                json_object_set_new(curr, tok, json_real(*(double *)ptr));
                break;
            case TYPE_STRING:
                json_object_set_new(curr, tok, json_string(*(char **)ptr));
                break;
            case TYPE_SERVICE_LIST:
                add_service_list_to_json(curr, tok, ptr);
                break;
            default:
                json_object_set_new(curr, tok, json_string("<unsupported>"));
        }
    }

    free(temp);
}


//--------------------------------------------------------------------------
// Main function to build JSON from map
//--------------------------------------------------------------------------
json_t *build_json_from_map(PointerMap *map) {
    json_t *root = json_object();
    json_t *config = json_object();
    json_object_set_new(root, "config", config);

    for (int i = 0; i < MAP_SIZE; i++) {
        MapEntry *entry = map->buckets[i];
        while (entry) {
            add_path_to_json(config, entry->key, "node", entry->ptr, entry->type);
            entry = entry->next;
        }
    }

    return root;
}