#ifndef HTTP_ROUTE_REQUEST_H
#define HTTP_ROUTE_REQUEST_H

#include <microhttpd.h>

int http_route_request(const char *url, struct MHD_Connection *connection);

#endif