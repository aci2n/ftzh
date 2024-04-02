#include "entity.h"
#include <SDL.h>
#include <stdlib.h>

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

Entity *entity_new(float x, float y, SDL_Texture *texture) {
  return entity_init(malloc(sizeof(Entity)), x, y, 0, 0, 0, texture);
}

Entity *entity_init(Entity *entity, float x, float y, float dx, float dy,
                    int health, SDL_Texture *texture) {
  (*entity) = (Entity){
      .x = x,
      .y = y,
      .dx = dx,
      .dy = dy,
      .health = health,
      .texture = texture,
  };
  SDL_QueryTexture(texture, 0, 0, &entity->w, &entity->h);
  return entity;
}

SDL_Texture *entity_get_texture(Entity *entity) { return entity->texture; }

float entity_get_x(Entity *entity) { return entity->x; }

float entity_get_y(Entity *entity) { return entity->y; }

void entity_move(Entity *entity, float dx, float dy) {
  entity->x += dx;
  entity->y += dy;
}

int entity_get_health(Entity *entity) { return entity->health; }

float entity_get_dx(Entity *entity) { return entity->dx; }

float entity_get_dy(Entity *entity) { return entity->dy; }

void entity_set_health(Entity *entity, int health) { entity->health = health; }

void entity_set_next(Entity *entity, Entity *next) { entity->next = next; }

void entity_set_dx(Entity *entity, float dx) { entity->dx = dx; }

void entity_set_dy(Entity *entity, float dy) { entity->dy = dy; }

int entity_get_reload(Entity *entity) { return entity->reload; }

void entity_set_reload(Entity *entity, int reload) { entity->reload = reload; }

int entity_get_h(Entity *entity) { return entity->h; }

int entity_get_w(Entity *entity) { return entity->w; }

void entity_set_x(Entity *entity, float x) { entity->x = x; }

void entity_set_y(Entity *entity, float y) { entity->y = y; }

Entity *entity_get_next(Entity *entity) {
	return entity->next;
}

void entity_destroy(Entity *entity) {
	free(entity);
}
