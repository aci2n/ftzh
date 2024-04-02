#include "entity.h"
#include <SDL.h>
#include <stdlib.h>

struct Entity {
  int x;
  int y;
  SDL_Texture *texture;
};

Entity *entity_new(int x, int y, SDL_Texture *texture) {
  return entity_init(malloc(sizeof(Entity)), x, y, texture);
}

Entity *entity_init(Entity *entity, int x, int y, SDL_Texture *texture) {
  (*entity) = (Entity){
      .x = x,
      .y = y,
      .texture = texture,
  };
  return entity;
}

SDL_Texture *entity_get_texture(Entity *entity) { return entity->texture; }

int entity_get_x(Entity *entity) { return entity->x; }

int entity_get_y(Entity *entity) { return entity->y; }

void entity_move(Entity *entity, int dx, int dy) {
	entity->x += dx;
	entity->y += dy;
}
