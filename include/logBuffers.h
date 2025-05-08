#ifndef LOG_BUFFERS_H
#define LOG_BUFFERS_H

#include <pthread.h>
#include "node.h"

typedef struct {
    MPU *buffer;
    int size;
    int head;
    int count;
    pthread_mutex_t lock;
} LogBufferMPU;

/*
typedef struct {
    BME *buffer;
    int size;
    int head;
    int count;
    pthread_mutex_t lock;
} LogBufferBME;
*/

typedef struct {
    uint64_t mpu_file;
    uint64_t mpu_influx;
    uint64_t bme;
} LogBufferLastTimestamp;

typedef struct {
    char *mpu;
    char *bme;
} LogBufferFileInfo;

typedef struct {
    int mpu;
    int bme;
} LogBufferSize;

typedef struct {
    char *deviceId;
    LogBufferSize size;
    LogBufferLastTimestamp lastTimestamp;
    LogBufferFileInfo filename;
} LogBufferInfo;

extern LogBufferMPU *logBufferMPU;
//extern LogBufferBME *logBufferBME;

int init_log_buffer_mpu(int buffer_size);
int init_log_buffer_bme(int buffer_size);
void free_log_buffers(LogBufferInfo *bufferInfo);

#endif