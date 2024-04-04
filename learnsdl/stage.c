#include "stage.h"
#include "defs.h"
#include "draw.h"
#include "entity.h"
#include "tools.h"
#include <SDL.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <math.h>
#include <stdlib.h>

enum {
  TEXTURE_PLAYER,
  TEXTURE_ENEMY,
  TEXTURE_BULLET,
  TEXTURE_ENEMY_BULLET,
  TEXTURE_COUNT,
};

struct Stage {
  SDL_Renderer *renderer;
  InputState *input_state;
  SDL_Texture *textures[TEXTURE_COUNT];

  Entity player;
  Entity *fighter_tail;
  Entity *bullet_tail;
  int enemy_spawn_timer;
  int reset_timer;
};

static void clear_entities(Stage *stage) {
  for (Entity *fighter = stage->fighter_tail; fighter;) {
    Entity *next = fighter->next;
    free(fighter);
    fighter = next;
  }
  stage->fighter_tail = 0;
  for (Entity *bullet = stage->bullet_tail; bullet;) {
    Entity *next = bullet->next;
    free(bullet);
    bullet = next;
  }
  stage->bullet_tail = 0;
}

static Entity create_player(SDL_Texture *texture) {
  int w = 0;
  int h = 0;
  SDL_QueryTexture(texture, 0, 0, &w, &h);

  return (Entity){
      .x = PLAYER_START_X,
      .y = PLAYER_START_Y,
      .w = w,
      .h = w,
      .health = PLAYER_HEALTH,
      .side = SIDE_PLAYER,
      .texture = texture,
  };
}

static void stage_reset(Stage *stage) {
  clear_entities(stage);
  stage->enemy_spawn_timer = 0;
  stage->reset_timer = FPS * 2;
  stage->player = create_player(stage->textures[TEXTURE_PLAYER]);
}

static void fire_bullet(Stage *stage, float offset_y) {
  Entity const *player = &stage->player;
  Entity *bullet = malloc(sizeof(Entity));
  SDL_Texture *texture = stage->textures[TEXTURE_BULLET];
  int w = 0;
  int h = 0;
  SDL_QueryTexture(texture, 0, 0, &w, &h);

  (*bullet) = (Entity){
      .x = player->x + player->w,
      .y = player->y + player->h / 2.0 - offset_y * h,
      .w = w,
      .h = h,
      .dx = PLAYER_BULLET_SPEED,
      .dy = randf() * 2 - 1,
      .texture = texture,
      .health = 1,
      .next = stage->bullet_tail,
      .side = SIDE_PLAYER,
  };

  /* printf("fire bullet: bullet=%p tail=%p\n", bullet, stage->bullet_tail); */
  stage->bullet_tail = bullet;
}

static void do_player(Stage *stage) {
  Entity *player = &stage->player;
  InputState const *input_state = stage->input_state;
  float speed = PLAYER_SPEED;

  player->dx = 0;
  player->dy = 0;
  if (player->reload > 0) {
    player->reload--;
  }
  if (input_state[SDL_SCANCODE_LSHIFT]) {
    speed *= PLAYER_SPEEDUP_RATE;
  }
  if (input_state[SDL_SCANCODE_UP]) {
    player->dy = -speed;
  }
  if (input_state[SDL_SCANCODE_DOWN]) {
    player->dy = speed;
  }
  if (input_state[SDL_SCANCODE_RIGHT]) {
    player->dx = speed;
  }
  if (input_state[SDL_SCANCODE_LEFT]) {
    player->dx = -speed;
  }
  if (input_state[SDL_SCANCODE_LCTRL] && player->reload == 0) {
    fire_bullet(stage, -1.5);
    fire_bullet(stage, 0.5);
    player->reload = PLAYER_RELOAD_RATE;
  }
  player->x =
      min_int(SCREEN_WIDTH - player->w, max_int(0, player->x + player->dx));
  player->y =
      min_int(SCREEN_HEIGHT - player->h, max_int(0, player->y + player->dy));
}

static void do_bullets(Stage *stage) {
  Entity **prev = &stage->bullet_tail;

  for (Entity *bullet = *prev; bullet;) {
    Entity **next = &bullet->next;
    bullet->x += bullet->dx;
    bullet->y += bullet->dy;
    if (bullet->health <= 0 || bullet->x >= SCREEN_WIDTH) {
      *prev = *next;
      free(bullet);
    } else {
      prev = next;
    }
    bullet = *next;
  }
}

