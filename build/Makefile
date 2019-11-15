CC = gcc
SOURCE_DIR = ../src
INCLUDE_DIR = ../include
SDL_OPT = `sdl-config --cflags --libs`
SDL_IMG_OPT = -lSDL_image
SDL_DRAW_OPT = `sdl-config --libs` /usr/local/lib/libSDL_draw.a
INCLUDES = $(wildcard $(INCLUDE_DIR)/*.h)
SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
EXEC = sokoban

build:  $(SOURCES) $(HEADERS)
	$(CC) -g -O0 -o $(EXEC) $(SOURCES) -I$(INCLUDE_DIR) $(SDL_OPT) $(SDL_IMG_OPT) $(SDL_DRAW_OPT)

clean:
	rm -vf $(EXEC)

run: build
	./$(EXEC)

debug: build
	valgrind -v --tool=memcheck --leak-check=full --track-origins=yes --leak-resolution=high --show-reachable=yes ./$(EXEC)

cache: build
	valgrind --tool=callgrind ./$(EXEC)
