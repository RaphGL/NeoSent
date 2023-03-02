#include "render.h"
#include "parser.h"
#include "utils.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static inline void
ns_renderer_check_image_validity(const vec_Vector *token_vec) {
  bool is_valid = true;
  for (size_t i = 0; i < vec_len(token_vec); i++) {
    ns_Item token = vec_get(token_vec, i);

    if (token.type == NS_IMAGE) {
      if (access(token.content, F_OK) != 0) {
        fprintf(stderr, "Error: %s declared at %d:%d does not exist.\n",
                token.content, token.linenum, token.colnum);
        is_valid = false;
      }
    }
  }

  if (!is_valid) {
    exit(1);
  }
}

ns_Renderer ns_renderer_create(char *title, char *font_file, uint32_t fg,
                               uint32_t bg, vec_Vector *token_vec,
                               bool show_progressbar) {
  ns_renderer_check_image_validity(token_vec);

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

  char win_title[BUFSIZ] = "NeoSent - ";
  strncat(win_title, title, BUFSIZ - strlen(win_title));
  SDL_Window *window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       1536, 864, SDL_WINDOW_RESIZABLE);
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

  TTF_Font *font = TTF_OpenFont(font_file, 16);
  if (!font) {
    fputs("Error: Could not load font.", stderr);
    exit(1);
  }

  SDL_Color bg_color;
  SDL_Color fg_color;

  if (ns_is_lil_endian()) {
    bg_color = (SDL_Color){
        .r = (0x000000FF & bg),
        .g = (0x0000FF00 & bg) >> 8,
        .b = (0x00FF0000 & bg) >> 16,
        .a = 0xFF,
    };

    fg_color = (SDL_Color){
        .r = (0x000000FF & fg),
        .g = (0x0000FF00 & fg) >> 8,
        .b = (0x00FF0000 & fg) >> 16,
        .a = 0xFF,
    };
  } else {
    bg_color = (SDL_Color){
        .r = (0xFF000000 & bg),
        .g = (0x00FF0000 & bg) << 8,
        .b = (0x0000FF00 & bg) << 16,
        .a = 0xFF,
    };

    fg_color = (SDL_Color){
        .r = (0x000000FF & fg),
        .g = (0x0000FF00 & fg) >> 8,
        .b = (0x00FF0000 & fg) >> 16,
        .a = 0xFF,
    };
  }

  return (ns_Renderer){
      .window = window,
      .renderer = renderer,
      .fg = fg_color,
      .bg = bg_color,
      .font = font,
      .is_fullscreen = false,
      .total_pages = vec_len(token_vec),
      .show_progressbar = show_progressbar,
  };
}

void ns_renderer_destroy(ns_Renderer *restrict renderer) {
  SDL_DestroyRenderer(renderer->renderer);
  SDL_DestroyWindow(renderer->window);
  IMG_Quit();
  SDL_Quit();
  TTF_CloseFont(renderer->font);
  TTF_Quit();
}

void ns_renderer_toggle_fullscreen(ns_Renderer *renderer) {
  if (renderer->is_fullscreen) {
    if (SDL_SetWindowFullscreen(renderer->window, 0) == 0) {
      renderer->is_fullscreen = false;
      SDL_ShowCursor(SDL_ENABLE);
    }
  } else {
    if (SDL_SetWindowFullscreen(renderer->window,
                                SDL_WINDOW_FULLSCREEN_DESKTOP) == 0) {
      renderer->is_fullscreen = true;
      SDL_ShowCursor(SDL_DISABLE);
    }
  }
}

void ns_renderer_invert_colors(ns_Renderer *renderer) {
  SDL_Color tmp = renderer->fg;
  renderer->fg = renderer->bg;
  renderer->bg = tmp;
}

static void ns_renderer_draw_progressbar(const ns_Renderer *renderer) {
  SDL_Point win_size = renderer->win_size;
  int progress_w = win_size.x / renderer->total_pages;
  if (renderer->curr_page == 0) {
    progress_w = 0;
  }
  SDL_Rect bar = (SDL_Rect){
      .y = win_size.y - win_size.y * 0.005,
      .x = 0,
      .w = progress_w * (renderer->curr_page + 1),
      .h = win_size.y * 0.005,
  };

  SDL_SetRenderDrawColor(renderer->renderer, renderer->fg.r, renderer->fg.g,
                         renderer->fg.b, renderer->fg.a);
  SDL_RenderFillRect(renderer->renderer, &bar);
}

static void ns_renderer_draw_img(const ns_Renderer *renderer,
                                 const ns_Item *item) {
  SDL_Surface *image_surface = IMG_Load(item->content);
  if (!image_surface) {
    fprintf(stderr, "Error: Could not read image %s defined at %d:%d\n",
            item->content, item->linenum, item->colnum);
    return;
  }

  SDL_Texture *image_texture =
      SDL_CreateTextureFromSurface(renderer->renderer, image_surface);

  SDL_Rect image_rect = (SDL_Rect){
      .x = 0,
      .y = 0,
  };

  SDL_QueryTexture(image_texture, NULL, NULL, &image_rect.w, &image_rect.h);
  SDL_Point win_size = renderer->win_size;

  // make images bigger than resolution scale down
  if (image_rect.w > win_size.x || image_rect.h > win_size.y) {
    float image_ratio = (float)image_rect.w / image_rect.h;
    image_rect.h = win_size.y;
    image_rect.w = win_size.y * image_ratio;
    image_rect.x = win_size.x / 2 - image_rect.w / 2;
    image_rect.y = win_size.y / 2 - image_rect.h / 2;
    SDL_RenderCopy(renderer->renderer, image_texture, NULL, &image_rect);
    goto cleanup;
  } else {
    image_rect.x = win_size.x / 2 - image_rect.w / 2;
    image_rect.y = win_size.y / 2 - image_rect.h / 2;
  }

  SDL_RenderCopy(renderer->renderer, image_texture, NULL, &image_rect);
cleanup:
  SDL_DestroyTexture(image_texture);
  SDL_FreeSurface(image_surface);
}

static void ns_renderer_draw_paragraph(const ns_Renderer *renderer,
                                       const ns_Item *item) {
  const int text_len = ns_get_longest_line(item->content);
  SDL_Point win_size = renderer->win_size;
  TTF_SetFontSize(renderer->font, win_size.x / text_len);

  TTF_SetFontHinting(renderer->font, TTF_HINTING_LIGHT_SUBPIXEL);
  SDL_Surface *text_surface = TTF_RenderUTF8_Blended_Wrapped(
      renderer->font, item->content, renderer->fg, 0);
  SDL_Texture *text_texture =
      SDL_CreateTextureFromSurface(renderer->renderer, text_surface);

  SDL_Rect text_rect;
  SDL_QueryTexture(text_texture, NULL, NULL, &text_rect.w, &text_rect.h);
  text_rect.x = win_size.x / 2 - text_rect.w / 2;
  text_rect.y = win_size.y / 2 - text_rect.h / 2;

  SDL_RenderCopy(renderer->renderer, text_texture, NULL, &text_rect);
  SDL_DestroyTexture(text_texture);
  SDL_FreeSurface(text_surface);
}

void ns_renderer_draw(ns_Renderer *renderer, const vec_Vector *token_vec,
                      const size_t page) {
  ns_Item item = vec_get(token_vec, page);
  renderer->curr_page = page;
  SDL_GetWindowSize(renderer->window, &renderer->win_size.x,
                    &renderer->win_size.y);

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

  if (renderer->show_progressbar) {
    ns_renderer_draw_progressbar(renderer);
  }

  SDL_RenderPresent(renderer->renderer);
}
