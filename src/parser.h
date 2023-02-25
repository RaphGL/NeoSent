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

// Peek at the next character in the stylesheet
char ns_parser_peek(ns_Parser *parser);

// Get the char in the stylesheet. One cannot go back a char after doing this.
char ns_parser_next(ns_Parser *parser);

// Parses comments and sets the curr char at the first whitespace after the it
void ns_parser_parse_comment(ns_Parser *parser);

// Parses image includes and sets its path to item->content and sets the curr
// char to the first whitespace after it
void ns_parser_parse_image(ns_Parser *parser, ns_Item *item);

// Parses paragraphs and sets their content to item->content and sets the curr
// char to the first whitespace after it
void ns_parser_parse_paragraph(ns_Parser *parser, ns_Item *item);

// Parses empty slides and sets the curr char to the first whiespace after it
void ns_parser_parse_emptyslide(ns_Parser *parser, ns_Item *item);

// Parses the entire stylesheet file and assigns a vector with the parsed tokens
// to token_vec
void ns_parser_parse(ns_Parser *parser, vec_Vector *token_vec);

#endif
