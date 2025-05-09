#include <unistd.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <avahi-common/defs.h>
#include <avahi-client/lookup.h>
#include <avahi-client/client.h>
#include <avahi-common/error.h>
#include <avahi-common/timeval.h>
#include <avahi-common/simple-watch.h>
#include "../../../include/node.h"
#include "../../../include/utilsNodeFree.h"
#include "../../../include/utilsSignalHandler.h"
#include "../../../include/utilsVerbosePrint.h"
#include "../../../include/mDnsScanCallbacks.h"


typedef struct {
    AvahiSimplePoll *simple_poll;
    AvahiTimeout *timeout;
} MdnsContext;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Helper: Create Service Browser 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static AvahiServiceBrowser* create_browser(AvahiClient *client, const char *type) {
    AvahiServiceBrowser *sb = avahi_service_browser_new(
        client, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC,
        type, NULL, 0, browse_callback, client
    );

    if (!sb) {
        fprintf(stderr, "[mDNS Scan][%d]: Failed to create browser for type '%s': %s\n",
                node.pid.mDNS, type, avahi_strerror(avahi_client_errno(client)));
    }

    return sb;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// mDNS: Scan Service Thread
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void *mdns_scan_service_thread(void *arg) {
    //--------------------------------------------------------------------------
    // Variables
    //--------------------------------------------------------------------------
    int error;
    AvahiClient *client;
    AvahiServiceBrowser *sbhttp, *sbmq, *sbflux;
    MdnsScanParams *params = (MdnsScanParams *)arg;
    pid_t t_pid = syscall(SYS_gettid);
    char *fluxmDNSserviceType;
    char *httpmDNSserviceType;
    char *mqBrokermDNSserviceType;

    //--------------------------------------------------------------------------
    // Run Check and local assignments
    //--------------------------------------------------------------------------
    pthread_mutex_lock(&node.lock);
    if (node.config.network.mDNS < 1) return NULL;
    node.pid.mDNS           = t_pid;
    httpmDNSserviceType     = strdup(node.config.network.httpmDNSserviceType);
    fluxmDNSserviceType     = strdup(node.config.influxDB.mDNSserviceType);
    mqBrokermDNSserviceType = strdup(node.config.network.mqBrokermDNSserviceType);
    pthread_mutex_unlock(&node.lock);

    //--------------------------------------------------------------------------
    // Notification
    //--------------------------------------------------------------------------
    verbose_mutex_print("[mDNS Scan][%d]: Started\n", t_pid);
    
    //--------------------------------------------------------------------------
    // Lock mutex
    //--------------------------------------------------------------------------
    pthread_mutex_lock(&lock);
    
    //--------------------------------------------------------------------------
    // Create Poll
    //--------------------------------------------------------------------------
    simple_poll = avahi_simple_poll_new();
    if (!simple_poll) {
        fprintf(stderr, "[mDNS Scan][%d]: Failed to create poll loop\n", t_pid);
        return NULL;
    }
    
    //--------------------------------------------------------------------------
    //  Set Client
    //--------------------------------------------------------------------------
    client = avahi_client_new(
        avahi_simple_poll_get(simple_poll), 0,
        client_callback, NULL, &error
    );
    if (!client) {
        fprintf(stderr, "[mDNS Scan][%d]: Failed to create client: %s\n", t_pid, avahi_strerror(error));
        return NULL;
    }
    
    //--------------------------------------------------------------------------
    //  Create Browser(s)
    //--------------------------------------------------------------------------
    sbhttp  = create_browser(client, httpmDNSserviceType);      free(httpmDNSserviceType);    
    sbflux  = create_browser(client, fluxmDNSserviceType);      free(fluxmDNSserviceType);
    sbmq    = create_browser(client, mqBrokermDNSserviceType);  free(mqBrokermDNSserviceType);
    
    //--------------------------------------------------------------------------
    // Unlock mutex and loop the poll
    //--------------------------------------------------------------------------    
    pthread_mutex_unlock(&lock);

    if (sbmq || sbhttp || sbflux) {
        if (params && params->one_shot) {
            if (params->timeout_secs <= 0) {
                fprintf(stderr, "Invalid timeout value — must be > 0\n");
            } else {
                struct timeval tv;
                avahi_simple_poll_get(simple_poll)->timeout_new(
                    avahi_simple_poll_get(simple_poll),
                    avahi_elapse_time(&tv, params->timeout_secs * 1000, 0),
                    timeout_callback,
                    client);                                
            }
        }        
        avahi_simple_poll_loop(simple_poll);
        //..............................
        // Cleanup after poll loop ends
        //''''''''''''''''''''''''''''''
        if (sbmq) avahi_service_browser_free(sbmq);
        if (sbhttp) avahi_service_browser_free(sbhttp);
        if (sbflux) avahi_service_browser_free(sbflux);
        avahi_client_free(client);
        avahi_simple_poll_free(simple_poll);
    } else {
        avahi_client_free(client);
        avahi_simple_poll_free(simple_poll);
        fprintf(stderr, "[mDNS Scan]: No service browsers created, exiting.\n");
    }    

    //--------------------------------------------------------------------------
    // Cleanup Servicelist
    //--------------------------------------------------------------------------
    //free_service_list(true);

    //--------------------------------------------------------------------------
    //  Complete
    //--------------------------------------------------------------------------
    verbose_mutex_print("[mDNS Scan][%d]: Stopped\n", t_pid);

    return NULL;

}


