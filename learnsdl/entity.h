#ifndef ENTITY_H
#define ENTITY_H

#include "tools.h"
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

bool entity_collision(Entity const a[static 1], Entity const b[static 1]) {
	if (a->side == b->side) {
		return false;
	}
	if (a->health <= 0 || b->health <= 0) {
		return false;
	}
  return collision(a->x, a->y, a->texture.w, a->texture.h, b->x, b->y,
                   b->texture.w, b->texture.h);
}

#endif
