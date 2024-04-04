#include "entity.h"
#include "tools.h"

bool entity_collision(Entity const a[restrict static 1],
                      Entity const b[restrict static 1]) {
  return collision(a->x, a->y, a->texture.w, a->texture.h, b->x, b->y,
                   b->texture.w, b->texture.h);
}

void entity_log(Entity const e[static 1]) {
  SDL_LogInfo(0, "{Entity[x=%f,y=%f,dx=%f,dy=%f,w=%d,h=%d]}\n", e->x, e->y,
              e->dx, e->dy, e->texture.w, e->texture.h);
}
