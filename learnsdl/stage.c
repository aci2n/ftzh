#include "stage.h"
#include "SDL_scancode.h"
#include "defs.h"
#include "draw.h"
#include "entity.h"
#include "tools.h"
#include <SDL_render.h>
#include <math.h>
#include <stdlib.h>

struct Stage {
  SDL_Renderer *renderer;
  InputState *input_state;
  Entity player;
  Entity *fighter_tail;
  Entity *bullet_tail;
  EntityTexture player_texture;
  EntityTexture bullet_texture;
  EntityTexture enemy_texture;
  int enemy_spawn_timer;
};

static void fire_bullet(Stage *stage, float offset_y) {
  Entity const *player = &stage->player;
  Entity *bullet = malloc(sizeof(Entity));

  (*bullet) = (Entity){
      .x = player->x + player->texture.w,
      .y = player->y + player->texture.h / 2.0 -
           offset_y * stage->bullet_texture.h,
      .dx = PLAYER_BULLET_SPEED,
      .dy = randf() * 2 - 1,
      .texture = stage->bullet_texture,
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
  player->x += player->dx;
  player->y += player->dy;
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
    if (fighter->health <= 0 || fighter->x <= -fighter->texture.w) {
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

  (*enemy) = (Entity){
      .x = SCREEN_WIDTH,
      .y = rand() % SCREEN_HEIGHT,
      .dx = -(ENEMY_DX_MIN + rand() % (ENEMY_DX_MAX - ENEMY_DX_MIN)),
      .dy = randf() * ENEMY_DY_MAX,
      .texture = stage->enemy_texture,
      .next = stage->fighter_tail,
      .side = SIDE_ENEMY,
      .health = ENEMY_STARTING_HEALTH,
  };

  stage->fighter_tail = enemy;
  stage->enemy_spawn_timer =
      ENEMY_BASE_SPAWN_RATE + rand() % ENEMY_RAND_SPAWN_RATE;
}

static void do_collisions(Stage *stage) {
 bullet:
  for (Entity *bullet = stage->bullet_tail; bullet; bullet = bullet->next) {
    for (Entity *enemy = stage->fighter_tail; enemy; enemy = enemy->next) {
      if (entity_collision(bullet, enemy)) {
        if (--bullet->health) {
					goto bullet;
				}
        enemy->health--;
      }
    }
  }
}

static void logic(Stage *stage) {
  do_collisions(stage);
  do_player(stage);
  do_fighters(stage);
  do_bullets(stage);
  spawn_enemies(stage);
}

static void blit_list(SDL_Renderer *renderer, Entity *head) {
  for (Entity *entity = head; entity; entity = entity->next) {
    blit(renderer, entity->texture.ref, entity->x, entity->y);
  }
}

static void draw(Stage *stage) {
  blit_list(stage->renderer, &stage->player);
  blit_list(stage->renderer, stage->fighter_tail);
  blit_list(stage->renderer, stage->bullet_tail);
}

static EntityTexture create_entity_texture(SDL_Renderer *renderer,
                                           char const path[static 1]) {
  EntityTexture texture = {.ref = load_texture(renderer, path)};
  SDL_QueryTexture(texture.ref, 0, 0, &texture.w, &texture.h);
  return texture;
}

Stage *stage_new(App *app) { return stage_init(malloc(sizeof(Stage)), app); }

Stage *stage_init(Stage *stage, App *app) {
  SDL_Renderer *renderer = app_get_renderer(app);

  (*stage) = (Stage){
      .renderer = renderer,
      .input_state = app_get_input_state(app),
      .player_texture = create_entity_texture(renderer, "gfx/player.png"),
      .bullet_texture = create_entity_texture(renderer, "gfx/bullet.png"),
      .enemy_texture = create_entity_texture(renderer, "gfx/enemy.png"),
  };
  stage->player = (Entity){
      .x = PLAYER_START_X,
      .y = PLAYER_START_Y,
      .texture = stage->player_texture,
      .side = SIDE_PLAYER,
  };

  return stage;
}

void stage_invoke(Stage *stage) {
  logic(stage);
  draw(stage);
}

void stage_destroy(Stage *stage) {
  for (Entity *fighter = stage->fighter_tail; fighter;) {
    Entity *next = fighter->next;
    free(fighter);
    fighter = next;
  }
  for (Entity *bullet = stage->bullet_tail; bullet;) {
    Entity *next = bullet->next;
    free(bullet);
    bullet = next;
  }
  SDL_DestroyTexture(stage->player_texture.ref);
  SDL_DestroyTexture(stage->bullet_texture.ref);
  SDL_DestroyTexture(stage->enemy_texture.ref);
  free(stage);
}
