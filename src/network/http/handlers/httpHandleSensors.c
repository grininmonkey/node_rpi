#include <string.h>
#include <jansson.h>
#include "../../../../include/node.h"
#include "../../../../include/httpResponse.h"
#include "../../../../include/httpHandleSensors.h"
#include "../../../../include/utilsGetTimeStamp.h"
#include "../../../../include/utilsBuildJsonFromMap.h"

int http_handle_sensors(struct MHD_Connection *connection) {
    //------------------------------------------------------------------------
    // Set prefix
    //------------------------------------------------------------------------
    char sensor_path[256]; 
    const char *sensor_argument = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "sensor");
    if (sensor_argument) {
        snprintf(sensor_path, sizeof(sensor_path), "sensor.%s", sensor_argument);
    } else {
        snprintf(sensor_path, sizeof(sensor_path), "sensor");
    }

    //------------------------------------------------------------------------
    // Create json
    //------------------------------------------------------------------------
    json_t *root = json_object();
    json_object_set_new(root, "status", json_string("OK"));
    json_object_set_new(root, "timestamp", json_string(get_timestamp()));

    //------------------------------------------------------------------------        
    // Accessing mutex data
    //------------------------------------------------------------------------
    pthread_mutex_lock(&node.lock);
    
    json_t *sensor = json_object();
    json_object_set_new(root, sensor_path, sensor);

    for (int i = 0; i < MAP_SIZE; i++) {
        MapEntry *entry = node_map->buckets[i];
        while (entry) {
            add_path_to_json(sensor, entry->key, sensor_path, entry->ptr, entry->type);
            entry = entry->next;
        }
    }

    pthread_mutex_unlock(&node.lock);
    //------------------------------------------------------------------------
    // Complete json
    //------------------------------------------------------------------------
    char *json = json_dumps(root, JSON_COMPACT);
    json_decref(root);    

    //------------------------------------------------------------------------
    // Create response
    //------------------------------------------------------------------------
    struct MHD_Response *response = MHD_create_response_from_buffer(
        strlen(json), json, MHD_RESPMEM_MUST_FREE
    );

    //------------------------------------------------------------------------
    // Send Response
    //------------------------------------------------------------------------
    return send_json_response(connection, response, MHD_HTTP_OK);
    

}