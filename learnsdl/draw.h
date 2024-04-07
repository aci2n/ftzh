#ifndef DRAW_H
#define DRAW_H

#include <SDL.h>

SDL_Texture *load_texture(SDL_Renderer *renderer, char const *filename);
void blit(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *rect);
void blit_rect(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src,
               int x, int y);
void prepare_scene(SDL_Renderer *renderer);
void present_scene(SDL_Renderer *renderer);

#define BLIT_LIST(RENDERER, TYPE, HEAD)                                        \
  do {                                                                         \
    for (TYPE *curr = (HEAD); curr; curr = curr->next) {                       \
      SDL_Rect rect = {                                                        \
          .x = curr->x,                                                        \
          .y = curr->y,                                                        \
          .w = curr->w,                                                        \
          .h = curr->h,                                                        \
      };                                                                       \
      blit((RENDERER), curr->texture, &rect);                                  \
    }                                                                          \
  } while (false)

#endif
