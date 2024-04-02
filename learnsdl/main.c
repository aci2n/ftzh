#include "app.h"
#include "defs.h"
#include "draw.h"
#include "entity.h"
#include "input.h"
#include <SDL.h>
#include <SDL_render.h>
#include <stdbool.h>
#include <stdlib.h>

void handle_player_movement(InputState *input_state, Entity *player) {
  int dx = 4 * input_state->right - 4 * input_state->left;
  int dy = 4 * input_state->down - 4 * input_state->up;
  entity_move(player, dx, dy);
}

void handle_bullet_fire(InputState *input_state, Entity *player,
                        Entity *bullet) {
  if (input_state->fire && entity_get_health(bullet) <= 0) {
    entity_init(bullet, entity_get_x(player), entity_get_y(player), 16, 0, 1,
                entity_get_texture(bullet));
  }
  int dx = entity_get_dx(bullet);
  int dy = entity_get_dy(bullet);
  entity_move(bullet, dx, dy);
  if (entity_get_x(bullet) > SCREEN_WIDTH) {
    entity_set_health(bullet, 0);
  }
}

int main(int argc, char const *const argv[static argc - 1]) {
  App *app = app_new();
  SDL_Renderer *renderer = app_get_renderer(app);
  Entity *player =
      entity_new(100, 100, load_texture(renderer, "gfx/player.png"));
  InputState *input_state = app_get_input_state(app);
  Entity *bullet = entity_new(0, 0, load_texture(renderer, "gfx/bullet.png"));

  while (true) {
    prepare_scene(renderer);
    do_input(input_state);
    handle_player_movement(input_state, player);
    handle_bullet_fire(input_state, player, bullet);
    blit(renderer, entity_get_texture(player), entity_get_x(player),
         entity_get_y(player));
    if (entity_get_health(bullet) > 0) {
      blit(renderer, entity_get_texture(bullet), entity_get_x(bullet),
           entity_get_y(bullet));
    }
    present_scene(renderer);
    SDL_Delay(16);
  }

  app_destroy(app);
  return EXIT_SUCCESS;
}
