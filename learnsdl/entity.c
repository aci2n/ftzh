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
