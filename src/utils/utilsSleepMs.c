#include <time.h>
#include <stdio.h>
#include "../../include/utilsSleepMs.h"

void sleep_ms(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;              // whole seconds
    ts.tv_nsec = (milliseconds % 1000) * 1000000; // remaining ms → ns
    nanosleep(&ts, NULL);
}