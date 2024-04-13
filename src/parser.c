#include "parser.h"
#include "utils.h"
#include "vector.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ns_Parser ns_parser_new(char *filename) {
  FILE *stylesheet = fopen(filename, "r");
  if (!stylesheet) {
    fputs("Error: No such file or directory.\n", stderr);
    exit(1);
  }

  return (ns_Parser){
      .curr = '\0',
      .linenum = 1,
      .colnum = 0,
      .stylesheet = stylesheet,
  };
}

void ns_parser_free(ns_Parser *parser, vec_Vector *token_vec) {
  fclose(parser->stylesheet);
  *parser = (ns_Parser){
      .stylesheet = NULL,
      .colnum = 0,
      .linenum = 1,
      .curr = '\0',
  };

  for (size_t i = 0; i < vec_len(token_vec); i++) {
    ns_Item item = vec_get(token_vec, i);
    free(item.content);
  }

  vec_free(token_vec);
}

static char ns_parser_peek(ns_Parser *parser) {
  int c = fgetc(parser->stylesheet);
  ungetc(c, parser->stylesheet);
  return c;
}

static char ns_parser_next(ns_Parser *parser) {
  parser->curr = fgetc(parser->stylesheet);

  if (parser->curr == '\n') {
    ++parser->linenum;
    parser->colnum = 0;
  } else {
    ++parser->colnum;
  }

  return parser->curr;
}

static void ns_parser_parse_comment(ns_Parser *parser) {
  if (parser->curr != '#') {
    return;
  }

  int c;
  while ((c = ns_parser_next(parser)) != EOF) {
    if (c == '\n') {
      return;
    }
  }
}

static void ns_parser_parse_image(ns_Parser *parser, ns_Item *item) {
  ns_Item image = (ns_Item){
      .colnum = parser->colnum,
      .linenum = parser->linenum,
      .type = NS_IMAGE,
      .content = malloc(PATH_MAX),
  };

  int c;
  size_t i = 0;
  while ((c = ns_parser_next(parser)) != EOF) {
    // included images can't have spaces in their names
    if (isspace(c)) {
      image.content[i] = '\0';
      *item = image;
      break;
    }

    image.content[i] = c;
    i++;
  }

  // skips the all subsequent text and places curr in the next section
  while (c != EOF) {
    if (c == '\n' && ns_parser_peek(parser) == '\n') {
      return;
    }

    c = ns_parser_next(parser);
  }
}

static void ns_parser_parse_paragraph(ns_Parser *parser, ns_Item *item) {
  if (isspace(parser->curr)) {
    return;
  }

  int c;
  size_t i = 1;
  size_t para_size = 80;

  ns_Item paragraph = (ns_Item){
      .type = NS_PARAGRAPH,
      .colnum = parser->colnum,
      .linenum = parser->linenum,
      .content = malloc(sizeof(char) * (para_size + 1)),
  };

  paragraph.content[0] = parser->curr;
  while ((c = ns_parser_next(parser)) != EOF) {
    // skip escaped chars
    if (c == '\\') {
      switch (ns_parser_peek(parser)) {
      case '#':
      case '\\':
      case '@':
        c = ns_parser_next(parser);
        break;
      }
    }

    if (c == '#') {
      ns_parser_parse_comment(parser);
      c = '\n';
      --i; // fixes double newline insertion
    }

    if (c == '\n' &&
        (ns_parser_peek(parser) == '\n' || ns_parser_peek(parser) == EOF)) {
      paragraph.content[i] = '\0';
      *item = paragraph;
      return;
    }

    // grow string if necessary
    if (i == para_size) {
      para_size += 80;
      char *tmp = realloc(paragraph.content, para_size);
      if (!tmp) {
        fputs("Error: Could not allocate more memory.", stderr);
        exit(1);
      }
      paragraph.content = tmp;
    }

    paragraph.content[i] = c;
    i++;
  }
}

static bool ns_parser_parse_emptyslide(ns_Parser *parser, ns_Item *item) {
  if (parser->colnum != 1 && parser->curr != '\\') {
    return false;
  }

  int next = ns_parser_next(parser);
  if (!isspace(next)) {
    return false;
  }

  ns_Item empty_slide = (ns_Item){
      .type = NS_EMPTYSLIDE,
      .linenum = parser->linenum,
      .colnum = parser->colnum,
      .content = malloc(sizeof(char) * 2),
  };
  empty_slide.content[0] = '\\';
  empty_slide.content[1] = '\0';
  *item = empty_slide;
  return true;
}

#ifndef NDEBUG
void ns_parser_print(vec_Vector *token_vec) {
  for (size_t i = 0; i < vec_len(token_vec); i++) {
    ns_Item item = vec_get(token_vec, i);
    switch (item.type) {
    case NS_IMAGE:
      printf("Image(\n%s\n)", item.content);
      break;
    case NS_EMPTYSLIDE:
      printf("EmptySlide");
      break;
    case NS_PARAGRAPH:
      printf("Paragraph(\n%s\n)", item.content);
      break;
    }
    printf("\n\n");
  }
}
#endif

void ns_parser_parse(ns_Parser *parser, vec_Vector *token_vec) {
  while ((parser->curr = ns_parser_next(parser)) != EOF) {
    ns_Item item = {0};
    if (parser->colnum != 1)
      continue;
    switch (parser->curr) {
    case '@':
      ns_parser_parse_image(parser, &item);
      break;
    case '#':
      ns_parser_parse_comment(parser);
      break;
    case '\\':
      if (ns_parser_parse_emptyslide(parser, &item)) {
        ns_parser_parse_paragraph(parser, &item);
      }
      break;
    default:
      ns_parser_parse_paragraph(parser, &item);
      break;
    }

    if (item.content != NULL) {
      vec_push(token_vec, item);
      item.content = NULL;
    }
  }

#ifndef NDEBUG
  ns_parser_print(token_vec);
#endif
}
