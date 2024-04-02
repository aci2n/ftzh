#include "SDL_render.h"
#include "app.h"
#include "draw.h"
#include "entity.h"
#include "input.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>

void prepare_scene(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 96, 128, 255, 255);
  SDL_RenderClear(renderer);
}

void present_scene(SDL_Renderer *renderer) { SDL_RenderPresent(renderer); }

int main(int argc, char const *const argv[static argc - 1]) {
  App *app = app_new();
  SDL_Renderer *renderer = app_get_renderer(app);
  Entity *player =
      entity_new(100, 100, load_texture(renderer, "gfx/player.png"));

  while (true) {
    prepare_scene(renderer);
    do_input();
    present_scene(renderer);
    SDL_Delay(16);
  }

  app_destroy(app);
  return EXIT_SUCCESS;
}
