#include "input.h"
#include "app.h"
#include "defs.h"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_scancode.h>

void do_key_change(InputState *input_state, SDL_KeyboardEvent *event,
                   bool key_state) {
  if (event->repeat || event->keysym.scancode >= MAX_KEYBOARD_KEYS) {
    return;
  }
	input_state[event->keysym.scancode] = key_state;
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
