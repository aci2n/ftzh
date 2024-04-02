#include "stage.h"
#include "SDL_render.h"
#include "defs.h"
#include "draw.h"
#include "entity.h"

typedef struct BulletCache BulletCache;
struct BulletCache {
  SDL_Texture *texture;
  int w;
  int h;
};

struct Stage {
  SDL_Renderer *renderer;
  InputState *input_state;
  Entity *fighter_tail;
  Entity *bullet_tail;
  BulletCache bullet_cache;
};

static void fire_bullet(Stage *stage) {
  Entity *player = stage->fighter_tail;
  Entity *bullet = malloc(sizeof(Entity));
  BulletCache *cache = &stage->bullet_cache;

  (*bullet) = (Entity){
      .x = player->x,
      .y = player->y + player->h / 2.0 - cache->h / 2.0,
      .dx = PLAYER_BULLET_SPEED,
      .w = cache->w,
      .h = cache->h,
      .texture = cache->texture,
      .health = 1,
      .next = stage->bullet_tail,
  };

  printf("fire bullet: bullet=%p tail=%p\n", bullet, stage->bullet_tail);
  stage->bullet_tail = bullet;
}

static void do_player(Stage *stage) {
  Entity *player = stage->fighter_tail;
  InputState *input_state = stage->input_state;

  player->dx = 0;
  player->dy = 0;
  if (player->reload > 0) {
    player->reload--;
  }
  if (input_state[SDL_SCANCODE_UP]) {
    player->dy = -PLAYER_SPEED;
  }
  if (input_state[SDL_SCANCODE_DOWN]) {
    player->dy = PLAYER_SPEED;
  }
  if (input_state[SDL_SCANCODE_RIGHT]) {
    player->dx = PLAYER_SPEED;
  }
  if (input_state[SDL_SCANCODE_LEFT]) {
    player->dx = -PLAYER_SPEED;
  }
  if (input_state[SDL_SCANCODE_LCTRL] && player->reload == 0) {
    fire_bullet(stage);
  }
  player->x += player->dx;
  player->y += player->dy;
}

static void do_bullets(Stage *stage) {
  Entity **prev = &stage->bullet_tail;

  for (Entity *bullet = stage->bullet_tail; bullet;) {
    Entity **next = &bullet->next;
    bullet->x += bullet->dx;
    bullet->y += bullet->dy;
    if (bullet->x >= SCREEN_WIDTH) {
      *prev = *next;
      free(bullet);
    } else {
      prev = next;
    }
    bullet = *next;
  }
}

static void logic(Stage *stage) {
  do_player(stage);
  do_bullets(stage);
}

static void blit_list(SDL_Renderer *renderer, Entity *head) {
  for (Entity *entity = head; entity; entity = entity->next) {
    blit(renderer, entity->texture, entity->x, entity->y);
  }
}

static void draw(Stage *stage) {
  blit_list(stage->renderer, stage->fighter_tail);
  blit_list(stage->renderer, stage->bullet_tail);
}

Stage *stage_new(App *app) { return stage_init(malloc(sizeof(Stage)), app); }

Stage *stage_init(Stage *stage, App *app) {
  Entity *player = malloc(sizeof(Entity));
  SDL_Renderer *renderer = app_get_renderer(app);

  (*player) = (Entity){
      .x = 100,
      .y = 100,
      .texture = load_texture(renderer, "gfx/player.png"),
  };
  (*stage) = (Stage){
      .renderer = renderer,
      .fighter_tail = player,
      .bullet_cache =
          (BulletCache){
              .texture = load_texture(renderer, "gfx/bullet.png"),
          },
      .input_state = app_get_input_state(app),
  };
  SDL_QueryTexture(stage->bullet_cache.texture, 0, 0, &stage->bullet_cache.w,
                   &stage->bullet_cache.h);

  return stage;
}

void stage_invoke(Stage *stage) {
  logic(stage);
  draw(stage);
}

void stage_destroy(Stage *stage) {
  for (Entity *fighter = stage->fighter_tail; fighter;) {
    Entity *next = fighter->next;
    SDL_DestroyTexture(fighter->texture);
    free(fighter);
    fighter = next;
  }
  for (Entity *bullet = stage->bullet_tail; bullet;) {
    Entity *next = bullet->next;
    free(bullet);
    bullet = next;
  }
  SDL_DestroyTexture(stage->bullet_cache.texture);
  free(stage);
}
