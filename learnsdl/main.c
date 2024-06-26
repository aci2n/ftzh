#include "app.h"
#include "defs.h"
#include "draw.h"
#include "input.h"
#include "stage.h"
#include <SDL.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

static void cap_framerate(long then[static 1], float remainder[static 1]) {
  static long const ms_per_frame = 1000 / FPS;

  long const frame_time = SDL_GetTicks64() - *then;
  long const wait = ms_per_frame - frame_time;
  if (wait > 0) {
    SDL_Delay(wait);
  }
  /* *remainder = *remainder - (int)*remainder + 0.667; */
  *then = SDL_GetTicks64();
}

int main(int argc, char const *const argv[static argc - 1]) {
  srand(time(0));
  App *app = app_new();
  SDL_Renderer *renderer = app_get_renderer(app);
  InputState *input_state = app_get_input_state(app);
  Stage *stage = stage_new(app);
  long then = SDL_GetTicks64();
  float remainder = 0;

  while (true) {
    prepare_scene(renderer);
    do_input(input_state);
    stage_invoke(stage);
    present_scene(renderer);
    cap_framerate(&then, &remainder);
  }

  printf("cleanup\n");
  stage_destroy(stage);
  app_destroy(app);

  return EXIT_SUCCESS;
}
