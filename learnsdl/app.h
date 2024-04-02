#ifndef APP_H
#define APP_H

#include <SDL.h>
#include <stdbool.h>

typedef struct App App;
typedef struct InputState InputState;

struct InputState {
  bool up;
	bool down;
	bool left;
	bool right;
};

App* app_new();
App* app_init(App* app);
void app_destroy(App* app);
SDL_Renderer* app_get_renderer(App* app);
SDL_Window* app_get_window(App* app);
InputState* app_get_input_state(App* app);

#endif
