DEPS := sdl2 SDL2_image SDL2_mixer SDL2_ttf
CFLAGS += -ggdb -Og $(shell pkgconf --cflags $(DEPS))
LDLIBS += $(shell pkgconf --libs $(DEPS)) -lm
OBJS := $(patsubst %.c, %.o, $(wildcard *.c))

main: $(OBJS) *.h
	$(CC) $(LDLIBS) $(OBJS) -o main

run: main
	./main
