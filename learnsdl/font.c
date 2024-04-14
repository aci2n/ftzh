#include "font.h"
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
  size_t nfonts;
  TTF_Font **fonts;
};

FontStore *font_store_init(FontStore *store, size_t n,
                           char const *const paths[static n]) {
  TTF_Font **fonts = malloc(sizeof(TTF_Font *) * n);

  for (size_t i = 0; i < n; i++) {
    fonts[i] = TTF_OpenFont(paths[i], FONT_SIZE);
  }

  *store = (FontStore){
      .nfonts = n,
      .fonts = fonts,
  };

  return store;
}

FontStore *font_store_destroy(FontStore *store) { free(store); }
