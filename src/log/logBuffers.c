#include "../../include/logBuffers.h"
#include "../../include/logBufferBase.h"
#include <stdlib.h>

LogBufferMPU *logBufferMPU = NULL;
//LogBufferBME *logBufferBME = NULL;

int init_log_buffer_mpu(int buffer_size) {
    logBufferMPU = calloc(1, sizeof(LogBufferMPU));
    if (!logBufferMPU) return -1;

    return init_log_buffer_base(
        (void **)&logBufferMPU->buffer,
        &logBufferMPU->size,
        &logBufferMPU->head,
        &logBufferMPU->count,
        &logBufferMPU->lock,
        buffer_size,
        sizeof(MPU)
    );
}

/*
int init_log_buffer_bme(int buffer_size) {
    logBufferBME = calloc(1, sizeof(LogBufferBME));
    if (!logBufferBME) return -1;

    return init_log_buffer_base(
        (void **)&logBufferBME->buffer,
        &logBufferBME->size,
        &logBufferBME->head,
        &logBufferBME->count,
        &logBufferBME->lock,
        buffer_size,
        sizeof(BME)
    );
}
*/

void free_log_buffers(LogBufferInfo *bufferInfo) {

    if (bufferInfo) {
        SAFE_FREE(bufferInfo->filename.mpu);
        SAFE_FREE(bufferInfo->filename.bme);
        SAFE_FREE(bufferInfo->deviceId);
    }
    
    if (logBufferMPU) {
        free_log_buffer_base((void **)&logBufferMPU->buffer, &logBufferMPU->lock);
        free(logBufferMPU);
        logBufferMPU = NULL;
    }

    /*
    if (logBufferBME) {
        free_log_buffer_base((void **)&logBufferBME->buffer, &logBufferBME->lock);
        free(logBufferBME);
        logBufferBME = NULL;
    }
    */
}
