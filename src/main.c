#include <stdio.h>
#include "parser.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fputs("Error: No stylesheet file was provided.\n", stderr);
    return 1;
  }

  ns_Parser parser = ns_parser_new(argv[1]);

  ns_parser_parse(&parser);

  ns_parser_free(&parser);
  return 0;
}
