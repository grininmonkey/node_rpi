//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/httpRouteRequest.h"
#include "../../../include/httpHandleFile.h"
#include "../../../include/httpHandleTest.h"
#include "../../../include/httpHandleStatus.h"  
#include "../../../include/httpHandleApiNotFound.h"  
#include "../../../include/httpQueueErrorResponse.h"

//
// TODO: 
//      base filename path as a define somewhere, or make
//      it a config variable?
//

int http_route_request(const char *url, struct MHD_Connection *connection) {
    //---------------------------------------------------------
    // (API/json) Handlers
    //---------------------------------------------------------
    if (strcmp(url, "/api/status") == 0)
        return http_handle_status(connection);


    if (strcmp(url, "/test") == 0)
        return http_handle_test(connection);

    //---------------------------------------------------------
    // (HTML/File) Handlers
    //---------------------------------------------------------
    if (strcmp(url, "/") == 0 || strcmp(url, "/index.html") == 0)
        return http_handle_File(connection,"/srv/rpiNode/html/index.html","text/html");
    
    //TODO: determine content types, add generic handle_file call based on url var etc...
    //     soon™

    //---------------------------------------------------------
    // Handle /api? Not Found (json)
    //---------------------------------------------------------
    if (strncmp(url, "/api", 4) == 0)
        return http_handle_api_not_found(connection, url);
     
    //---------------------------------------------------------
    // Handle /? Not Found (html)
    //---------------------------------------------------------
    char error_msg[256];
    snprintf(error_msg, sizeof(error_msg), "%s&nbsp;:&nbsp;Not Found", url);
    return http_queue_error_response(connection, error_msg, MHD_HTTP_NOT_FOUND);

}