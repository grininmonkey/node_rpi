// signal_handler.h
#ifndef UTILS_SIGNAL_HANDLER_H
#define UTILS_SIGNAL_HANDLER_H

#include <signal.h>
#include <stdio.h>
#include <avahi-common/simple-watch.h>

extern volatile sig_atomic_t running;
extern pthread_mutex_t lock;
extern AvahiSimplePoll *simple_poll;

// Declaration of the signal handler function
void signal_handler(int sig);

#endif // SIGNAL_HANDLER_H

