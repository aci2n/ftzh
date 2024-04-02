#include "stage.h"
#include "defs.h"
#include "delegate.h"
#include "draw.h"
#include "entity.h"

struct Stage {
  App *app;
  Entity *fighter_tail;
  Entity *bullet_tail;
  Delegate *delegate;
};

static void fire_bullet(Stage *stage) {
  Entity *player = stage->fighter_tail;
  Entity *bullet =
      entity_new(entity_get_x(player), entity_get_y(player),
                 load_texture(app_get_renderer(stage->app), "gfx/bullet.png"));

  entity_set_dx(bullet, PLAYER_BULLET_SPEED);
  entity_set_health(bullet, 1);
  entity_set_y(bullet, entity_get_y(bullet) + entity_get_h(player) / 2.0 -
                           entity_get_h(bullet) / 2.0);
  entity_set_reload(player, 8);

  printf("fire bullet: bullet=%p tail=%p\n", bullet, stage->bullet_tail);
  entity_set_next(bullet, stage->bullet_tail);
  stage->bullet_tail = bullet;
}

static void do_player(Stage *stage) {
  Entity *player = stage->fighter_tail;
  InputState *input_state = app_get_input_state(stage->app);

  entity_set_dx(player, 0);
  entity_set_dy(player, 0);

  if (entity_get_reload(player) > 0) {
    entity_set_reload(player, entity_get_reload(player) - 1);
  }

  if (input_state[SDL_SCANCODE_UP]) {
    entity_set_dy(player, -PLAYER_SPEED);
  }

  if (input_state[SDL_SCANCODE_DOWN]) {
    entity_set_dy(player, PLAYER_SPEED);
  }

  if (input_state[SDL_SCANCODE_RIGHT]) {
    entity_set_dx(player, PLAYER_SPEED);
  }

  if (input_state[SDL_SCANCODE_LEFT]) {
    entity_set_dx(player, -PLAYER_SPEED);
  }

  if (input_state[SDL_SCANCODE_LCTRL] && entity_get_reload(player) == 0) {
    fire_bullet(stage);
  }

  entity_move(player, entity_get_dx(player), entity_get_dy(player));
}

static void do_bullets(Stage *stage) {
  Entity *prev = {0};
  Entity *next = {0};

  for (Entity *bullet = stage->bullet_tail; bullet; bullet = next) {
    next = entity_get_next(bullet);
    entity_move(bullet, entity_get_dx(bullet), entity_get_dy(bullet));

    if (entity_get_x(bullet) >= SCREEN_WIDTH) {
      if (bullet == stage->bullet_tail) {
        stage->bullet_tail = next;
      } else {
        entity_set_next(prev, next);
      }
      entity_destroy(bullet);
    } else {
			prev = bullet;
		}
  }
}

static void logic(DelegateCtx *ctx) {
  Stage *stage = (Stage *)ctx;
  do_player(stage);
  do_bullets(stage);
}

static void blit_list(SDL_Renderer *renderer, Entity *head) {
  for (Entity *entity = head; entity; entity = entity_get_next(entity)) {
    blit(renderer, entity_get_texture(entity), entity_get_x(entity),
         entity_get_y(entity));
  }
}

static void draw(DelegateCtx *ctx) {
  Stage *stage = (Stage *)ctx;
  SDL_Renderer *renderer = app_get_renderer(stage->app);
  blit_list(renderer, stage->fighter_tail);
  blit_list(renderer, stage->bullet_tail);
}

Stage *stage_new(App *app) { return stage_init(malloc(sizeof(Stage)), app); }

Stage *stage_init(Stage *stage, App *app) {
  Entity *player = entity_new(
      100, 100, load_texture(app_get_renderer(app), "gfx/player.png"));
  Delegate *delegate = delegate_new(logic, draw, stage);

  (*stage) = (Stage){
      .app = app,
      .fighter_tail = player,
      .delegate = delegate,
  };
  return stage;
}

void stage_invoke(Stage *stage) { delegate_invoke(stage->delegate); }
