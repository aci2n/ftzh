#ifndef FONT_H
#define FONT_H 1

#include <stdlib.h>

void font_init();
void font_cleanup();

typedef struct FontStore FontStore;

FontStore *font_store_init(FontStore *store, size_t n,
                           char const *const paths[static n]);
FontStore *font_store_destroy(FontStore *store);

#endif
