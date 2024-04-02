#include "stage.h"
#include "defs.h"
#include "draw.h"
#include "entity.h"
#include "tools.h"
#include <SDL_render.h>
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
};

static void fire_bullet(Stage *stage, float offset_y) {
  Entity const *player = &stage->player;
  Entity *bullet = malloc(sizeof(Entity));

  (*bullet) = (Entity){
      .x = player->x + player->texture.w,
      .y = player->y + player->texture.h / 2.0 -
           offset_y * stage->bullet_texture.h,
      .dx = PLAYER_BULLET_SPEED,
      .dy = randf() * 4 - 2,
      .texture = stage->bullet_texture,
      .health = 1,
      .next = stage->bullet_tail,
  };

  /* printf("fire bullet: bullet=%p tail=%p\n", bullet, stage->bullet_tail); */
  stage->bullet_tail = bullet;
}

static void do_player(Stage *stage) {
  Entity *player = &stage->player;
  InputState const *input_state = stage->input_state;

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
    fire_bullet(stage, -1.5);
    fire_bullet(stage, 0.5);
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
    blit(renderer, entity->texture.ref, entity->x, entity->y);
  }
}

static void draw(Stage *stage) {
  blit_list(stage->renderer, &stage->player);
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
      .x = 100,
      .y = 100,
      .texture = stage->player_texture,
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
