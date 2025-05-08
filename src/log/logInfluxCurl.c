
#include "../../include/node.h"
#include "../../include/utilsIP.h"
#include "../../include/utilsSleepMs.h"
#include "../../include/utilsVerbosePrint.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Free Influx Curl configuration
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void log_free_influx_curl(InfluxCurl *influxCurl) {
    if (influxCurl->curl) {
        curl_easy_cleanup(influxCurl->curl);
        influxCurl->curl = NULL;
    }
    if (influxCurl->headers) {
        curl_slist_free_all(influxCurl->headers);
        influxCurl->headers = NULL;
    }
    SAFE_FREE(influxCurl->full_url);
    SAFE_FREE(influxCurl->token);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Update Influx Curl configuration
// This function is called when the InfluxDB configuration changes.
// It updates the curl configuration and URL based on the new settings.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void log_update_influx_curl(InfluxCurl *influxCurl, InfluxDB *config) {
    if (influxCurl->cfgVersion == config->cfgVersion)
        return;

    influxCurl->cfgVersion = config->cfgVersion;
    verbose_print("[LOG][%d] InfluxDB -> Updating Curl configuration version(%i)\n", influxCurl->pid, config->cfgVersion);

    if (influxCurl->curl) {
        curl_easy_cleanup(influxCurl->curl);
        influxCurl->curl = NULL;
    }
    if (influxCurl->headers) {
        curl_slist_free_all(influxCurl->headers);
        influxCurl->headers = NULL;
    }
    
    sleep_ms(100); // troubleshooting for reset

    influxCurl->curl = curl_easy_init();
    if (!influxCurl->curl) {
        influxCurl->ready = false;
        return;
    }

    influxCurl->headers = curl_slist_append(NULL, "Content-Type: text/plain");
    if (config->dbVersion == 2 && config->token) {
        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Token %s", config->token);
        influxCurl->headers = curl_slist_append(influxCurl->headers, auth_header);
    }

    bool ip4  = config->serviceInfo.ip4address > 0;
    bool ip6  = is_ipv6_addr_set(&config->serviceInfo.ip6address);
    bool port = config->serviceInfo.port > 0;

    if (!(port && (ip4 || ip6)))
        return;

    char address[256];
    char full_url[512];
    char buffer[INET6_ADDRSTRLEN];

    const char *preAdr = config->https ? "https://" : "http://";
    const char *result = NULL;

    if (ip6) {
        bool isLL = is_ipv6_link_local(&config->serviceInfo.ip6address);
        result = ip_to_string(AF_INET6, &config->serviceInfo.ip6address, buffer, sizeof(buffer));
        if (result) {
            if (isLL && config->serviceInfo.interface) {
                snprintf(address, sizeof(address), "%s[%s%%25%s]:%u",
                         preAdr, buffer, config->serviceInfo.interface, config->serviceInfo.port);
            } else {
                snprintf(address, sizeof(address), "%s[%s]:%u",
                         preAdr, buffer, config->serviceInfo.port);
            }
        }
    } else {
        result = ip_to_string(AF_INET, &config->serviceInfo.ip4address, buffer, INET_ADDRSTRLEN);
        if (result) {
            snprintf(address, sizeof(address), "%s%s:%u",
                     preAdr, buffer, config->serviceInfo.port);
        }
    }

    if (!result)
        return;

    influxCurl->ready = true;

    if (config->dbVersion == 1) {
        snprintf(full_url, sizeof(full_url), "%s/write?db=%s&precision=%s",
                 address, config->dbOrBucket, config->precision);
    } else if (config->dbVersion == 2) {
        snprintf(full_url, sizeof(full_url),
                 "%s/api/v2/write?bucket=%s&org=%s&precision=%s",
                 address, config->dbOrBucket, config->org, config->precision);
    } else {
        verbose_print("[LOG][%d] InfluxDB -> Unsupported version: %d\n", influxCurl->pid, config->dbVersion);
        influxCurl->ready = false;
        return;
    }

    if (full_url[0] == '\0') {
        influxCurl->ready = false;
        return;
    }

    STRDUP_REPLACE(influxCurl->full_url, full_url);
    STRDUP_REPLACE_NULLABLE(influxCurl->token, config->token);

    curl_easy_setopt(influxCurl->curl, CURLOPT_URL, influxCurl->full_url);
    curl_easy_setopt(influxCurl->curl, CURLOPT_HTTPHEADER, influxCurl->headers);
    if (!config->forbidReuse && !config->freshConnect) {
        curl_easy_setopt(influxCurl->curl, CURLOPT_TCP_KEEPALIVE, 1L);
    }    
    curl_easy_setopt(influxCurl->curl, CURLOPT_FORBID_REUSE, config->forbidReuse ? 1L : 0L);
    curl_easy_setopt(influxCurl->curl, CURLOPT_FRESH_CONNECT, config->freshConnect ? 1L : 0L);
    curl_easy_setopt(influxCurl->curl, CURLOPT_TIMEOUT_MS, config->totalTimeoutMs > 0 ? config->totalTimeoutMs : 4000L);
    curl_easy_setopt(influxCurl->curl, CURLOPT_CONNECTTIMEOUT_MS, config->connectTimeoutMs > 0 ? config->connectTimeoutMs : 1000L);


    influxCurl->dbVersion = config->dbVersion;

}
