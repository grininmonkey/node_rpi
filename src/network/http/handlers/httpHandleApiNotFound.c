//#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "../../../../include/httpResponse.h"
#include "../../../../include/utilsGetTimeStamp.h"
#include "../../../../include/httpHandleApiNotFound.h"

int http_handle_api_not_found(struct MHD_Connection *connection,const char *url) {
    
    json_t *root = json_object();
    json_object_set_new(root, "status", json_integer(404));
    json_object_set_new(root, "error", json_string("Not Found"));
    json_object_set_new(root, "timestamp", json_string(get_timestamp()));

    char message[256]; 
    snprintf(message, sizeof(message), "%s was not found", url);
    json_object_set_new(root, "message", json_string(message));
    
    char *json = json_dumps(root, JSON_COMPACT);
    json_decref(root);

    struct MHD_Response *response = MHD_create_response_from_buffer(
        strlen(json), json, MHD_RESPMEM_MUST_FREE
    );

    return send_json_response(connection, response, MHD_HTTP_NOT_FOUND);

}
