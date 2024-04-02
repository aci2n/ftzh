#include "input.h"
#include <SDL.h>

void do_input() {
  SDL_Event event = {0};

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      exit(0);
      break;
    default:
      break;
    }
  }
}
