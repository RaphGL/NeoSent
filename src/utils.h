#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>
#include <stdint.h>

#define PATH_MAX 4096

// Returns the length of the longest line in the text
int ns_get_longest_line(const char *text);

// Converts a hexadecimal string into a 32 bit number
uint32_t ns_get_color(char *hexnum);

// Finds a font with the specified name, fontname is an empty string, a default
// font will be provided
void ns_get_font_path(char dest[PATH_MAX], const char *fontname);

#endif
