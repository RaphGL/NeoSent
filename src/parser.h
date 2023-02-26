#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>

typedef enum {
  NS_IMAGE,
  NS_EMPTYSLIDE,
  NS_PARAGRAPH,
} ns_ItemType;

typedef struct {
  ns_ItemType type;
  int linenum;
  int colnum;
  char *content;
} ns_Item;

typedef struct ns_parser {
  char curr;
  int linenum;
  int colnum;
  FILE *stylesheet;
} ns_Parser;

typedef struct vec_vector vec_Vector;

// Initializes a new parser
ns_Parser ns_parser_new(char *filename);

// Frees all resources allocated by the parser
void ns_parser_free(ns_Parser *parser, vec_Vector *token_vec);

// Parses the entire stylesheet file and assigns a vector with the parsed tokens
// to token_vec
void ns_parser_parse(ns_Parser *parser, vec_Vector *token_vec);

#endif
