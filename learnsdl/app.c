#include "app.h"
#include "defs.h"
#include <SDL_image.h>

struct App {
  SDL_Renderer *renderer;
  SDL_Window *window;
};

App *app_new() { return app_init(malloc(sizeof(App))); }

App *app_init(App *app) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  int const window_flags = 0;
  SDL_Window *window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                        SCREEN_HEIGHT, window_flags);
  if (!window) {
    fprintf(stderr, "couldn't initialize window: %s\n", SDL_GetError());
    exit(1);
  }

  int const renderer_flags = SDL_RENDERER_ACCELERATED;
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, renderer_flags);
  if (!renderer) {
    fprintf(stderr, "couldn't create renderer: %s\n", SDL_GetError());
    exit(1);
  }

  IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

  (*app) = (App){
      .window = window,
      .renderer = renderer,
  };

  return app;
}

SDL_Renderer *app_get_renderer(App *app) { return app->renderer; }

SDL_Window *app_get_window(App *app) { return app->window; }

void app_destroy(App *app) {
  free(app->window);
  free(app->renderer);
  free(app);
}
