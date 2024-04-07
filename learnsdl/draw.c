#include "draw.h"
#include <SDL_image.h>
#include <SDL_log.h>
#include <SDL_render.h>

SDL_Texture *load_texture(SDL_Renderer *renderer, char const *filename) {
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Loading %s", filename);
  return IMG_LoadTexture(renderer, filename);
}

void blit(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *rect) {
  SDL_RenderCopy(renderer, texture, 0, rect);
}

void blit_rect(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src,
               int x, int y) {
  SDL_Rect dest = (SDL_Rect){
      .x = x,
      .y = y,
      .w = src->w,
      .h = src->h,
  };
  SDL_RenderCopy(renderer, texture, src, &dest);
}

void prepare_scene(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 96, 128, 255, 255);
  SDL_RenderClear(renderer);
}

void present_scene(SDL_Renderer *renderer) { SDL_RenderPresent(renderer); }
