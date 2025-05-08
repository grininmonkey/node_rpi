#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../../include/node.h"
#include "../../include/logBuffers.h"
#include "../../include/logInfluxIlp.h"  
#include "../../include/logBufferBase.h"    
#include "../../include/logInfluxCurl.h"
#include "../../include/utilsWriteToFile.h"
#include "../../include/utilsVerbosePrint.h"
#include "../../include/logSendBufferToInflux.h"
#include "../../include/utilsGetCurrentTimestamp.h"

int log_write_values(bool influxInterval, bool fileInterval, InfluxCurl *influxCurl, LogBufferInfo *bufferInfo) {
    //-----------------------------------------------------------------
    // Local variables
    //-----------------------------------------------------------------
    char ilp_line[512];
    bool logToInflux    = false;
    bool logToFile      = false;

    //-----------------------------------------------------------------
    // Mutex lock assign to local variables
    //-----------------------------------------------------------------
    pthread_mutex_lock(&node.lock);
    logToFile       = node.config.logValuesToFile;
    logToInflux     = node.config.logValuesToInflux;
    log_update_influx_curl(logToInflux, influxCurl, &node.config.influxDB);
    pthread_mutex_unlock(&node.lock);

    //-----------------------------------------------------------------
    // Get Buffer(s)
    //-----------------------------------------------------------------
    MPU mpu_samples[bufferInfo->size.mpu];
    int mpu_samples_count = log_buffer_peek_all(logBufferMPU, mpu_samples, sizeof(MPU), bufferInfo->size.mpu);

    //... others soon™

    //-----------------------------------------------------------------
    // Clear Buffer File(s) if the interval is reached
    //-----------------------------------------------------------------
    if (logToFile && fileInterval) {
        write_to_file(bufferInfo->filename.mpu, "", "w");
        //... others soon™
    }

    //-----------------------------------------------------------------
    // Influx 
    //-----------------------------------------------------------------
    if (influxInterval && logToInflux && influxCurl->ready) {
        char influx_buffer[(2 * mpu_samples_count) * 512];
        size_t influx_buffer_size = 0;

        add_mpu_ilp_values_to_buffer(mpu_samples, mpu_samples_count, bufferInfo,
            ilp_line, sizeof(ilp_line), &influx_buffer_size, sizeof(influx_buffer), influx_buffer);

        if (influx_buffer_size > 0 )
            log_send_buffer_to_influx(influx_buffer, influx_buffer_size, influxCurl);

        bufferInfo->lastTimestamp.mpu_influx = mpu_samples[mpu_samples_count - 1].timestamp;
    }

    //-----------------------------------------------------------------
    // File
    //-----------------------------------------------------------------
    if (logToFile && mpu_samples_count > 0) {
        for (int i = mpu_samples_count - 1; i >= 0; i--) {
            if (mpu_samples[i].timestamp <= bufferInfo->lastTimestamp.mpu_file) break;
            format_accel_ilp(ilp_line, sizeof(ilp_line), bufferInfo, &mpu_samples[i]);
            write_to_file(bufferInfo->filename.mpu, ilp_line, "a"); 
            //... others soon™
        }
        bufferInfo->lastTimestamp.mpu_file = mpu_samples[mpu_samples_count - 1].timestamp;
    }
    

    return 1;

}