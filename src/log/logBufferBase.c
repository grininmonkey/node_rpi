#include <stdlib.h>
#include <string.h>
#include "../../include/logBufferBase.h"

typedef struct {
    void *buffer;
    int size;
    int head;
    int count;
    pthread_mutex_t lock;
} GenericLogBuffer;

int init_log_buffer_base(
    void **buffer,
    int *size,
    int *head,
    int *count,
    pthread_mutex_t *lock,
    int buffer_size,
    size_t element_size
) {
    if (buffer_size < 1 || element_size == 0 || !buffer || !size || !head || !count || !lock)
        return -1;

    void *buf = calloc(buffer_size, element_size);
    if (!buf)
        return -1;

    *buffer = buf;
    *size = buffer_size;
    *head = 0;
    *count = 0;

    if (pthread_mutex_init(lock, NULL) != 0) {
        free(buf);
        *buffer = NULL;
        return -1;
    }

    return 0;
}

int log_buffer_push(void *log_buffer, void *sample, size_t element_size) {
    GenericLogBuffer *buf = (GenericLogBuffer *)log_buffer;

    pthread_mutex_lock(&buf->lock);

    int index = buf->head;
    char *base = (char *)buf->buffer;
    memcpy(base + (index * element_size), sample, element_size);

    buf->head = (buf->head + 1) % buf->size;
    if (buf->count < buf->size) {
        buf->count++;
    }

    pthread_mutex_unlock(&buf->lock);
    return 0;
}

int log_buffer_peek_all(void *log_buffer, void *out_array, size_t element_size, int max_out) {
    GenericLogBuffer *buf = (GenericLogBuffer *)log_buffer;

    pthread_mutex_lock(&buf->lock);

    int copied = buf->count;
    if (copied > max_out) copied = max_out;

    int start = (buf->head - buf->count + buf->size) % buf->size;
    char *src = (char *)buf->buffer;
    char *dst = (char *)out_array;

    for (int i = 0; i < copied; ++i) {
        int index = (start + i) % buf->size;
        memcpy(dst + (i * element_size), src + (index * element_size), element_size);
    }

    pthread_mutex_unlock(&buf->lock);
    return copied; // Number of items copied into out_array
}


int log_buffer_pop_all(void *log_buffer, void *out_array, size_t element_size, int max_out) {
    GenericLogBuffer *buf = (GenericLogBuffer *)log_buffer;

    pthread_mutex_lock(&buf->lock);

    int copied = buf->count;
    if (copied > max_out) copied = max_out;

    int start = (buf->head - copied + buf->size) % buf->size;
    char *src = (char *)buf->buffer;
    char *dst = (char *)out_array;

    for (int i = 0; i < copied; ++i) {
        int index = (start + i) % buf->size;
        memcpy(dst + (i * element_size), src + (index * element_size), element_size);
    }

    // Clear buffer after pop
    buf->count = 0;

    pthread_mutex_unlock(&buf->lock);
    return copied; // number of samples written to out_array
}


void free_log_buffer_base(void **buffer, pthread_mutex_t *lock) {
    if (buffer && *buffer) {
        free(*buffer);
        *buffer = NULL;
    }
    if (lock)
        pthread_mutex_destroy(lock);
}
