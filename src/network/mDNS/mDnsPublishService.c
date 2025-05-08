#include <avahi-client/client.h>
#include <avahi-client/publish.h>
#include <avahi-common/error.h>
#include <avahi-common/thread-watch.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>      // for syscall()
#include <sys/syscall.h> // for SYS_gettid
#include "../../../include/mDnsPublishService.h"
#include "../../../include/utilsVerbosePrint.h"

// Store globals for cleanup
static AvahiThreadedPoll *avahi_poll = NULL;
static AvahiClient *client = NULL;
static AvahiEntryGroup *group = NULL;

static void entry_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state, void *userdata) {
    pid_t *thread_id = (pid_t *)userdata;
    switch (state) {
        case AVAHI_ENTRY_GROUP_ESTABLISHED:
            verbose_print("[NETWORK][%d]: Avahi service successfully established\n", *thread_id);
            break;
        case AVAHI_ENTRY_GROUP_COLLISION:
            fprintf(stderr, "[NETWORK][%d]: Avahi service name collision\n", *thread_id);
            break;
        case AVAHI_ENTRY_GROUP_FAILURE:
            fprintf(stderr, "[NETWORK][%d]: Avahi entry group failure: %s\n", *thread_id,
                    avahi_strerror(avahi_client_errno(avahi_entry_group_get_client(g))));
            break;
        default:
            break;
    }
    // Only free if this is the final or first meaningful callback
    if (state == AVAHI_ENTRY_GROUP_ESTABLISHED || state == AVAHI_ENTRY_GROUP_FAILURE || state == AVAHI_ENTRY_GROUP_COLLISION) {
        free(thread_id);
        thread_id = NULL;
    }
}

static void client_callback(AvahiClient *c, AvahiClientState state, void *userdata) {
    pid_t *thread_id = (pid_t *)userdata;
    if (state == AVAHI_CLIENT_FAILURE) {
        fprintf(stderr, "[NETWORK][%d]: Avahi client failure: %s\n", *thread_id, avahi_strerror(avahi_client_errno(c)));
    }
}

int mdns_publish_service(uint16_t port, pid_t t_pid, char *service_name, char *service_type) {

    int error;
    pid_t *t_pid_ptr = malloc(sizeof(pid_t));
    if (!t_pid_ptr) {
        perror("malloc");
        exit(1);
    }
    *t_pid_ptr = syscall(SYS_gettid);

    avahi_poll = avahi_threaded_poll_new();
    if (!avahi_poll) {
        fprintf(stderr, "[NETWORK][%d]: Failed to create threaded poll object.\n", *t_pid_ptr);
        return -1;
    }

    client = avahi_client_new(avahi_threaded_poll_get(avahi_poll), 0, client_callback, (void *)&t_pid, &error);
    if (!client) {
        fprintf(stderr, "[NETWORK][%d]: Failed to create Avahi client: %s\n", *t_pid_ptr, avahi_strerror(error));
        avahi_threaded_poll_free(avahi_poll);
        return -1;
    }

    group = avahi_entry_group_new(client, entry_group_callback, (void *)t_pid_ptr);
    if (!group) {
        fprintf(stderr, "[NETWORK][%d]: Failed to create entry group: %s\n", *t_pid_ptr, avahi_strerror(avahi_client_errno(client)));
        avahi_client_free(client);
        avahi_threaded_poll_free(avahi_poll);
        return -1;
    }

    int ret = avahi_entry_group_add_service(group, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, 0,
                                            service_name, service_type, NULL, NULL, port, NULL);
    if (ret < 0) {
        fprintf(stderr, "[NETWORK][%d]: Failed to add service [%s]: %s\n", *t_pid_ptr, service_type, avahi_strerror(ret));
        if (group)      avahi_entry_group_free(group);
        if (client)     avahi_client_free(client);
        if (avahi_poll) avahi_threaded_poll_free(avahi_poll);
        free(t_pid_ptr);        
        return -1;
    }

    ret = avahi_entry_group_commit(group);
    if (ret < 0) {
        fprintf(stderr, "[NETWORK][%d]: Failed to commit entry group: %s\n", *t_pid_ptr, avahi_strerror(ret));
        return -1;
    }

    verbose_print("[NETWORK][%d]: Starting Avahi threaded poll...\n", *t_pid_ptr);
    avahi_threaded_poll_start(avahi_poll);

    return 0;
}

void stop_mdns_service(pid_t t_pid) {
    verbose_print("[NETWORK][%d]: Stopping Avahi service...\n", t_pid);

    if (avahi_poll) {
        avahi_threaded_poll_stop(avahi_poll);
    }
    if (group) {
        avahi_entry_group_free(group);
        group = NULL;
    }
    if (client) {
        avahi_client_free(client);
        client = NULL;
    }
    if (avahi_poll) {
        avahi_threaded_poll_free(avahi_poll);
        avahi_poll = NULL;
    }
}
