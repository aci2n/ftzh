#include "tools.h"
#include "SDL_log.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

float randf() { return (float)rand() / (float)RAND_MAX; }

int max_int(int a, int b) { return a > b ? a : b; }

int min_int(int a, int b) { return a < b ? a : b; }

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
  return (max_int(x1, x2) < min_int(x1 + w1, x2 + w2)) &&
         (max_int(y1, y2) < min_int(y1 + h1, y2 + h2));
}

float vector_magnitude(float x, float y) { return sqrt(pow(x, 2) + pow(y, 2)); }

void calc_slope(int x1, int y1, int x2, int y2, float dx[restrict static 1],
                float dy[restrict static 1]) {
  float x = x1 - x2;
  float y = y1 - y2;
  float v = vector_magnitude(x, y);
  *dx = x / v;
  *dy = y / v;
  /* SDL_LogInfo(0, "vector magnitude %f", vector_magnitude(*dx, *dy)); */
  /* assert(vector_magnitude(*dx, *dy) == 1); */
}
