#include "parser.h"
#include "render.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fputs("Error: No stylesheet file was provided.\n", stderr);
    return 1;
  }

  // Parsing presentation file
  ns_Parser parser = ns_parser_new(argv[1]);
  vec_Vector *token_vec = vec_new();
  ns_parser_parse(&parser, token_vec);

  // Presenting contents of file
  ns_Renderer renderer = ns_renderer_create(argv[1]);
  SDL_Event e;
  size_t page = 0;
  for (;;) {
    ns_renderer_draw(&renderer, token_vec, page);
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        goto cleanup;
      }

      switch (e.key.keysym.sym) {
      case SDLK_RIGHT:
      case 'l':
        if (page + 1 < vec_len(token_vec))
          page++;
        break;

      case SDLK_LEFT:
      case 'h':
        if (page == 0) {
          continue;
        }
        page--;
        break;

      case 'q':
        goto cleanup;
        break;
      }
    }
  }

  // Final cleanup
cleanup:
  ns_renderer_destroy(&renderer);
  ns_parser_free(&parser, token_vec);
  return 0;
}
