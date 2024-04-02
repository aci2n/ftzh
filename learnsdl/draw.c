#include "draw.h"
#include <SDL_image.h>
#include <SDL_log.h>
#include <SDL_render.h>

SDL_Texture *load_texture(SDL_Renderer *renderer, char const *filename) {
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Loading %s", filename);
  return IMG_LoadTexture(renderer, filename);
}

void blit(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y) {
  SDL_Rect rect = {
      .x = x,
      .y = y,
  };
  SDL_QueryTexture(texture, 0, 0, &rect.w, &rect.h);
  SDL_RenderCopy(renderer, texture, 0, &rect);
}

void prepare_scene(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 96, 128, 255, 255);
  SDL_RenderClear(renderer);
}

void present_scene(SDL_Renderer *renderer) { SDL_RenderPresent(renderer); }
