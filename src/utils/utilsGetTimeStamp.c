#include <time.h>
#include <stdio.h>

const char* get_timestamp() {
    static char buffer[80];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local);
    return buffer;
}