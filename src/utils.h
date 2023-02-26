#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>
#include <stdint.h>

// Returns the length of the longest line in the text
int ns_get_longest_line(const char *text);

// Returns true if little endian and false otherwise
bool ns_is_lil_endian(void);

// Converts a hexadecimal string into a 32 bit number
int32_t ns_get_color(char *hexnum);

#endif
