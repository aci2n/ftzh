#ifndef STAGE_H
#define STAGE_H 1

#include "app.h"

typedef struct Stage Stage;

Stage *stage_new(App *);
Stage *stage_init(Stage *, App *);
void stage_invoke(Stage *);
void stage_destroy(Stage *);

#endif
