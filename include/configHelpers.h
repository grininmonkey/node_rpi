#ifndef CONFIG_HELPERS_H
#define CONFIG_HELPERS_H

#include <jansson.h>
#include <stdbool.h>
#include <stdint.h>

int get_integer(int current, json_t *obj);
int get_true_false(int current, json_t *obj);
bool get_boolean(bool current, json_t *obj);
void set_string_nullable(char **dest, json_t *new_val);
void set_uint8(uint8_t *dest, json_t *new_val);
void set_uint16(uint16_t *dest, json_t *new_val);
void set_boolean(bool *dest, json_t *new_val);

#endif