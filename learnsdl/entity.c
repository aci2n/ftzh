#include "entity.h"
#include "tools.h"

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
