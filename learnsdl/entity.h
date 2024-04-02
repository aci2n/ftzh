#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

typedef struct Entity Entity;
struct Entity {
  int w;
  int h;
  float x;
  float y;
  float dx;
  float dy;
  int health;
  int reload;
  SDL_Texture *texture;
  Entity *next;
};

#endif
