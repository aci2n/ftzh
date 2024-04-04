#include "entity.h"
#include "tools.h"

bool entity_collision(Entity const a[restrict static 1],
                      Entity const b[restrict static 1]) {
  return collision(a->x, a->y, a->w, a->h, b->x, b->y, b->w, b->h);
}

void entity_log(Entity const e[static 1]) {
  SDL_LogInfo(0, "{Entity[x=%f,y=%f,dx=%f,dy=%f,w=%d,h=%d]}\n", e->x, e->y,
              e->dx, e->dy, e->w, e->h);
}
