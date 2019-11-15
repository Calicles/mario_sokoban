#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_draw.h>
#include "game_const.h"
#include "editor.h"
#include "game.h"
#include "window.h"
#include "map.h"

int Editor_loop(Window *window)
{
    int finished_editing = 0;
    SDL_Event event;

    Game_editor game_editor;
    game_editor.game_state.window = window;

    if (!Editor_init(&game_editor) == EXIT_FAILURE)
    {
        Game_freeAll(&game_editor.game_state);
        return EXIT_FAILURE;
    }

    while (!finished_editing)
    {
        SDL_WaitEvent(&event);
        
        switch (event.type)
        {
        case SDL_QUIT:
            finished_editing = 1;
            break;

        case SDL_MOUSEMOTION:
            game_editor.currentImgPos.x = event.button.x;
            game_editor.currentImgPos.y = event.button.y;
            break;
        
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                //PLACE ITEM IF FREE TILE
                Editor_placeItem(&game_editor,
                                    event.button.x,
                                    event.button.y, 
                                    game_editor.current_imageId);
                break;
            
            case SDL_BUTTON_RIGHT:
                //ERAZE Item
                Editor_placeItem(&game_editor,
                                    event.button.x,
                                    event.button.y,
                                    FREE);
                break;
            }
            break;

        case SDL_KEYDOWN:

            switch (event.key.keysym.sym)
            {
            case SDLK_KP1:
                Editor_switchCursor(&game_editor, WALL);
                break;

            case SDLK_KP2:
                Editor_switchCursor(&game_editor, CASSE);
                break;

            case SDLK_KP3:
                Editor_switchCursor(&game_editor, TARGET_PLACE);
                break;

            case SDLK_KP4:
                Editor_switchCursor(&game_editor, PLAYER);
                break;

            case SDLK_s:
                Editor_saveLvl(&game_editor);
                break;

            case SDLK_ESCAPE:
                finished_editing = 1;
                break;
                
            default:
                break;
            }           
        
        default:
            break;
        }

        Game_refresh(&game_editor.game_state);
        Editor_drawGrid(&game_editor); // TODO
        Editor_drawCursor(&game_editor);
        Window_refresh(game_editor.game_state.window);
    }

    SDL_Delay(500);
    Game_freeAll(&game_editor.game_state);
    return EXIT_SUCCESS;
}


int Editor_init(Game_editor *game_editor)
{
    FILE *lvlFile = NULL;
    game_editor->current_imageId = WALL;
    game_editor->currentImg = game_editor->game_state.wall_img;
    game_editor->game_state.numberOfCasses = 0;
    game_editor->game_state.number_casses_placed = 10;
    game_editor->game_state.casses = (Casse*) malloc(sizeof(Casse) * game_editor->game_state.number_casses_placed);
    game_editor->game_state.targets = NULL;

    Map_initVirgin(&game_editor->game_state.map);

    if (!Game_initImages(&game_editor->game_state)) 
    {
        Game_freeAll(&game_editor->game_state);
        return EXIT_FAILURE;
    }
}

int Editor_placeItem(Game_editor *game_editor, int xtarget,
 int ytarget, const int item_const)
{
        //IF OK TODO
        int row = 0, col = 0, xtile = 0, ytile = 0;
        Editor_findTileAt(game_editor, xtarget,
                         ytarget, &row, &col, &xtile, &ytile);
        
        if (Map_getElemAt(&game_editor->game_state.map, row, col) != WALL)
        {
            if (item_const == PLAYER)
            {
                game_editor->game_state.player.x = xtarget;
                game_editor->game_state.player.y = ytarget;

            }else
            {
                if (item_const == CASSE)
                {
                   Casse *casse = &game_editor->game_state.casses[game_editor->game_state.numberOfCasses];
                   casse->position.x = xtile;
                   casse->position.y = ytile;
                   game_editor->game_state.numberOfCasses++;
                }else if (item_const == TARGET_PLACE)
                {
                    game_editor->nbr_targets++;
                }
                Map_setTileAt(&game_editor->game_state.map, row, col, item_const);
            }
        }
}

void Editor_findTileAt(Game_editor *game_editor, int xtarget, int ytarget,
 int *row, int *col, int *xtile, int *ytile)
{
    Map *map = &game_editor->game_state.map;
    int tilew = game_editor->game_state.wall_img->w;
    int tileh = game_editor->game_state.wall_img->h;
    *row = ytarget / tileh;
    *col = xtarget / tilew;
}

void Editor_drawGrid(Game_editor *game_editor)
{
    int w = game_editor->game_state.map.width;
    int h = game_editor->game_state.map.height;
    int tilew = game_editor->game_state.wall_img->w;
    int tileh = game_editor->game_state.wall_img->h;
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    Uint32 yellow = 200;

// VERTI LINES
    for (int i = 0; i < h; i++)
    {
        x1 = 0;
        x2 = tilew * w;
        y1 = i * tileh;
        y2 = y1;
        Draw_Line(game_editor->game_state.window->screen,
                x1, y1, x2, y2,
                yellow);
    }
    for (int j = 0; j < w; j++)
    {
        x1 = tilew * j;
        x2 = x1;
        y1 = 0;
        y2 = tileh * h;
        Draw_Line(game_editor->game_state.window->screen,
                x1, y1, x2, y2,
                yellow);
    }
}

void Editor_drawCursor(Game_editor *game_editor)
{
    Window_draw(game_editor->game_state.window,
    game_editor->currentImg,
    &game_editor->currentImgPos);
}

int Editor_saveLvl(Game_editor *game_editor)
{

}

void Editor_switchCursor(Game_editor *game_editor, int const item_const)
{
    if (item_const == CASSE)
        game_editor->currentImg = game_editor->game_state.casse_img;
    else if (item_const == WALL)
        game_editor->currentImg = game_editor->game_state.wall_img;
    else if (item_const == PLAYER)
        game_editor->currentImg = game_editor->game_state.player_down;
    else if (item_const == TARGET_PLACE)
        game_editor->currentImg = game_editor->game_state.target_img;

    game_editor->current_imageId = item_const;
    game_editor->currentImgPos.w = game_editor->currentImg->w;
    game_editor->currentImgPos.h = game_editor->currentImg->h;
}
