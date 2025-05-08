#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/syscall.h>
#include "../../../include/node.h"
#include "../../../include/utilsSignalHandler.h"
#include "../../../include/utilsVerbosePrint.h"
#include "../../../include/utilsSleepMs.h"
#include "../../../include/mDnsPublishService.h"
#include "../../../include/httpAnswerConnection.h"

void* http_serve_thread(void* arg) {
    //--------------------------------------------------------------------------
    // Vars
    //--------------------------------------------------------------------------
    pid_t t_pid         = syscall(SYS_gettid);
    uint16_t httpPort   = 80;
    char *service_name  = NULL;
    char *service_type  = NULL;

    //--------------------------------------------------------------------------
    // Run Check & initial assignments
    //--------------------------------------------------------------------------
    pthread_mutex_lock(&node.lock);
    if (node.config.network.httpPort < 80) {
        pthread_mutex_unlock(&node.lock);
        return NULL;
    }
    node.pid.http = t_pid;
    httpPort      = node.config.network.httpPort;
    service_name  = strdup(node.config.id);
    service_type  = strdup(node.config.network.httpmDNSserviceType);
    pthread_mutex_unlock(&node.lock);
    //--------------------------------------------------------------------------
    // stdout Notification
    //--------------------------------------------------------------------------
    verbose_print("[NETWORK][%d]: Micro http started on port(%i)\n", t_pid, httpPort);

    //--------------------------------------------------------------------------
    // Start Micro HTTP daemon-server & publish mDNS http service
    //--------------------------------------------------------------------------
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, (unsigned int)httpPort, NULL, NULL,
        (MHD_AccessHandlerCallback)&http_answer_connection, NULL, MHD_OPTION_END);

    if (NULL == daemon) {
        verbose_mutex_print("[NETWORK][%d]: Micro http stopped, unable to start daemon\n", t_pid);
        return NULL;
    } 
    
    if (mdns_publish_service(httpPort, t_pid, service_name, service_type) != 0)
        fprintf(stderr, "[NETWORK][%d]: Failed to publish mDNS service.\n", t_pid);


    //--------------------------------------------------------------------------
    // thread loop (keep alive)
    //--------------------------------------------------------------------------
    while (1) {
        pthread_mutex_lock(&lock);
        if (!running) {
            pthread_mutex_unlock(&lock);
            break;
        }
        pthread_mutex_unlock(&lock);

        sleep_ms(100);
    }
    
    //--------------------------------------------------------------------------
    // Clean up
    //--------------------------------------------------------------------------
    free(service_name);
    free(service_type);
    stop_mdns_service(t_pid);
    MHD_stop_daemon(daemon);
    verbose_mutex_print("[NETWORK][%d]: Micro http Stopped\n", t_pid);
 
    return NULL;
}