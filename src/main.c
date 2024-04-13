#include "parser.h"
#include "render.h"
#include "utils.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define NS_VERSION "1.2"

static ns_Renderer renderer;
static ns_Parser parser;
static vec_Vector *token_vec;

void ns_cleanup(int signum) {
  (void)signum;
  ns_renderer_destroy(&renderer);
  ns_parser_free(&parser, token_vec);
  exit(0);
}

void ns_show_help_message(void) {
  puts("Simple plaintext presentation tool"
       "\n\n"
       "Usage: nsent [OPTION...] <FILE>"
       "\n\n"
       " -t\tChange the text color\n"
       " -b\tChange the background color\n"
       " -f\tChange the font family\n"
       " -s\tChange font size\n"
       " -p\tHide the progress bar\n"
       " -h\tShow this message");
}

int main(int argc, char *argv[]) {
  int opt;

  // --- Default values ---
  uint32_t text_color = 0xFFFFFFFF;
  uint32_t bg_color = 0;
  bool show_progressbar = true;
  char font[PATH_MAX];
  size_t fontsiz = 52;
  ns_get_font_path(font, "");

  // --- Overriding defaults with environment variables ---
  char *env;
  if ((env = getenv("NS_FG_COLOR")) != NULL) {
    text_color = ns_get_color(env);
  }

  if ((env = getenv("NS_BG_COLOR")) != NULL) {
    bg_color = ns_get_color(env);
  }

  if ((env = getenv("NS_PREFERRED_FONT")) != NULL) {
    ns_get_font_path(font, env);
  }

  if ((env = getenv("NS_FONT_SIZE")) != NULL) {
    fontsiz = SDL_strtol(env, NULL, 10);
  }

  // --- Parsing command line arguments ---
  while ((opt = getopt(argc, argv, ":hVt:b:f:p")) != -1) {
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

    case 's':
      fontsiz = SDL_strtol(env, NULL, 10);
      break;

    case 'p':
      show_progressbar = false;
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

    case 'V':
      puts("NeoSent v" NS_VERSION);
      return 0;
      break;
    }
  }

  char *stylesheet = argv[optind];

  // --- Parsing presentation file ---
  parser = ns_parser_new(stylesheet);
  token_vec = vec_new();
  ns_parser_parse(&parser, token_vec);

  signal(SIGINT, ns_cleanup);
  signal(SIGABRT, ns_cleanup);
  signal(SIGTERM, ns_cleanup);
  signal(SIGTSTP, ns_cleanup);

  // --- Presenting contents of file ---
  renderer = ns_renderer_create(stylesheet, font, fontsiz, text_color, bg_color,
                                token_vec, show_progressbar);
  SDL_Event e;
  size_t page = 0;
  for (;;) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        goto cleanup;
      }

      if (e.type == SDL_MOUSEBUTTONDOWN) {
        switch (e.button.button) {
        case SDL_BUTTON_LEFT:
          if (page + 1 < vec_len(token_vec)) {
            ++page;
          }
          break;

        case SDL_BUTTON_RIGHT:
          if (page != 0) {
            --page;
          }
          break;
        }
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
  ns_cleanup(0);
  return 0;
}
