#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

typedef struct Entity Entity;

Entity* entity_new(int, int, SDL_Texture*);
Entity* entity_init(Entity*, int, int, SDL_Texture*);
SDL_Texture* entity_get_texture(Entity*);
int entity_get_x(Entity*);
int entity_get_y(Entity*);
void entity_move(Entity*, int dx, int dy);

#endif
