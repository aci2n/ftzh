#ifndef TOOLS_H
#define TOOLS_H 1

#include <stdlib.h>

float randf() { return (float)rand() / (float)RAND_MAX; }

int max_int(int a, int b) { return a > b ? a : b; }

int min_int(int a, int b) { return a < b ? a : b; }

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
  return (max_int(x1, x2) < min_int(x1 + w1, x2 + w2)) &&
         (max_int(y1, y2) < min_int(y1 + h1, y2 + h2));
}

#endif
