#include <unistd.h>      
#include <sys/syscall.h> 
#include <stdbool.h>       
#include "../../include/node.h"
#include "../../include/logBuffers.h"
#include "../../include/utilsSleepMs.h"
#include "../../include/logInfluxCurl.h"
#include "../../include/utilsMakePath.h"
#include "../../include/logWriteValues.h"
#include "../../include/utilsVerbosePrint.h"
#include "../../include/utilsSignalHandler.h"
#include "../../include/utilsGetCurrentTimestamp.h"

void *log_values_thread(void *args) {
    //-----------------------------------------------------------------
    // Local variables
    //-----------------------------------------------------------------
    int log_frequency_ms          = 100;
    int mpu_frequency_ms          = 10;
    int influx_interval_seconds   = 1;
    bool influx_interval          = false;
    bool file_interval            = false;
    int influx_interval_ms        = 5000;
    int file_interval_ms          = 1000;
    InfluxCurl influxCurl         = {0};
    LogBufferInfo bufferInfo      = {0};

    //-----------------------------------------------------------------
    // Mutex lock -> assign variables
    //-----------------------------------------------------------------
    pthread_mutex_lock(&node.lock); 
    if (!node.config.logValues) {
        pthread_mutex_unlock(&node.lock); 
        return NULL;
    }
    node.pid.dbs            = syscall(SYS_gettid);
    influxCurl.pid          = node.pid.dbs;
    log_frequency_ms        = node.config.logMilliseconds;
    //mpu_frequency_ms        = ??? <soon™>;
    influx_interval_seconds = node.config.influxDB.postIntervalSeconds;
    bufferInfo.size.mpu     = (influx_interval_seconds * 1000) / mpu_frequency_ms;
    influx_interval_ms      = influx_interval_seconds * 1000;
    file_interval_ms        = node.config.logValuesFileClearSeconds * 1000;
    bufferInfo.deviceId     = strdup(node.config.id);
    bufferInfo.filename.mpu = make_path_n(3, node.config.servePath, node.config.dataFolderName, MPU_DATA_FILE_NAME);
    verbose_print("[LOG][%d]: Started -> Frequency(%i)ms\n", node.pid.dbs, node.config.logMilliseconds);
    pthread_mutex_unlock(&node.lock); 
    
    //-----------------------------------------------------------------
    //
    //-----------------------------------------------------------------
    init_log_buffer_mpu(bufferInfo.size.mpu);

    //-----------------------------------------------------------------
    // Thread Loop
    //-----------------------------------------------------------------
    while (1) {
        pthread_mutex_lock(&lock);
        if (!running) {
            pthread_mutex_unlock(&lock);
            break;
        }
        pthread_mutex_unlock(&lock);

        uint64_t now                     = get_now_ms();
        static uint64_t last_influx_time = 0;
        static uint64_t last_file_time   = 0;

        influx_interval   = (now - last_influx_time) >= influx_interval_ms;
        file_interval     = (now - last_file_time) >= file_interval_ms;

        if(influx_interval)
            last_influx_time = now;
        if(file_interval)
            last_file_time = now;

        log_write_values(influx_interval, file_interval, &influxCurl, &bufferInfo);

        file_interval   = false;
        influx_interval = false;

        sleep_ms(log_frequency_ms);
    }

    verbose_mutex_print("[LOG][%d]: Stopped\n", node.pid.dbs);

    //-----------------------------------------------------------------
    // Cleanup
    //-----------------------------------------------------------------
    free_log_buffers(&bufferInfo);
    log_free_influx_curl(&influxCurl);


    return NULL;
}