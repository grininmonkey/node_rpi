#include <stdio.h>
#include <string.h>
#include "../../../include/httpResponse.h"
#include "../../../include/utilsStrReplace.h"
#include "../../../include/httpQueueErrorResponse.h"

int http_queue_error_response(struct MHD_Connection *connection, char *error_msg, unsigned int status_code) {

    struct MHD_Response *response;

    //----------------------------------------------------------
    // html Template
    //----------------------------------------------------------
    const char *html =   "<!DOCTYPE html>"
        "<html lang='en'>"
        "<head>"
        "  <meta charset='UTF-8' />"
        "  <meta name='viewport' content='width=device-width, initial-scale=1.0' />"
        "  <title>Error!</title>"
        "  <style>body{display:flex;flex-direction:column;align-items:center;justify-content:center;min-height:100vh;font-family:sans-serif;margin:0}.svg-container{position:relative;width:200px;height:200px}svg{width:100%;height:100%;display:block}.overlay-number{position:absolute;top:55%;left:45%;transform:translate(-50%,-50%);font-size:52px;font-weight:bold;color:#fff;pointer-events:none}.caption{margin-top:16px;margin-bottom:25px;font-size:18px;color:#888;}</style>"
        "</head>"
        "<body>"
        "  <div class='svg-container'>"
        "    <div class='overlay-number'>{{STATUS_CODE}}</div>"
        "    <svg fill='#282828' version='1.1' id='Capa_1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' viewBox='0 0 453.415 453.416' preserveAspectRatio='xMidYMid meet' style='width:100%;height:auto;' xml:space='preserve'><g><g><path d='M344.373,133.15c-3.971,0-7.201,3.23-7.201,7.202s3.23,7.203,7.201,7.203c3.973,0,7.203-3.231,7.203-7.203S348.346,133.15,344.373,133.15z'/><path d='M337.828,87.162v35.733c0,3.608,2.938,6.545,6.545,6.545c3.611,0,6.547-2.937,6.547-6.545V87.162c0-3.609-2.936-6.545-6.547-6.545C340.766,80.617,337.828,83.553,337.828,87.162z'/><path d='M451.096,167.386L359.346,9.978c-3.053-5.237-8.657-8.457-14.719-8.457c-6.061,0-11.666,3.22-14.717,8.457l-42.867,73.541H13.25C5.93,83.519,0,89.454,0,96.771v242.48c0,7.32,5.93,13.252,13.25,13.252h202.672v86.137c0,5.359,3.229,10.193,8.182,12.244c1.639,0.68,3.359,1.01,5.066,1.01c3.447,0,6.84-1.348,9.373-3.883l96.172-95.508h56.976c7.319,0,13.254-5.932,13.254-13.252v-146.25h31.436c6.098,0,11.732-3.26,14.771-8.549C454.189,179.164,454.168,172.655,451.096,167.386z M282.525,158.929l62.102-106.542l62.102,106.542H282.525z'/></g></g></svg>"
        "  </div>"
        "  <div class='caption'>{{ERROR_MSG}}</div>"
        "</body>"
        "</html>";

    //----------------------------------------------------------
    // Replace Vars in html with values
    //----------------------------------------------------------
    char status_str[12];
    snprintf(status_str, sizeof(status_str), "%u", status_code);

    char *rendered = str_replace_pairs(html, 2, 
        "{{STATUS_CODE}}", status_str,
        "{{ERROR_MSG}}", error_msg
    );

    //----------------------------------------------------------
    // Set/Send response
    //----------------------------------------------------------
    if (!rendered) {
        status_code = 500;
        const char *msg = "Template rendering failed!";
        response = MHD_create_response_from_buffer(
            strlen(msg), (void *)msg, MHD_RESPMEM_PERSISTENT
        );        
    } else {
        response = MHD_create_response_from_buffer(
            strlen(rendered), rendered, MHD_RESPMEM_MUST_FREE
        );        
    }

    return send_html_response(connection, response, status_code);


}