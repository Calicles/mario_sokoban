#ifndef EDITOR_H
#define EDITOR_H

#include <stdlib.h>
#include <stdio.h>
#include "game.h"

typedef struct str_game_editor
{
    Game game_state;
    int current_imageId;
    SDL_Surface *currentImg;
    SDL_Rect currentImgPos;
    int nbr_targets;
} Game_editor;

int Editor_init(Game_editor *game_editor);

int Editor_loop(Window *window);

int Editor_checkIfFreePlace(int x, int y, Map *map);

int Editor_placeItem(Game_editor *game_editor, int xtarget, int ytarget, const int item_const);

void Editor_mouseCoorToTileCoor(int mousex, int mousey, int *coorResult, int tileW, int tileH);

void Editor_drawGrid(Game_editor *game_eitor);

void Editor_drawCursor(Game_editor *game_editor);

int Editor_saveLvl(Game_editor *game_editor);

void Editor_findTileAt(Game_editor *game_editor, int xtarget, int ytarget, int *row, int *col, int *xtile, int *ytile);

void Editor_switchCursor(Game_editor *game_editor, int const item_const);
#endif
