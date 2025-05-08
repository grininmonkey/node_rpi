#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <microhttpd.h>

int send_json_response(struct MHD_Connection *connection, struct MHD_Response *response, unsigned int status_code);
int send_html_response(struct MHD_Connection *connection, struct MHD_Response *response, unsigned int status_code);
int send_response(struct MHD_Connection *connection, struct MHD_Response *response, unsigned int status_code);

#endif