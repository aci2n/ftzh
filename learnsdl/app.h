#ifndef APP_H
#define APP_H

#include <SDL.h>

typedef struct App App;

App* app_new();
App* app_init(App* app);
void app_destroy(App* app);
SDL_Renderer* app_get_renderer(App* app);
SDL_Window* app_get_window(App* app);

#endif