static void do_fighters(Stage *stage) {
  Entity **prev = &stage->fighter_tail;

  for (Entity *fighter = *prev; fighter;) {
    Entity **next = &fighter->next;
    fighter->x += fighter->dx;
    fighter->y += sin(fighter->x / ENEMY_DY_FACTOR) * fighter->dy;
    if (fighter->health <= 0 || fighter->x <= -fighter->w) {
      *prev = *next;
      free(fighter);
    } else {
      prev = next;
    }
    fighter = *next;
  }
}

static void spawn_enemies(Stage *stage) {
  if (stage->enemy_spawn_timer > 0) {
    stage->enemy_spawn_timer--;
    return;
  }

  Entity *enemy = malloc(sizeof(Entity));
  SDL_Texture *texture = stage->textures[TEXTURE_ENEMY];
  int w = 0;
  int h = 0;
  SDL_QueryTexture(texture, 0, 0, &w, &h);

  (*enemy) = (Entity){
      .x = SCREEN_WIDTH,
      .y = rand() % SCREEN_HEIGHT,
      .w = w,
      .h = h,
      .dx = -(ENEMY_DX_MIN + rand() % (ENEMY_DX_MAX - ENEMY_DX_MIN)),
      .dy = randf() * ENEMY_DY_MAX,
      .texture = stage->textures[TEXTURE_ENEMY],
      .next = stage->fighter_tail,
      .side = SIDE_ENEMY,
      .health = ENEMY_STARTING_HEALTH,
  };

  stage->fighter_tail = enemy;
  stage->enemy_spawn_timer =
      ENEMY_BASE_SPAWN_RATE + rand() % ENEMY_RAND_SPAWN_RATE;
}

static void do_collisions(Stage *stage) {
  Entity *player = &stage->player;

  for (Entity *bullet = stage->bullet_tail; bullet; bullet = bullet->next) {
    for (Entity *enemy = stage->fighter_tail; enemy; enemy = enemy->next) {
      if (entity_collision(bullet, enemy)) {
        enemy->health--;
        if (--bullet->health <= 0) {
          break;
        }
      }
    }
  }

  for (Entity *enemy = stage->fighter_tail; enemy; enemy = enemy->next) {
    if (entity_collision(enemy, player)) {
      if (--player->health <= 0) {
        break;
      }
    }
  }
}

static void do_reset(Stage *stage) {
  if (stage->player.health <= 0 && --stage->reset_timer <= 0) {
    stage_reset(stage);
  }
}

static void logic(Stage *stage) {
  do_reset(stage);
  do_collisions(stage);
  do_player(stage);
  do_fighters(stage);
  do_bullets(stage);
  spawn_enemies(stage);
}

static void blit_list(SDL_Renderer *renderer, Entity *head) {
  for (Entity *entity = head; entity; entity = entity->next) {
    SDL_Rect rect = {
        .x = entity->x,
        .y = entity->y,
        .w = entity->w,
        .h = entity->h,
    };
    blit(renderer, entity->texture, &rect);
  }
}

static void draw(Stage *stage) {
  blit_list(stage->renderer, &stage->player);
  blit_list(stage->renderer, stage->fighter_tail);
  blit_list(stage->renderer, stage->bullet_tail);
}

Stage *stage_new(App *app) { return stage_init(malloc(sizeof(Stage)), app); }

Stage *stage_init(Stage *stage, App *app) {
  SDL_Renderer *renderer = app_get_renderer(app);

  (*stage) = (Stage){
      .renderer = renderer,
      .input_state = app_get_input_state(app),
      .textures =
          {
              [TEXTURE_PLAYER] = load_texture(renderer, "gfx/player.png"),
              [TEXTURE_BULLET] = load_texture(renderer, "gfx/bullet.png"),
              [TEXTURE_ENEMY] = load_texture(renderer, "gfx/enemy.png"),
              [TEXTURE_ENEMY_BULLET] =
                  load_texture(renderer, "gfx/enemy_bullet.png"),
          },
  };
  stage_reset(stage);

  return stage;
}

void stage_invoke(Stage *stage) {
  logic(stage);
  draw(stage);
}

void stage_destroy(Stage *stage) {
  clear_entities(stage);
  for (size_t i = 0; i < TEXTURE_COUNT; i++) {
    SDL_DestroyTexture(stage->textures[i]);
  }
  free(stage);
}
