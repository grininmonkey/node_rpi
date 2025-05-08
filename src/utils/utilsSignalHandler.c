// signal_handler.c
#include <pthread.h>
#include <avahi-common/simple-watch.h>
#include "../../include/utilsSignalHandler.h"

// Flag to control the program's state
volatile sig_atomic_t running = 1;

// mDNS browse poll
AvahiSimplePoll *simple_poll = NULL;
  
// Mutex initialization
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;  // Mutex initialization

// Signal handler for SIGINT (Ctrl+C) and SIGTERM
void signal_handler(int sig) {
    pthread_mutex_lock(&lock);
    running = 0;
    if (simple_poll) 
        avahi_simple_poll_quit(simple_poll);
    pthread_mutex_unlock(&lock);
    printf("\nReceived signal %d, shutting down gracefully...\n", sig);
}

