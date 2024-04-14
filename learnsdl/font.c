#include "font.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "defs.h"
#include <SDL_ttf.h>

void font_init() {
  if (TTF_Init() < 0) {
    fprintf(stderr, "could not initialize TTF: %s\n", SDL_GetError());
    exit(1);
  }
}

void font_cleanup() { TTF_Quit(); }

struct FontStore {
  SDL_Renderer *renderer;
  size_t nfonts;
  TTF_Font **fonts;
};

FontStore *font_store_init(FontStore *store, SDL_Renderer *renderer, size_t n,
                           char const *const paths[static n]) {
  TTF_Font **fonts = malloc(sizeof(TTF_Font *) * n);

  for (size_t i = 0; i < n; i++) {
    fonts[i] = TTF_OpenFont(paths[i], FONT_SIZE);
  }

  *store = (FontStore){
      .renderer = renderer,
      .nfonts = n,
      .fonts = fonts,
  };

  return store;
}

void font_store_destroy(FontStore *store) { free(store); }

SDL_Texture *font_store_texture(FontStore *store, size_t font,
                                char const text[static 1], SDL_Color color) {
  SDL_Surface *surface =
      TTF_RenderUTF8_Blended(store->fonts[font], text, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(store->renderer, surface);

  SDL_FreeSurface(surface);

  return texture;
}
