#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../../include/node.h"
#include "../../include/utilsVerbosePrint.h"


int log_send_buffer_to_influx(const char *buffer, int buffer_size, InfluxCurl *influxCurl) {

    int response_code = 0;

    CURL *curl = influxCurl->curl;
    if (!curl) {
        return 0;
    }

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, buffer_size);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        verbose_print("[LOG][%d] Influx push failed: %s\n", influxCurl->pid, curl_easy_strerror(res));
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    return response_code;
    
}

