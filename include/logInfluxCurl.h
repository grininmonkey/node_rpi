#ifndef LOG_INFLUX_CURL_H
#define LOG_INFLUX_CURL_H

#include "node.h"

void log_free_influx_curl(InfluxCurl *influxCurl);
void log_update_influx_curl(InfluxCurl *influxCurl, InfluxDB *config);

#endif