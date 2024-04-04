#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <stdbool.h>

typedef struct EntityTexture EntityTexture;
struct EntityTexture {
  SDL_Texture *ref;
  int w;
  int h;
};

typedef struct Entity Entity;
struct Entity {
  float x;
  float y;
  float dx;
  float dy;
  int health;
  int reload;
  EntityTexture texture;
  Entity *next;
  int side;
};

bool entity_collision(Entity const[static 1], Entity const[static 1]);

#endif
