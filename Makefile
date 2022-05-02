PROG = Battle-Of-Pleiades
CC = clang

include common.mk

CXXFLAGS += `sdl2-config --cflags`
CXXFLAGS += -g -lefence

LDFLAGS += `sdl2-config --libs` -lSDL2_mixer -lSDL2_ttf -lSDL2_image -lm

# linking the program.
$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

