#ifndef LOG_SEND_BUFFER_TO_INFLUX_H
#define LOG_SEND_BUFFER_TO_INFLUX_H

int log_send_buffer_to_influx(const char *buffer, int buffer_size, InfluxCurl *influxCurl);

#endif