#include <string.h>
#include "../../../../include/node.h"
#include "../../../../include/httpResponse.h"
#include "../../../../include/httpHandleTest.h"

int http_handle_test(struct MHD_Connection *connection) {
    //-----------------------------------------------------------------
    // 
    //-----------------------------------------------------------------
    pthread_mutex_lock(&node.lock);
        
        if (!node.config.influxDB.freshConnect) {
            node.config.influxDB.freshConnect = true;
            node.config.influxDB.forbidReuse = true;
        } else {
            node.config.influxDB.freshConnect = false;
            node.config.influxDB.forbidReuse = false;
        }
        node.config.influxDB.cfgVersion++;
    pthread_mutex_unlock(&node.lock);


    char *html =   "<!DOCTYPE html>"
    "<html lang='en'>"
    "<head>"
    "  <meta charset='UTF-8' />"
    "  <meta name='viewport' content='width=device-width, initial-scale=1.0' />"
    "  <title>Test</title>"
    "</head>"
    "<body>"
    "  <div>Test Influx Curl Reset</div>"
    "</body>"
    "</html>";    

    //------------------------------------------------------------------------
    // Create response
    //------------------------------------------------------------------------
    struct MHD_Response *response = MHD_create_response_from_buffer(
        strlen(html), html, MHD_RESPMEM_PERSISTENT
    );

    //----------------------------------------------------------
    // Send Response
    //----------------------------------------------------------
    return send_html_response(connection, response, MHD_HTTP_OK);

}