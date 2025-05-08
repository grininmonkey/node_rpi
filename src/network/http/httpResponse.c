
#include "../../../include/httpResponse.h"


int send_json_response(struct MHD_Connection *connection, struct MHD_Response *response, unsigned int status_code) {
    MHD_add_response_header(response, "Content-Type", "application/json; charset=utf-8");
    MHD_add_response_header(response, "Content-Disposition", "inline");
    MHD_add_response_header(response, "X-Content-Type-Options", "nosniff");

    return send_response(connection, response, status_code);
}

int send_html_response(struct MHD_Connection *connection, struct MHD_Response *response, unsigned int status_code) {
    MHD_add_response_header(response, "Content-Type", "text/html; charset=utf-8");
    return send_response(connection, response, status_code);
}

int send_response(struct MHD_Connection *connection, struct MHD_Response *response, unsigned int status_code) {

    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
    MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");

    int ret = MHD_queue_response(connection, status_code, response);
    MHD_destroy_response(response);
    return ret;
}

