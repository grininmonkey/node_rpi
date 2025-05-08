#include <string.h>
#include <unistd.h>
#include "../../include/node.h"
#include "../../include/configHelpers.h"


void set_string_nullable(char **dest, json_t *new_val) {
    if (json_is_string(new_val)) {
        const char *new_value = json_string_value(new_val);
        if (new_value && new_value[0] != '\0') {
            char *tmp = *dest;
            STRDUP_REPLACE_NULLABLE(tmp, new_value);
            *dest = tmp;
        }
    }
}

void set_uint8(uint8_t *dest, json_t *new_val){
    if (json_is_integer(new_val)) {
        uint8_t new_value = json_integer_value(new_val);
        if (new_value > 0) {
            *dest = new_value;
        }
    }
}

void set_uint16(uint16_t *dest, json_t *new_val){
    if (json_is_integer(new_val)) {
        uint16_t new_value = json_integer_value(new_val);
        if (new_value > 0) {
            *dest = new_value;
        }
    }
}

void set_boolean(bool *dest, json_t *new_val) {
    if (json_is_boolean(new_val)) {
        *dest = json_is_true(new_val);
    }
}

int get_true_false(int current, json_t *obj) {
    if (json_is_integer(obj))
        return INT_TRUE_FALSE(json_integer_value(obj));
    return current;
}

int get_integer(int current, json_t *obj) {
    if (json_is_integer(obj))
        return json_integer_value(obj);
    return current;
}

bool get_boolean(bool current, json_t *obj) {
    if (json_is_boolean(obj))
        return json_is_true(obj);
    return current;
}
