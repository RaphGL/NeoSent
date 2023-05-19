#include "utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
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

uint32_t ns_get_color(char *hexnum) {
  int len = strlen(hexnum);
  if (len == 0) {
    return 0;
  }

  int i;
  if (len > 2 && hexnum[0] == '0' && hexnum[1] == 'x') {
    i = 2;
  } else if (len > 1 && hexnum[0] == '#') {
    i = 1;
  } else {
    i = 0;
  }

  char hexc[3] = {0};
  int curr_color = 0;
  uint32_t end_color = 0;

  for (; i < len; i += 2) {
    hexc[0] = hexnum[i];
    hexc[1] = hexnum[i + 1];
    uint32_t hexn = strtoul(hexc, NULL, 16);
    end_color |= hexn << (8 * curr_color);
    curr_color++;
  }

  return end_color;
}

void ns_get_font_path(char dest[PATH_MAX], const char *fontname) {
  char cmd[BUFSIZ] = "fc-match --format=%{file} ";
  strcat(cmd, fontname);
  FILE *fp = popen(cmd, "r");
  fgets(dest, PATH_MAX, fp);
  pclose(fp);
}
