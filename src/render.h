#ifndef RENDER_H
#define RENDER_H
#include "vector.h"
#include "hashmap.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Color fg;
  SDL_Color bg;
  TTF_Font *font;
  struct hashmap *img_texture_cache;
  size_t font_size;
  size_t curr_page;
  size_t total_pages;
  bool is_fullscreen;
  bool show_progressbar;
  SDL_Point win_size;
} ns_Renderer;

// Sets up and initializes a renderer and its window
ns_Renderer ns_renderer_create(char *title, char *font_file, size_t font_size,
                               uint32_t fg, uint32_t bg, vec_Vector *token_vec,
                               bool show_progressbar);

// Deallocates all resources used by the renderer
void ns_renderer_destroy(ns_Renderer *restrict renderer);

// Interprets the tokens and draws the given page onto the screen
void ns_renderer_draw(ns_Renderer *renderer, const vec_Vector *token_vec,
                      const size_t page);

// Toggle between fullscreen and windowed mode
void ns_renderer_toggle_fullscreen(ns_Renderer *renderer);

// Inverts background and foreground colors
void ns_renderer_invert_colors(ns_Renderer *renderer);

#endif
