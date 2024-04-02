#include "input.h"
#include "SDL_events.h"
#include "SDL_scancode.h"
#include "app.h"
#include <SDL.h>

void do_key_change(InputState *input_state, SDL_KeyboardEvent *event,
                   bool key_state) {
  if (event->repeat) {
    return;
  }

  switch (event->keysym.scancode) {
  case SDL_SCANCODE_UP:
    input_state->up = key_state;
    break;
  case SDL_SCANCODE_DOWN:
    input_state->down = key_state;
    break;
  case SDL_SCANCODE_LEFT:
    input_state->left = key_state;
    break;
  case SDL_SCANCODE_RIGHT:
    input_state->right = key_state;
    break;
  case SDL_SCANCODE_LCTRL:
    input_state->fire = key_state;
    break;
  default:
    break;
  }
}

void do_key_down(InputState *input_state, SDL_KeyboardEvent *event) {
  do_key_change(input_state, event, true);
}

void do_key_up(InputState *input_state, SDL_KeyboardEvent *event) {
  do_key_change(input_state, event, false);
}

void do_input(InputState *input_state) {
  SDL_Event event = {0};

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      exit(0);
      break;

    case SDL_KEYDOWN:
      do_key_down(input_state, &event.key);
      break;

    case SDL_KEYUP:
      do_key_up(input_state, &event.key);
      break;

    default:
      break;
    }
  }
}
