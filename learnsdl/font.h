#ifndef FONT_H
#define FONT_H 1

#include <SDL.h>

void font_init();
void font_cleanup();

typedef struct FontStore FontStore;

FontStore *font_store_init(FontStore *store, SDL_Renderer *renderer, size_t n,
                           char const *const paths[static n]);
void font_store_destroy(FontStore *store);
SDL_Texture *font_store_texture(FontStore *store, size_t font,
                                char const text[static 1], SDL_Color color);

#endif
