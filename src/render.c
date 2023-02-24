#include <SDL2/SDL.h>

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    size_t curr_page;
} ns_Renderer;

void ns_renderer_create(void);
void ns_renderer_destroy(void);
void ns_renderer_draw_img(void);
void ns_renderer_draw_paragraph(void);
void ns_renderer_draw_emptyslide(void);
