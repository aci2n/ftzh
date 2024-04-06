#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <stdbool.h>

typedef struct Entity Entity;
struct Entity {
  float x;
  float y;
	int w;
	int h;
  float dx;
  float dy;
  int health;
  int reload;
  SDL_Texture *texture;
  Entity *next;
  int side;
};

typedef struct Explosion Explosion;
struct Explosion {
  float x;
	float y;
	float dx;
	float dy;
	int r, g, b, a;
	Explosion *next;
};

typedef struct Debris Debris;
struct Debris {
  float x;
	float y;
	float dx;
	float dy;
	SDL_Rect rect;
	SDL_Rect texture;
	int life;
	Debris *next;
};

typedef struct Star Star;
struct Star {
  int x;
	int y;
	int speed;
};

bool entity_collision(Entity const[restrict static 1],
                      Entity const[restrict static 1]);
void entity_log(Entity const[static 1]);

#endif
