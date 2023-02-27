#include "parser.h"
#include "render.h"
#include "utils.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <getopt.h>
#include <linux/limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void ns_show_help_message(void) {
  puts("Simple plaintext presentation tool"
       "\n\n"
       "Usage: nsent [OPTION...] <FILE>"
       "\n\n"
       " -t\tChange the text color\n"
       " -b\tChange the background color\n"
       " -f\tChange the font family\n"
       " -h\tShow this message");
}

int main(int argc, char *argv[]) {
  int opt;

  // default values
  uint32_t text_color = 0xFFFFFFFF;
  uint32_t bg_color = 0;
  char font[PATH_MAX];
  ns_get_font_path(font, "");

  // --- Parsing command line arguments ---
  while ((opt = getopt(argc, argv, ":ht:b:f:")) != -1) {
    switch (opt) {
    case 't':
      text_color = ns_get_color(optarg);
      break;

    case 'b':
      bg_color = ns_get_color(optarg);
      break;

    case 'f':
      ns_get_font_path(font, optarg);
      break;

    case ':':
      fputs("Error: Argument was not provided to option.\n", stderr);
      return 1;
      break;

    case '?':
      fputs("Error: No such option.\n", stderr);
      return 1;
      break;

    case 'h':
      ns_show_help_message();
      return 0;
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
      ns_renderer_create(stylesheet, font, text_color, bg_color, token_vec);
  SDL_Event e;
  size_t page = 0;
  for (;;) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        goto cleanup;
      }

      // --- Neosent keybindings ---
      if (e.key.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
          // next page
        case SDLK_RIGHT:
        case SDLK_DOWN:
        case SDLK_PAGEDOWN:
        case 'l':
        case 'j':
        case 'n':
          if (page + 1 < vec_len(token_vec)) {
            ++page;
          }
          break;

          // previous page
        case SDLK_LEFT:
        case SDLK_UP:
        case SDLK_PAGEUP:
        case 'h':
        case 'k':
        case 'p':
          if (page != 0) {
            --page;
          }
          break;

        case 'i':
          ns_renderer_invert_colors(&renderer);
          break;

          // TODO show keymaps
        case SDLK_F1:
          break;

          // toggle fullscreen
        case SDLK_F11:
        case 'f':
          ns_renderer_toggle_fullscreen(&renderer);
          break;

          // go to first page
        case SDLK_HOME:
          page = 0;
          break;

          // go to last page
        case SDLK_END:
          page = vec_len(token_vec) - 1;
          break;

          // close program
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
