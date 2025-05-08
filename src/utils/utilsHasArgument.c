#include <string.h>
#include <stdbool.h>

bool has_arg(int argc, char *argv[], const char *search_arg) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], search_arg) == 0) {
            return 1; // found
        }
    }
    return 0; // not found
}

const char *get_arg_value(int argc, char *argv[], const char *search_arg) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], search_arg) == 0) {
            if (i + 1 < argc) { // Ensure there is a next argument
                return argv[i + 1]; // Return the next argument
            } else {
                return NULL; // No value follows the search_arg
            }
        }
    }
    return NULL; // search_arg not found
}