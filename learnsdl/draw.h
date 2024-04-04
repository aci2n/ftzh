#ifndef DRAW_H
#define DRAW_H

#include <SDL.h>

SDL_Texture *load_texture(SDL_Renderer *renderer, char const *filename);
void blit(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *rect);
void prepare_scene(SDL_Renderer *renderer);
void present_scene(SDL_Renderer *renderer);

#endif
