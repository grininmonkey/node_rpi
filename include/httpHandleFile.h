#ifndef HTTP_HANDLE_FILE_H
#define HTTP_HANDLE_FILE_H

#include <microhttpd.h>

int http_handle_File(struct MHD_Connection *connection, char *filename, char *contentType);

#endif