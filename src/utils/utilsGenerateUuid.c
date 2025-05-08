#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

char* generate_uuid_string(void) {
    uuid_t uuid;
    char uuid_str[37];

    uuid_generate(uuid);
    uuid_unparse(uuid, uuid_str);

    return strdup(uuid_str);  // Caller must free the returned string
}
