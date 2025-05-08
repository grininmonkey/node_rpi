#include <stdio.h>
#include <string.h>
#include "../../include/node.h"


void verbose_print(const char *format, ...) {
    if (!node.config.verbose) {
        return;
    }

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void verbose_mutex_print(const char *format, ...) {
    pthread_mutex_lock(&node.lock);
        if (!node.config.verbose) {
            pthread_mutex_unlock(&node.lock);    
            return;
        }
    pthread_mutex_unlock(&node.lock);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}