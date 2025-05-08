#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../../../include/httpHandleFile.h"
#include "../../../../include/httpQueueErrorResponse.h"
#include "../../../../include/utilsReadFile.h"
#include "../../../../include/utilsStrReplace.h"

int http_handle_File(struct MHD_Connection *connection, char *filename, char *contentType) {
    // -------------------------------------------------------------
    // Read the file
    // -------------------------------------------------------------
    char *file_contents = read_file(filename);
    if (!file_contents) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "Failed to load file: %s", filename);
        return http_queue_error_response(connection, error_msg, MHD_HTTP_INTERNAL_SERVER_ERROR);
    }

    // -------------------------------------------------------------
    // Testing  TODO: setup parameter var processing 
    // -------------------------------------------------------------
    char *final_output = str_replace(file_contents, "{{TEST_VAR1}}", "Yay... it worked");
    free(file_contents);  // original content no longer needed

    if (!final_output) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "Failed to process file: %s", filename);
        return http_queue_error_response(connection, error_msg, MHD_HTTP_INTERNAL_SERVER_ERROR);
    }

    // -------------------------------------------------------------
    // Create response
    // -------------------------------------------------------------
    struct MHD_Response *response = MHD_create_response_from_buffer(
        strlen(final_output), (void *)final_output, MHD_RESPMEM_MUST_FREE
    );

    // -------------------------------------------------------------
    // Add Header(s)
    // -------------------------------------------------------------
    MHD_add_response_header(response, "Content-Type", contentType);
    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
    MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");
    
    // -------------------------------------------------------------
    // Add response to queue
    // -------------------------------------------------------------
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;

}