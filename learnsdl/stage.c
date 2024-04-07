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
  TEXTURE_BACKGROUND,
  TEXTURE_EXPLOSION,
  TEXTURE_COUNT,
};

struct Stage {
  SDL_Renderer *renderer;
  InputState *input_state;
  SDL_Texture *textures[TEXTURE_COUNT];

  Entity player;
  Entity *enemy_tail;
  Entity *bullet_tail;
  Entity *enemy_bullet_tail;
  Debris *debris_tail;
  Explosion *explosion_tail;
  int enemy_spawn_timer;
  int reset_timer;
  size_t frame_counter;
  Star stars[MAX_STARS];
};

static void clear_entities(Stage *stage) {
  FREE_LIST(Entity, &stage->enemy_tail, next);
  FREE_LIST(Entity, &stage->bullet_tail, next);
  FREE_LIST(Entity, &stage->enemy_bullet_tail, next);
  FREE_LIST(Debris, &stage->debris_tail, next);
  FREE_LIST(Explosion, &stage->explosion_tail, next);
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

static void init_starfield(Stage *stage) {
  for (size_t i = 0; i < MAX_STARS; i++) {
    stage->stars[i] = (Star){
        .x = rand() % SCREEN_WIDTH,
        .y = rand() % SCREEN_HEIGHT,
        .speed = 1 + rand() % 8,
    };
  }
}

static void stage_reset(Stage *stage) {
  clear_entities(stage);
  init_starfield(stage);
  stage->enemy_spawn_timer = 0;
  stage->reset_timer = FPS * 2;
  stage->frame_counter = 0;
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

static void do_player_collisions(Stage *stage) {
  Entity *player = &stage->player;

  for (Entity *bullet = stage->enemy_bullet_tail; bullet;
       bullet = bullet->next) {
    if (entity_collision(bullet, player)) {
      player->health--;
      bullet->health--;
    }
  }

  for (Entity *enemy = stage->enemy_tail; enemy; enemy = enemy->next) {
    if (entity_collision(enemy, player)) {
      enemy->health--;
      player->health--;
    }
  }
}

static void do_player(Stage *stage) {
  Entity *player = &stage->player;

  if (player->health <= 0) {
    return;
  }

  do_player_collisions(stage);

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

static void do_enemies(Stage *stage) {
  Entity **prev = &stage->enemy_tail;

  for (Entity *enemy = *prev; enemy;) {
    Entity **next = &enemy->next;
    enemy->x += enemy->dx;
    enemy->y += sin(enemy->x / ENEMY_DY_FACTOR) * enemy->dy;
    if (enemy->health <= 0 || enemy->x <= -enemy->w) {
      *prev = *next;
      free(enemy);
    } else {
      prev = next;
    }
    enemy = *next;
  }
}

static void fire_enemy_bullet(Stage *stage, Entity *enemy) {
  Entity *player = &stage->player;
  Entity *bullet = malloc(sizeof(Entity));
  SDL_Texture *texture = stage->textures[TEXTURE_ENEMY_BULLET];
  float dx = 0;
  float dy = 0;
  int w = 0;
  int h = 0;
  SDL_QueryTexture(texture, 0, 0, &w, &h);

  (*bullet) = (Entity){
      .x = enemy->x,
      .y = enemy->y + enemy->h / 2.0 - 0.5 * h,
      .w = w,
      .h = h,
      .texture = texture,
      .health = 1,
      .next = stage->enemy_bullet_tail,
      .side = SIDE_ENEMY,
  };

  calc_slope(player->x, player->y, bullet->x, bullet->y, &dx, &dy);
  bullet->dx = dx * ENEMY_BULLET_SPEED;
  bullet->dy = dy * ENEMY_BULLET_SPEED;
  enemy->reload = (rand() % FPS) * 2;
  stage->enemy_bullet_tail = bullet;
}

static void do_enemy_bullets(Stage *stage) {
  Entity **prev = &stage->enemy_bullet_tail;

  for (Entity *bullet = stage->enemy_bullet_tail; bullet;) {
    Entity **next = &bullet->next;
    bullet->x += bullet->dx;
    bullet->y += bullet->dy;
    if (bullet->health <= 0 || bullet->x <= bullet->w) {
      *prev = *next;
      free(bullet);
    } else {
      prev = next;
    }
    bullet = *next;
  }

  for (Entity *enemy = stage->enemy_tail; enemy; enemy = enemy->next) {
    if (--enemy->reload <= 0) {
      fire_enemy_bullet(stage, enemy);
    }
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
      .next = stage->enemy_tail,
      .side = SIDE_ENEMY,
      .health = ENEMY_STARTING_HEALTH,
  };

  stage->enemy_tail = enemy;
  stage->enemy_spawn_timer =
      ENEMY_BASE_SPAWN_RATE + rand() % ENEMY_RAND_SPAWN_RATE;
}

static void do_collisions(Stage *stage) {
  for (Entity *bullet = stage->bullet_tail; bullet; bullet = bullet->next) {
    for (Entity *enemy = stage->enemy_tail; enemy; enemy = enemy->next) {
      if (entity_collision(bullet, enemy)) {
        bullet->health--;
        enemy->health--;
      }
    }
  }
}

static void do_reset(Stage *stage) {
  if (stage->player.health <= 0 && --stage->reset_timer <= 0) {
    stage_reset(stage);
  }
}

static void do_frame_counter(Stage *stage) { stage->frame_counter++; }

static void do_starfield(Stage *stage) {
  for (size_t i = 0; i < MAX_STARS; i++) {
    Star *star = &stage->stars[i];
    star->x -= star->speed;
    if (star->x < 0) {
      star->x += SCREEN_WIDTH;
    }
  }
}

static void do_explosions(Stage *stage) {
  Explosion **prev = &stage->explosion_tail;
  for (Explosion *curr = *prev; curr;) {
    Explosion **next = &curr->next;
    curr->x -= curr->dx;
    curr->y -= curr->dy;
    if (--curr->a <= 0) {
      *prev = *next;
      free(curr);
    } else {
      prev = next;
    }
    curr = *next;
  }
}

static void add_debris(Stage *stage, Entity *entity) {
  size_t const bound = 2;
  size_t const parts = pow(bound, 2);
  size_t const part_w = entity->w / bound;
  size_t const part_h = entity->h / bound;

  for (size_t i = 0; i < parts; i++) {
    Debris *debris = malloc(sizeof(*debris));
    size_t const idx_x = i / bound;
    size_t const idx_y = i % bound;

    (*debris) = (Debris){
        .dx = randf() * 4 - 2,
        .dy = randf() * 4 - 2,
        .src_x = idx_x * part_w,
        .src_y = idx_y * part_h,
        .dst_x = stage->player.x + idx_x * part_w,
        .dst_y = stage->player.y + idx_y * part_h,
        .w = part_w,
        .h = part_h,
        .life = 10,
        .texture = entity->texture,
        .next = stage->debris_tail,
    };
    stage->debris_tail = debris;
  }
}

static void do_debris(Stage *stage) {
  for (Debris *curr = stage->debris_tail; curr; curr = curr->next) {
    curr->dst_x += curr->dx;
    curr->dst_y += curr->dy;
  }
  if (stage->player.health <= 0 && stage->debris_tail == 0) {
    add_debris(stage, &stage->player);
  }
}

static void logic(Stage *stage) {
  do_reset(stage);
  do_frame_counter(stage);
  do_starfield(stage);
  do_debris(stage);
  do_explosions(stage);
  do_collisions(stage);
  do_player(stage);
  do_enemies(stage);
  do_enemy_bullets(stage);
  do_bullets(stage);
  spawn_enemies(stage);
}

static void draw_background(Stage *stage) {
  SDL_Texture *texture = stage->textures[TEXTURE_BACKGROUND];
  int texture_w = 0;
  int texture_h = 0;
  int offset_x =
      ((stage->frame_counter * BACKGROUND_SPEED) / FPS) % SCREEN_WIDTH;
  SDL_QueryTexture(texture, 0, 0, &texture_w, &texture_h);

  SDL_Rect src1 = (SDL_Rect){
      .x = offset_x,
      .y = 0,
      .w = texture_w - offset_x,
      .h = texture_h,
  };
  SDL_Rect dest1 = (SDL_Rect){
      .x = 0,
      .y = 0,
      .w = texture_w - offset_x,
      .h = SCREEN_HEIGHT,
  };
  SDL_Rect src2 = (SDL_Rect){
      .x = 0,
      .y = 0,
      .w = offset_x,
      .h = texture_h,
  };
  SDL_Rect dest2 = (SDL_Rect){
      .x = SCREEN_WIDTH - offset_x,
      .y = 0,
      .w = offset_x,
      .h = SCREEN_HEIGHT,
  };

  SDL_RenderCopy(stage->renderer, texture, &src1, &dest1);
  SDL_RenderCopy(stage->renderer, texture, &src2, &dest2);
}

static void draw_stars(Stage *stage) {
  for (size_t i = 0; i < MAX_STARS; i++) {
    Star *star = &stage->stars[i];
    int c = star->speed * 32;
    SDL_SetRenderDrawColor(stage->renderer, c, c, c, 255);
    SDL_RenderDrawLine(stage->renderer, star->x, star->y, star->x + 3, star->y);
  }
}

static void draw_debris(Stage *stage) {
  Entity const player = stage->player;
  for (Debris *curr = stage->debris_tail; curr; curr = curr->next) {
    SDL_Rect src = {
        .x = curr->src_x,
        .y = curr->src_y,
        .w = curr->w,
        .h = curr->h,
    };
    blit_rect(stage->renderer, curr->texture, &src, curr->dst_x, curr->dst_y);
  }
}

static void draw(Stage *stage) {
  draw_background(stage);
  draw_stars(stage);
  if (stage->player.health > 0) {
    BLIT_LIST(stage->renderer, Entity, &stage->player);
  }
  BLIT_LIST(stage->renderer, Entity, stage->enemy_tail);
  BLIT_LIST(stage->renderer, Entity, stage->enemy_tail);
  BLIT_LIST(stage->renderer, Entity, stage->bullet_tail);
  BLIT_LIST(stage->renderer, Entity, stage->enemy_bullet_tail);
  draw_debris(stage);
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
                  load_texture(renderer, "gfx/bullet_enemy.png"),
              [TEXTURE_BACKGROUND] =
                  load_texture(renderer, "gfx/background.png"),
              [TEXTURE_EXPLOSION] = load_texture(renderer, "gfx/explosion.png"),
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
