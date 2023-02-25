#include "render.h"
#include "parser.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <string.h>

ns_Renderer ns_renderer_create(char *title) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fputs(SDL_GetError(), stderr);
    exit(1);
  }

  const int img_flags =
      IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP | IMG_INIT_AVIF;
  if (IMG_Init(img_flags) != img_flags) {
    fputs("Error: Failed to initialize image subsystems.", stderr);
    exit(1);
  }

  if (TTF_Init() != 0) {
    fputs("Error: Failed to initialize font rendering subsystem.", stderr);
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
      .window = window,
      .renderer = renderer,
      .fg = {0xFF, 0xFF, 0xFF, 0xFF},
      .bg = {0, 0, 0, 0xFF},
  };
}

void ns_renderer_destroy(ns_Renderer *renderer) {
  SDL_DestroyRenderer(renderer->renderer);
  SDL_DestroyWindow(renderer->window);
  IMG_Quit();
  SDL_Quit();
  TTF_Quit();
}

void ns_renderer_draw_img(ns_Renderer *renderer, ns_Item *item) {
  SDL_Surface *image_surface = IMG_Load(item->content);
  SDL_Texture *image_texture =
      SDL_CreateTextureFromSurface(renderer->renderer, image_surface);

  SDL_Rect image_rect = (SDL_Rect){
      .x = 0,
      .y = 0,
  };

  SDL_QueryTexture(image_texture, NULL, NULL, &image_rect.w, &image_rect.h);
  SDL_Point win_size;
  SDL_GetWindowSize(renderer->window, &win_size.x, &win_size.y);
  if (image_rect.w > win_size.x || image_rect.h > win_size.y) {
    // TODO make image maintain ration when scaled down to fit
    image_rect.w = win_size.x;
    image_rect.h = win_size.y;
  }

  SDL_RenderCopy(renderer->renderer, image_texture, NULL, &image_rect);
  SDL_DestroyTexture(image_texture);
  SDL_FreeSurface(image_surface);
}

void ns_renderer_draw_paragraph(ns_Renderer *renderer, ns_Item *item) {
  TTF_Font *font = TTF_OpenFont("NotoSans-Regular.ttf", 30);
  if (!font) {
    fputs("Error: Could not load font.", stderr);
    exit(1);
  }

  SDL_Point win_size;
  SDL_GetWindowSize(renderer->window, &win_size.x, &win_size.y);

  SDL_Surface *text_surface = TTF_RenderText_LCD_Wrapped(
      font, item->content, renderer->fg, renderer->bg, win_size.x);
  SDL_Texture *text_texture =
      SDL_CreateTextureFromSurface(renderer->renderer, text_surface);

  SDL_Rect text_rect = (SDL_Rect){
      .x = 0,
      .y = 0,
  };
  SDL_QueryTexture(text_texture, NULL, NULL, &text_rect.w, &text_rect.h);

  SDL_RenderCopy(renderer->renderer, text_texture, NULL, &text_rect);
  SDL_DestroyTexture(text_texture);
  SDL_FreeSurface(text_surface);
  TTF_CloseFont(font);
}

void ns_renderer_draw(ns_Renderer *renderer, vec_Vector *token_vec,
                      const size_t page) {
  ns_Item item = vec_get(token_vec, page);
  SDL_SetRenderDrawColor(renderer->renderer, renderer->bg.r, renderer->bg.g,
                         renderer->bg.b, renderer->bg.a);
  SDL_RenderClear(renderer->renderer);
  switch (item.type) {
  case NS_IMAGE:
    ns_renderer_draw_img(renderer, &item);
    break;
  case NS_PARAGRAPH:
    ns_renderer_draw_paragraph(renderer, &item);
    break;
  case NS_EMPTYSLIDE:
    // doing nothing will make the slide empty
    break;
  }

  SDL_RenderPresent(renderer->renderer);
}
