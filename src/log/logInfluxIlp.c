
#include <stdio.h>
#include "../../include/logInfluxIlp.h"
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

    InfluxDB Line Protocol (ILP)

    https://docs.influxdata.com/influxdb/v1.8/write_protocols/line_protocol_tutorials/
    https://docs.influxdata.com/influxdb/v2.0/reference/syntax/line-protocol/

|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

//------------------------------------------------------------------
// Add ILP line to buffer (Generic)
//------------------------------------------------------------------
bool add_ilp_line_to_buffer(char *influx_buffer, size_t *influx_buffer_size,
    size_t buffer_capacity, const char *ilp_line
) {
    int written = snprintf(influx_buffer + *influx_buffer_size,
        buffer_capacity - *influx_buffer_size,
        "%s\n", ilp_line);

    if (written < 0 || (size_t)written >= buffer_capacity - *influx_buffer_size)
        return false;

    *influx_buffer_size += written;
    return true;
}

//------------------------------------------------------------------
// ILP formaters for MPU6050
//------------------------------------------------------------------
void format_accel_ilp(char *buffer, size_t buffer_size, LogBufferInfo *bufferInfo, MPU *mpu) {
    snprintf(buffer, buffer_size,
        "accel,device=MPU6050-%s "
        "x_raw=%d,y_raw=%d,z_raw=%d,"
        "x_g=%.5f,y_g=%.5f,z_g=%.5f,"
        "x_ms2=%.5f,y_ms2=%.5f,z_ms2=%.5f "
        "%lld",
        bufferInfo->deviceId,
        mpu->accel.raw.x, mpu->accel.raw.y, mpu->accel.raw.z,
        mpu->accel.g.x, mpu->accel.g.y, mpu->accel.g.z,
        mpu->accel.ms2.x, mpu->accel.ms2.y, mpu->accel.ms2.z,
        mpu->timestamp
    );
}

void format_gyro_ilp(char *buffer, size_t buffer_size, LogBufferInfo *bufferInfo, MPU *mpu) {
    snprintf(buffer, buffer_size,
        "gyro,device=MPU6050-%s "
        "x_raw=%d,y_raw=%d,z_raw=%d,"
        "x_dps=%.5f,y_dps=%.5f,z_dps=%.5f "
        "%lld",
        bufferInfo->deviceId,
        mpu->gyro.raw.x, mpu->gyro.raw.y, mpu->gyro.raw.z,
        mpu->gyro.dps.x, mpu->gyro.dps.y, mpu->gyro.dps.z,
        mpu->timestamp
    );
}

void add_mpu_ilp_values_to_buffer(MPU *mpu_samples, int mpu_samples_count, 
    LogBufferInfo *bufferInfo, char *ilp_line, size_t ilp_line_size,
    size_t *influx_buffer_size, size_t buffer_capacity, char *influx_buffer
) {
    for (int i = 0; i <= mpu_samples_count - 1; i++) {
        if (mpu_samples[i].timestamp > bufferInfo->lastTimestamp.mpu_influx) {
            // accelleration
            format_accel_ilp(ilp_line, ilp_line_size, bufferInfo, &mpu_samples[i]);
            add_ilp_line_to_buffer(influx_buffer, influx_buffer_size, buffer_capacity, ilp_line);
            // gyro
            format_gyro_ilp(ilp_line, ilp_line_size, bufferInfo, &mpu_samples[i]);
            add_ilp_line_to_buffer(influx_buffer, influx_buffer_size, buffer_capacity, ilp_line);
        }
    }
}

