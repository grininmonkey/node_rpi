#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../../include/node.h"


size_t read_callback(char *ptr, size_t size, size_t nmemb, void *stream) {
    FILE *file = (FILE *)stream;
    return fread(ptr, size, nmemb, file);
}

void log_send_file_to_influx(const char *filename, InfluxCurl *influxCurl) {

    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    if (file_size <= 0) {
        fclose(file);    
        return;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL\n");
        fclose(file);
        return;
    }

    // Set up CURL
    curl_easy_setopt(curl, CURLOPT_URL, influxCurl->full_url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, file);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)file_size);

    // Set headers
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: text/plain");
    if (influxCurl->dbVersion == 2 && influxCurl->token) {
        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Token %s", influxCurl->token);
        headers = curl_slist_append(headers, auth_header);
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        FILE *clear = fopen(filename, "w");
        if (clear) fclose(clear);
        else perror("Failed to clear file");
    } else {
        fprintf(stderr, "CURL request failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    fclose(file);

}