#ifndef RENDER_H
#define RENDER_H
#include <SDL2/SDL_ttf.h>
#include <stddef.h>
#include <SDL2/SDL.h>
#include "vector.h"

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Color fg;
    SDL_Color bg;
    size_t curr_page;
} ns_Renderer;

ns_Renderer ns_renderer_create(char *title);
void ns_renderer_destroy(ns_Renderer *renderer);
void ns_renderer_draw(ns_Renderer *renderer, vec_Vector *token_vec, size_t page);

#endif
