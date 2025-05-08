#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

char *str_replace(const char *input, const char *placeholder, const char *replacement) {
    size_t input_len = strlen(input);
    size_t placeholder_len = strlen(placeholder);
    size_t replacement_len = strlen(replacement);

    // Count occurrences
    int count = 0;
    const char *p = input;
    while ((p = strstr(p, placeholder)) != NULL) {
        count++;
        p += placeholder_len;
    }

    size_t new_len = input_len + count * (replacement_len - placeholder_len);
    char *result = malloc(new_len + 1);
    if (!result) return NULL;

    const char *src = input;
    char *dst = result;

    while ((p = strstr(src, placeholder)) != NULL) {
        size_t bytes = p - src;
        memcpy(dst, src, bytes);
        dst += bytes;
        memcpy(dst, replacement, replacement_len);
        dst += replacement_len;
        src = p + placeholder_len;
    }

    strcpy(dst, src);
    return result;
}

char *str_replace_pairs(const char *input, int num_pairs, ...) {
    va_list args;
    va_start(args, num_pairs);

    char *result = strdup(input);
    if (!result) return NULL;

    for (int i = 0; i < num_pairs; ++i) {
        const char *key = va_arg(args, const char *);
        const char *val = va_arg(args, const char *);
        char *temp = str_replace(result, key, val);
        free(result);
        result = temp;
    }

    va_end(args);
    return result;
}