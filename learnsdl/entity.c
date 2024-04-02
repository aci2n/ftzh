#include "entity.h"
#include <SDL.h>
#include <stdlib.h>

struct Entity {
  int x;
  int y;
  int dx;
  int dy;
  int health;
  SDL_Texture *texture;
};

Entity *entity_new(int x, int y, SDL_Texture *texture) {
  return entity_init(malloc(sizeof(Entity)), x, y, 0, 0, 0, texture);
}

Entity *entity_init(Entity *entity, int x, int y, int dx, int dy, int health,
                    SDL_Texture *texture) {
  (*entity) = (Entity){
      .x = x,
      .y = y,
      .dx = dx,
      .dy = dy,
      .health = health,
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

int entity_get_health(Entity *entity) { return entity->health; }

int entity_get_dx(Entity *entity) { return entity->dx; }

int entity_get_dy(Entity *entity) { return entity->dy; }

void entity_set_health(Entity *entity, int health) { entity->health = health; }
