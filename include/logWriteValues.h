#ifndef LOG_WRITE_VALUES_H
#define LOG_WRITE_VALUES_H

int log_write_values(bool influxInterval, bool fileInterval, InfluxCurl *influxCurl, LogBufferInfo *bufferInfo);

#endif