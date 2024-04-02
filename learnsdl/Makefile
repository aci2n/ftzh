EXTMODULES := sdl2 SDL2_image
CFLAGS += -ggdb -Og $(shell pkgconf --cflags $(EXTMODULES))
LDFLAGS += $(shell pkgconf --libs $(EXTMODULES)) -lm
OBJS := $(patsubst %.c, %.o, $(wildcard *.c))

main: $(OBJS)
