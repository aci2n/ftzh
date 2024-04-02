#include "app.h"
#include "draw.h"
#include "entity.h"
#include "input.h"
#include <SDL.h>
#include <SDL_render.h>
#include <stdbool.h>
#include <stdlib.h>

void handle_movement(InputState *input_state, Entity *player) {
  int dx = 4 * input_state->right - 4 * input_state->left;
  int dy = 4 * input_state->down - 4 * input_state->up;
  entity_move(player, dx, dy);
}

int main(int argc, char const *const argv[static argc - 1]) {
  App *app = app_new();
  SDL_Renderer *renderer = app_get_renderer(app);
  Entity *player =
      entity_new(100, 100, load_texture(renderer, "gfx/player.png"));
  InputState *input_state = app_get_input_state(app);

  while (true) {
    prepare_scene(renderer);
    do_input(input_state);
    handle_movement(input_state, player);
    blit(renderer, entity_get_texture(player), entity_get_x(player),
         entity_get_y(player));
    present_scene(renderer);
    SDL_Delay(16);
  }

  app_destroy(app);
  return EXIT_SUCCESS;
}
