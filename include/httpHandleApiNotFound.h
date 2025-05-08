#ifndef HTTP_HANDLE_API_NOT_FOUND_H
#define HTTP_HANDLE_API_NOT_FOUND_H

#include <microhttpd.h>

int http_handle_api_not_found(struct MHD_Connection *connection, const char *url);

#endif