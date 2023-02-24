#include "parser.h"
#include "vector.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fputs("Error: No stylesheet file was provided.\n", stderr);
    return 1;
  }

  ns_Parser parser = ns_parser_new(argv[1]);
  vec_Vector *token_vec = vec_new();

  ns_parser_parse(&parser, token_vec);

  ns_parser_free(&parser, token_vec);
  return 0;
}
