#ifndef UTILS_STR_REPLACE_H
#define UTILS_STR_REPLACE_H

char *str_replace(const char *input, const char *placeholder, const char *replacement);
char *str_replace_pairs(const char *input, int num_pairs, ...);

#endif