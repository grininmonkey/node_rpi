#ifndef LOG_BUFFER_BASE_H
#define LOG_BUFFER_BASE_H

#include <pthread.h>

// Generic initializer for log buffers with common layout
int init_log_buffer_base(
    void **buffer,
    int *size,
    int *head,
    int *count,
    pthread_mutex_t *lock,
    int buffer_size,
    size_t element_size
);

void free_log_buffer_base(void **buffer, pthread_mutex_t *lock);
int log_buffer_push(void *log_buffer, void *sample, size_t element_size);
int log_buffer_pop_all(void *log_buffer, void *out_array, size_t element_size, int max_out);
int log_buffer_peek_all(void *log_buffer, void *out_array, size_t element_size, int max_out);

#endif
