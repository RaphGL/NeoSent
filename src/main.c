#include "parser.h"
#include "render.h"
#include "utils.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // --- Parsing command line arguments ---
  int opt;
  uint32_t text_color = 0xFFFFFFFF;
  uint32_t bg_color = 0;
  char *font = "NotoSans-Regular.ttf";
  while ((opt = getopt(argc, argv, ":t:b:f:")) != -1) {
    switch (opt) {
    case 't':
      text_color = ns_get_color(optarg);
      break;

    case 'b':
      bg_color = ns_get_color(optarg);
      break;

    case 'f':
      font = optarg;
      break;

    case ':':
      fputs("Error: Argument was not provided to option.\n", stderr);
      return 1;
      break;

    case '?':
      fputs("Error: No such option.\n", stderr);
      return 1;
      break;
    }
  }

  char *stylesheet = argv[optind];

  // --- Parsing presentation file ---
  ns_Parser parser = ns_parser_new(stylesheet);
  vec_Vector *token_vec = vec_new();
  ns_parser_parse(&parser, token_vec);

  // --- Presenting contents of file ---
  ns_Renderer renderer =
      ns_renderer_create(stylesheet, font, text_color, bg_color);
  SDL_Event e;
  size_t page = 0;
  for (;;) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        goto cleanup;
      }

      if (e.key.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
        case SDLK_RIGHT:
        case 'l':
          if (page + 1 < vec_len(token_vec)) {
            ++page;
          }
          break;

        case SDLK_LEFT:
        case 'h':
          if (page != 0) {
            --page;
          }
          break;

        case 'q':
          goto cleanup;
          break;
        }
      }
    }

    ns_renderer_draw(&renderer, token_vec, page);
  }

  // --- Final cleanup ---
cleanup:
  ns_renderer_destroy(&renderer);
  ns_parser_free(&parser, token_vec);
  return 0;
}
