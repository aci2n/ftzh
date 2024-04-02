#include "app.h"
#include <stdlib.h>

int main(int argc, char const *const argv[static argc - 1]) {
  App *app = app_new();
  app_destroy(app);
  return EXIT_SUCCESS;
}
