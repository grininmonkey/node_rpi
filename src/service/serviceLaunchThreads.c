#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../../include/node.h"
#include "../../include/sensorsDummy.h"
#include "../../include/utilsSetTmpfs.h"
#include "../../include/utilsNodeFree.h"
#include "../../include/httpServeThread.h"
#include "../../include/logValuesThread.h"
#include "../../include/utilsVerbosePrint.h"
#include "../../include/utilsSignalHandler.h"
#include "../../include/mDnsScanServiceThread.h"
#include "../../include/utilsCheckServiceRunning.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Define the number of threads to launch
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define NUM_SERVICE_THREADS 4

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Thread Function(s) Array
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void* (*service_thread_functions[NUM_SERVICE_THREADS])(void*) = {
    http_serve_thread,
    sensors_dummy_thread,
    log_values_thread,
    mdns_scan_service_thread
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Function pointer array
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void service_launch_threads(int argc, char *argv[]) {
    //--------------------------------------------
    // Check if already running service instance
    //--------------------------------------------
    if (check_service_running(node.pid.main) == 1) {
        printf("Already running a service instance!\n");
        return;
    }

    //--------------------------------------------
    // Start msg
    //--------------------------------------------
    verbose_print("[main][%d]: Started using: %s\n", node.pid.main, node.config.configFile);
    
    //--------------------------------------------
    // Initialize and Launch Service Threads
    //--------------------------------------------
    if (setTmpfs(node.pid.main)) {

        pthread_t threads[NUM_SERVICE_THREADS];
        for (int i = 0; i < NUM_SERVICE_THREADS; i++) {
            if (pthread_create(&threads[i], NULL, service_thread_functions[i], NULL) != 0) {
                perror("service_launch_threads - pthread_create");
            }
        }

        for (int i = 0; i < NUM_SERVICE_THREADS; i++) {
            int res = pthread_join(threads[i], NULL);
            if (res != 0) {
                fprintf(stderr, "Error joining thread %d: %s\n", i, strerror(res));
            }                         
        }        

    }
    //--------------------------------------------
    // Clear Mutex(s)
    //--------------------------------------------
    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&node.lock);
    //--------------------------------------------
    // Free memory allocations etc...
    //--------------------------------------------
    free_network();

    //--------------------------------------------
    // Exit msg
    //--------------------------------------------
    verbose_print("[main][%d]: Stopped\n", node.pid.main);

}