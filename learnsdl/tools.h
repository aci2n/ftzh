#ifndef TOOLS_H
#define TOOLS_H 1

float randf();
int max_int(int, int);
int min_int(int, int);
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void calc_slope(int x1, int y1, int x2, int y2, float dx[restrict static 1],
                float dy[restrict static 1]);

#define FREE_LIST(TYPE, HEAD, NEXT)                                            \
  do {                                                                         \
    for (TYPE *curr = *(HEAD); curr;) {                                        \
      TYPE *next = curr->NEXT;                                                 \
      free(curr);                                                              \
      curr = next;                                                             \
    }                                                                          \
    *(HEAD) = 0;                                                               \
} while (false)

#define TYPE_NEW(TYPE, ...) TYPE ## _init(malloc(sizeof(TYPE*)), __VA_ARGS__)
#define ARR_LEN(ARR) (sizeof(ARR) / sizeof(ARR[0]))

#endif
