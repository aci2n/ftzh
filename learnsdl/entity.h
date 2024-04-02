#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

typedef struct Entity Entity;

Entity* entity_new(int, int, SDL_Texture*);
Entity* entity_init(Entity*, int, int, SDL_Texture*);

#endif
