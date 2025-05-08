#include <time.h>
#include <stdint.h>

long long get_current_timestamp_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts); // Get current time
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec; // Convert to nanoseconds
}

uint64_t get_now_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);  // Get current time
    return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000; // Convert to milliseconds
}