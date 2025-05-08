#ifndef UTILS_HAS_ARGUMENT_H
#define UTILS_HAS_ARGUMENT_H

bool has_arg(int argc, char *argv[], const char *search_arg);
const char *get_arg_value(int argc, char *argv[], const char *search_arg);

#endif