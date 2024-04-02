#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

typedef struct Entity Entity;

Entity *entity_new(float, float, SDL_Texture *);
Entity *entity_init(Entity *, float, float, float, float, int, SDL_Texture *);
SDL_Texture *entity_get_texture(Entity *);
float entity_get_x(Entity *);
float entity_get_y(Entity *);
void entity_move(Entity *, float dx, float dy);
int entity_get_health(Entity *);
float entity_get_dx(Entity *);
float entity_get_dy(Entity *);
void entity_set_health(Entity *, int);
void entity_set_next(Entity *, Entity *);
void entity_set_dx(Entity *, float);
void entity_set_dy(Entity *, float);
int entity_get_reload(Entity *);
void entity_set_reload(Entity *, int);
int entity_get_h(Entity *);
int entity_get_w(Entity *);
void entity_set_y(Entity *, float);
void entity_set_x(Entity *, float);
void entity_destroy(Entity *);
Entity *entity_get_next(Entity *);

#endif
