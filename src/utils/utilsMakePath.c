#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Strip leading and trailing slashes into a newly allocated string
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
char *strip_slashes(const char *input) {
    while (*input == '/') input++; // skip leading slashes
    size_t len = strlen(input);
    while (len > 0 && input[len - 1] == '/') len--; // remove trailing slashes

    char *clean = malloc(len + 1);
    if (!clean) return NULL;

    strncpy(clean, input, len);
    clean[len] = '\0';
    return clean;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Joins `count` strings into a normalized path with single slashes
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
char *make_path_n(int count, ...) {
    va_list args;
    va_start(args, count);
    //---------------------------------------------------
    // First pass: calculate total length
    //---------------------------------------------------
    size_t total = 0;
    bool is_absolute = false;
    const char **parts = malloc(sizeof(char*) * count);
    if (!parts) return NULL;

    for (int i = 0; i < count; i++) {
        parts[i] = va_arg(args, const char *);
        if (i == 0 && parts[i][0] == '/') is_absolute = true;
        total += strlen(parts[i]) + 1;  // +1 for slash
    }
    va_end(args);

    //---------------------------------------------------
    // Allocate output buffer
    //---------------------------------------------------
    char *path = malloc(total + 1);  // +1 for null terminator
    if (!path) {
        free(parts);
        return NULL;
    }

    path[0] = '\0';
    if (is_absolute) strcat(path, "/");

    //---------------------------------------------------
    // Build normalized path
    //---------------------------------------------------
    for (int i = 0; i < count; i++) {
        char *clean = strip_slashes(parts[i]);
        if (!clean) {
            free(path);
            free(parts);
            return NULL;
        }

        strcat(path, clean);
        if (i < count - 1) strcat(path, "/");

        free(clean);
    }

    free(parts);
    return path;
}
