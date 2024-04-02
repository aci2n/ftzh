#include "app.h"
#include "draw.h"
#include "input.h"
#include "stage.h"
#include <SDL.h>
#include <SDL_render.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char const *const argv[static argc - 1]) {
  App *app = app_new();
  SDL_Renderer *renderer = app_get_renderer(app);
  InputState *input_state = app_get_input_state(app);
  Stage *stage = stage_new(app);

  while (true) {
    prepare_scene(renderer);
    do_input(input_state);
    stage_invoke(stage);
    present_scene(renderer);
    SDL_Delay(16);
  }

  app_destroy(app);
  return EXIT_SUCCESS;
}
