#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_const.h"
#include "img_const.h"
#include "window.h"
#include "img_reader.h"
#include "rect.h"
#include "move_manager.h"

int Game_loop(Window *window, char *levelFileName) 
{
    Game game;
    game.window = window;
    if (Game_init(&game, levelFileName) == EXIT_FAILURE) 
    {
        Game_freeAll(&game);
        return EXIT_FAILURE;
    }
    int win = 0;

    SDL_Event event;

    while (!win)
    {
        SDL_ShowCursor(SDL_DISABLE);

        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                win = 1;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_DOWN:
                    Game_move(&game, 0, MOVE_SPEED);
                    break;

                case SDLK_UP:
                    Game_move(&game, 0, -MOVE_SPEED);
                    break;

                case SDLK_RIGHT:
                    Game_move(&game, MOVE_SPEED, 0);
                    break;

                case SDLK_LEFT:
                    Game_move(&game, -MOVE_SPEED, 0);
                    break;

                case SDLK_ESCAPE:
                    win = 1;
                    break;
                
                default:
                    break;
                }           

        }
        Game_refresh(&game);
        if (game.number_casses_placed == game.numberOfCasses)
        {
            win = 1;
        }
    }
    SDL_Delay(3000);
    Game_freeAll(&game);
    return EXIT_SUCCESS;
}

int Game_init(Game *game, char *levelFileName)
{
    FILE *lvlFile = NULL;
    lvlFile = fopen(levelFileName, "r");


    if (lvlFile != NULL)
    {   
        char position[20] = {'\0'};
        char *pSeparator = NULL;
        int numberOfPosition;
        game->numberOfCasses = 0;
        game->number_casses_placed = 0;
        
        if (!Game_initImages(game)) 
        {
            Game_freeAll(game);
            return EXIT_FAILURE;
        }

        do {
            fgets(position, 20, lvlFile);
            game->numberOfCasses++;
        }while (strchr(position, ' ') != NULL);

        game->numberOfCasses -= 2;

        rewind(lvlFile);

        game->casses = (Casse*) malloc(sizeof(Casse) * game->numberOfCasses);
        game->targets = (SDL_Rect*) malloc(sizeof(SDL_Rect) * game->numberOfCasses);

        numberOfPosition = game->numberOfCasses + 1;

        for (int i = 0; i < numberOfPosition; i++) {
            int x = 0, y = 0;
            fgets(position, 20, lvlFile);
            pSeparator = strchr(position, ' ');
            *pSeparator = '\0';

            x = atol(position);
            y = atol(pSeparator + 1);
            
            if (i == 0)
            {
                game->player.x = x;
                game->player.y = y;
                game->player.w = game->player_img->w;
                game->player.h = game->player_img->h;
            }else
            {
                game->casses[i - 1].position.x = x;
                game->casses[i - 1].position.y = y;
                game->casses[i - 1].position.w = game->casse_img->w;
                game->casses[i - 1].position.h = game->casse_img->h;
                game->casses[i - 1].placed = 0;
            }
        }

        Map_init(&game->map, lvlFile, game->numberOfCasses + LEVEL_HEADER);
        Game_initTargets(game);


        fclose(lvlFile);

        return EXIT_SUCCESS;

    }else
        return EXIT_FAILURE;
    
}

static int Game_initTargets(Game *game)
{
    int w = game->map.width;
    int h = game->map.height;
    int count = 0;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int elem = Map_getElemAt(&game->map, i, j);

            if (elem == TARGET_PLACE)
            {
                game->targets[count].x = game->wall_img->w * j;
                game->targets[count].y = game->wall_img->h * i;
                game->targets[count].w = game->wall_img->w;
                game->targets[count].h = game->wall_img->h;

                count++;
            }
        }
    }
}

int Game_initImages(Game *game)
{
    int success = 1;
    game->player_down = Img_reader_getImage(MARIO_B);
    game->player_img = game->player_down;
    game->player_up = Img_reader_getImage(MARIO_U);
    game->player_right = Img_reader_getImage(MARIO_L);
    game->player_left = Img_reader_getImage(MARIO_R);
    game->wall_img = Img_reader_getImage(MUR);
    game->casse_img = Img_reader_getImage(CAISSE);
    game->casse_ok_img = Img_reader_getImage(CAISSE_OK);
    game->target_img = Img_reader_getImage(TARGET);

    success = ((game->player_img != NULL) && (game->player_up != NULL)
                && (game->player_right != NULL)
                && (game->player_left != NULL)
                && (game->wall_img != NULL)
                && (game->target_img != NULL)
                && (game->target_img != NULL)
                && (game->casse_img != NULL));

    return success;
}

static void Game_move(Game *game, int dx, int dy)
{
    Casse *pcasse = NULL;
    if (dy < 0){
        game->player_img = game->player_up;
        Move_checkOnUp(game, dx, dy);
    } else if (dy > 0) {
        game->player_img = game->player_down;
        Move_checkOnDown(game, dx, dy);
    } else if (dx < 0) {
        game->player_img = game->player_right;
        Move_checkOnLeft(game, dx, dy);
    } else if (dx > 0) {
        game->player_img = game->player_left;
        Move_checkOnRight(game, dx, dy);
    }
}

void Game_refresh(Game *game)
{
    Window_blackScreen(game->window);
    Game_drawMap(game);
    Window_draw(game->window, game->player_img, &game->player);
    Game_drawCasses(game);
    Window_refresh(game->window);
}

static void Game_drawMap(const Game *game)
{
    int w = game->map.width, h = game->map.height;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int elem = Map_getElemAt(&game->map, i, j);
            
            SDL_Rect r;
            r.x = game->wall_img->w * j;
            r.y = game->wall_img->h * i;
            r.w = game->wall_img->w;
            r.h = game->wall_img->h;

            if (elem == WALL)
            {
                Window_draw(game->window, game->wall_img, &r);
            }else if (elem == TARGET_PLACE)
            {
                r.x += (r.w / 2) - (game->target_img->w / 2);
                r.y += (r.h / 2) - (game->target_img->h / 2);
                r.w = game->target_img->w;
                r.h = game->target_img->h;
                Window_draw(game->window, game->target_img, &r);
            }else
            {
                SDL_FillRect(game->window->screen, &r, SDL_MapRGB(game->window->screen->format, 0, 0, 0));
            }
        }
    }
}

static void Game_drawCasses(Game *game)
{
    for (int i = 0; i < game->numberOfCasses; i++) {
        if (game->casses[i].placed)
        {
            Window_draw(game->window, game->casse_ok_img, &game->casses[i].position);
        }else
        {
            Window_draw(game->window, game->casse_img, &game->casses[i].position);
            
        }
    }
}

void Game_freeAll(Game *game)
{
    Map_free(&game->map);
    if (game->player_left != NULL)
        SDL_FreeSurface(game->player_left);
    
    if (game->player_up != NULL)
        SDL_FreeSurface(game->player_up);
    if (game->player_down != NULL)
        SDL_FreeSurface(game->player_down);
    if (game->player_right != NULL)
        SDL_FreeSurface(game->player_right);
    if (game->wall_img != NULL)
        SDL_FreeSurface(game->wall_img);
    if (game->casse_img != NULL)
        SDL_FreeSurface(game->casse_img);
    if (game->casse_ok_img != NULL)
        SDL_FreeSurface(game->casse_ok_img);
    if (game->target_img != NULL)
        SDL_FreeSurface(game->target_img);

    if (game->casses != NULL)
        free(game->casses);
    if (game->targets != NULL)
        free(game->targets);
}
