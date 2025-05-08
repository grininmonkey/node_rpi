#include <stdbool.h>       
#include "../../include/node.h"
#include "../../include/logBuffers.h"
#include "../../include/utilsSleepMs.h"
#include "../../include/logBufferBase.h" 
#include "../../include/logWriteValues.h"
#include "../../include/utilsVerbosePrint.h"
#include "../../include/utilsSignalHandler.h"
#include "../../include/utilsGetCurrentTimestamp.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Dummy Sensor Thread for testing Circular Buffer
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//temporary function to simulate data
float generate_random_float_x() {
    return (float)rand() / (float)RAND_MAX; // Generates a float between 0 and 1
}


void *sensors_dummy_thread(void *args) {
    //-----------------------------------------------------------------
    // Local variables
    //-----------------------------------------------------------------
    int frequency_ms = 10;


    verbose_print("[DUMMY]: Started -> Frequency(%i)ms\n", frequency_ms);
    //-----------------------------------------------------------------
    // Thread Loop
    //-----------------------------------------------------------------
    while (1) {
        pthread_mutex_lock(&lock);
        if (!running) {
            pthread_mutex_unlock(&lock);
            break;
        }

        node.mpu.accel.g.x = generate_random_float_x(),
        node.mpu.accel.g.y = generate_random_float_x(),
        node.mpu.accel.g.z = generate_random_float_x(),


        pthread_mutex_unlock(&lock);

        //-----------------------------------------------------------------
        // testing
        //-----------------------------------------------------------------
        if (logBufferMPU != NULL) {

            MPU test = {
                .timestamp = get_current_timestamp_ns(),
                .accel = {
                    .g = {
                        .x = generate_random_float_x(),
                        .y = generate_random_float_x(),
                        .z = generate_random_float_x()
                    },
                }
            };

            log_buffer_push(&logBufferMPU->buffer, &test, sizeof(MPU));
    
        }


        sleep_ms(frequency_ms);
    }

    verbose_print("[DUMMY]: Stopped\n");

    return NULL;
}