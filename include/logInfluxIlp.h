#ifndef LOG_INFLUX_ILP_H
#define LOG_INFLUX_ILP_H

#include "node.h"
#include "logBuffers.h"

void format_accel_ilp(char *buffer, size_t buffer_size, LogBufferInfo *bufferInfo, MPU *mpu);
void format_gyro_ilp(char *buffer, size_t buffer_size, LogBufferInfo *bufferInfo, MPU *mpu);
void add_mpu_ilp_values_to_buffer(MPU *mpu_samples, int mpu_samples_count, LogBufferInfo *bufferInfo,
    char *ilp_line, size_t ilp_line_size, size_t *influx_buffer_size, size_t buffer_capacity, char *influx_buffer);
bool add_ilp_line_to_buffer(char *influx_buffer, size_t *influx_buffer_size,
    size_t buffer_capacity, const char *ilp_line);

#endif