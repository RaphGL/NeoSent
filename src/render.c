#include "render.h"
#include "parser.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <stdio.h>

ns_Renderer ns_renderer_create(char *title) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fputs(SDL_GetError(), stderr);
    exit(1);
  }

  SDL_Window *window =
      // TODO make window fullscreen
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       1536, 864, SDL_WINDOW_SHOWN);
  if (!window) {
    fputs("Error: Could not create window.", stderr);
    exit(1);
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    fputs(SDL_GetError(), stderr);
    exit(1);
  }

  return (ns_Renderer){
      .curr_page = 1,
      .window = window,
      .renderer = renderer,
  };
}

void ns_renderer_destroy(ns_Renderer *renderer) {
  SDL_DestroyRenderer(renderer->renderer);
  SDL_DestroyWindow(renderer->window);
  SDL_Quit();
  renderer->curr_page = 1;
}

void ns_renderer_draw_img(SDL_Renderer *renderer, ns_Item *item) {}
void ns_renderer_draw_paragraph(SDL_Renderer *renderer, ns_Item *item) {}
void ns_renderer_draw_emptyslide(SDL_Renderer *renderer, ns_Item *item) {}

void ns_renderer_draw(ns_Renderer *renderer, vec_Vector *token_vec,
                      size_t page) {
  ns_Item item = vec_get(token_vec, page);
  switch (item.type) {
  case NS_IMAGE:
    ns_renderer_draw_img(renderer->renderer, &item);
    break;
  case NS_PARAGRAPH:
    ns_renderer_draw_paragraph(renderer->renderer, &item);
    break;
  case NS_EMPTYSLIDE:
    ns_renderer_draw_emptyslide(renderer->renderer, &item);
    break;
  }
}
