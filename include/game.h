#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "map.h"
#include "rect.h"

typedef struct Game Game;
struct Game 
{
    Window *window;
    Map map;
    SDL_Rect player;
    Casse *casses;
    SDL_Rect *targets;
    int numberOfCasses;
    int number_casses_placed;
    SDL_Surface *casse_img;
    SDL_Surface *casse_ok_img;
    SDL_Surface *player_img;
    SDL_Surface *player_up;
    SDL_Surface *player_down;
    SDL_Surface *player_right;
    SDL_Surface *player_left;
    SDL_Surface *wall_img;
    SDL_Surface *target_img;
};

int Game_loop(Window *window, char *levelFileName);

static int Game_init(Game *game, char *levelFileName);
int Game_initImages(Game *game);
static int Game_initTargets(Game *game);
static void Game_move(Game *game, int dx, int dy);
void Game_refresh(Game *game);
void Game_freeAll(Game *game);
static void Game_drawMap(const Game *game);
static void Game_drawCasses(Game *game);

#endif