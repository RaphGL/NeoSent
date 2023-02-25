#ifndef RENDER_H
#define RENDER_H
#include "vector.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stddef.h>

typedef struct {
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Color fg;
  SDL_Color bg;
  TTF_Font *font;
} ns_Renderer;

// Sets up and initializes a renderer and its window
ns_Renderer ns_renderer_create(char *title);

// Deallocates all resources used by the renderer
void ns_renderer_destroy(ns_Renderer *restrict renderer);

// Interprets the tokens and draws the given page onto the screen
void ns_renderer_draw(const ns_Renderer *renderer, const vec_Vector *token_vec,
                      const size_t page);

#endif
