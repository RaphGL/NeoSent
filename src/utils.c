#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int ns_get_longest_line(const char *text) {
  int text_len = 0;
  int biggest_len = 0;
  for (int i = 0; text[i] != '\0'; i++) {
    text_len++;
    if (text[i] == '\n') {
      text_len = 0;
    }

    if (text_len > biggest_len) {
      biggest_len = text_len;
    }
  }

  return biggest_len;
}

bool ns_is_lil_endian(void) {
  volatile uint32_t i = 0x01234567;
  // return false for big endian, true for little endian.
  return (*((uint8_t *)(&i))) == 0x67;
}

uint32_t ns_get_color(char *hexnum) {
  int len = strlen(hexnum);
  if (len == 0) {
    return 0;
  }

  if (len > 2 && hexnum[0] == '0' && hexnum[1] == 'x') {
    char hexc[3] = {0};
    int curr_color = 0;
    uint32_t end_color = 0;
    for (int i = 2; i < len; i += 2) {
      hexc[0] = hexnum[i];
      hexc[1] = hexnum[i + 1];
      uint32_t hexn = strtoul(hexc, NULL, 16);
      if (ns_is_lil_endian()) {
        end_color |= hexn << (8 * curr_color);
      } else {
        end_color |= hexn >> (8 * curr_color);
      }
      curr_color++;
    }
    return end_color;
  }

  return strtoul(hexnum, NULL, 10);
}
