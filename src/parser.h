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


ns_Parser ns_parser_new(char *filename);
void ns_parser_free(ns_Parser *parser);
char ns_parser_peek(ns_Parser *parser);
char ns_parser_next(ns_Parser *parser);
void ns_parser_parse_comment(ns_Parser *parser);
void ns_parser_parse_image(ns_Parser *parser, ns_Item *item);
void ns_parser_parse_paragraph(ns_Parser *parser, ns_Item *item);
void ns_parser_parse_emptyslide(ns_Parser *parser, ns_Item *item);
void ns_parser_parse(ns_Parser *parser);

#endif
