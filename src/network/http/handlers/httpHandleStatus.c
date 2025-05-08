#include <string.h>
#include <jansson.h>
#include "../../../../include/node.h"
#include "../../../../include/httpResponse.h"
#include "../../../../include/httpHandleStatus.h"
#include "../../../../include/utilsGetTimeStamp.h"

int http_handle_status(struct MHD_Connection *connection) {
    //------------------------------------------------------------------------
    // Create json
    //------------------------------------------------------------------------
    json_t *root = json_object();
    json_object_set_new(root, "status", json_string("OK"));
    json_object_set_new(root, "timestamp", json_string(get_timestamp()));

    //------------------------------------------------------------------------
    // Accessing mutex data
    //------------------------------------------------------------------------
    /*
    pthread_mutex_lock(&rpiNode.lock);
    
    json_object_set_new(root, "id", json_string(rpiNode.config.id));
    json_object_set_new(root, "clusterID", json_string(rpiNode.config.clusterID));
    json_object_set_new(root, "http", json_boolean(rpiNode.config.http));
    json_object_set_new(root, "mDNS", json_boolean(rpiNode.config.mDNS));
    json_object_set_new(root, "master", json_boolean(rpiNode.config.master));
    json_object_set_new(root, "useTmpfs", json_boolean(rpiNode.config.useTmpfs));
    json_object_set_new(root, "saveToDB", json_boolean(rpiNode.config.saveToDB));
    json_object_set_new(root, "broadcast", json_boolean(rpiNode.config.broadcast));
    json_object_set_new(root, "DBcreated", json_boolean(rpiNode.config.DBcreated));
    json_object_set_new(root, "httpPort", json_integer(rpiNode.config.httpPort));
    json_object_set_new(root, "tmpfsSizeM", json_integer(rpiNode.config.tmpfsSize));
    json_object_set_new(root, "broadcastPort", json_integer(rpiNode.config.broadcastPort));
    json_object_set_new(root, "updateDBSeconds", json_integer(rpiNode.config.updateDBSeconds));
    json_object_set_new(root, "broadcastIP", json_string(rpiNode.config.broadcastIP));
    json_object_set_new(root, "tmpfsFolderName", json_string(rpiNode.config.tmpfsFolderName));
    json_object_set_new(root, "currentValuesDB", json_string(rpiNode.config.currentValuesDB));
    json_object_set_new(root, "currentValuesDBName", json_string(rpiNode.config.currentValuesDBName));
    json_object_set_new(root, "currentValuesViewName", json_string(rpiNode.config.currentValuesViewName));

    json_t *name_value_array = json_array();  // Create an empty JSON array
    NameValue *current = rpiNode.internal_config;
    while (current != NULL) {
        json_t *obj = json_object();
        json_object_set_new(obj, "name", json_string(current->name));
        json_object_set_new(obj, "value", json_string(current->value));
        json_array_append_new(name_value_array, obj);  // Add object to array
        current = current->next;
    }
    
    pthread_mutex_unlock(&rpiNode.lock);
    //------------------------------------------------------------------------
    // Complete json
    //------------------------------------------------------------------------
    json_object_set_new(root, "internal_settings", name_value_array);
    //json_object_set_new(root, "internal", internal);
    */

    char *json = json_dumps(root, 0);
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