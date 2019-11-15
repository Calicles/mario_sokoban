#include <SDL/SDL.h>
#include "game.h"
#include "rect.h"
#include "game_const.h"
#include "move_manager.h"


void Move_checkOnDown(Game *game, int dx, int dy)
{
    int wall_w = game->wall_img->w;
    int wall_h = game->wall_img->h;
    int map_w = game->map.width;
    int map_h = game->map.height;
    Casse *casse = NULL;
    int row = 0, col_min = 0, col_max = 0;
    int player_already_moved = 0;
    int tooMuchCasses = 0;
    int delta_y = 0;

    // Check if casse is on the path
    for (int i = 0; i < game->numberOfCasses; i++) {
        //the casse is upper
        if (game->player.y > game->casses[i].position.y) continue;

        delta_y = game->casses[i].position.y - (game->player.y + game->player.h);
        
        if ( delta_y <= MOVE_SPEED && Rect_is_in_bound(game->casses[i].position.x, game->casses[i].position.x + game->casses[i].position.w,
                                                        game->player.x, game->player.x + game->player.w))
        {
            if (casse != NULL)
            {
                tooMuchCasses++;
            }else
            {
                casse = &game->casses[i];
            }
        }
    }

    // A casse is on the path
    if (casse != NULL)
    {
        delta_y = casse->position.y - (game->player.y + game->player.h);
        // Here the vector must be adjusted, have to go on the casse
        if (delta_y > 1)
        {
            dy = MOVE_SPEED - delta_y;

        }else
        {
            if (tooMuchCasses || casse->placed)
            {
                dy = 0;
            }
            // Check if the casses is blocked by another one or a wall
            else
            {
                int pathClear = 1;

                // check another casse in path
                for (int i = 0; i < game->numberOfCasses; i++) {
                    // The same casse
                    if (casse == &game->casses[i] || casse->position.y > game->casses[i].position.y) continue;

                    if (((game->casses[i].position.y + game->casses[i].position.h) - casse->position.y <= 1) && Rect_is_in_bound(casse->position.x, casse->position.x + casse->position.w,
                                                                                            game->casses[i].position.x, game->casses[i].position.x + game->casses[i].position.w))
                    {
                        pathClear = 0;
                        dy = 0;
                    }
                }

                if (pathClear)
                {

                    // check for a wall
                    row = (casse->position.y + casse->position.h) / wall_h + 1;
                    col_min = casse->position.x / wall_w;
                    col_max = (casse->position.x + casse->position.w) / wall_h;

                    for (int i = col_min; i <= col_max; i++) {
                        if (Map_getElemAt(&game->map, row, i) == WALL)
                        {
                            int y = row * wall_h;
                            if ((y - (casse->position.y + casse->position.h + dy)) < MOVE_SPEED)
                            {
                                dy = y - (casse->position.y + casse->position.h) - 1;
                            }
                        }
                    }
                }
            }
        }
        if (!Move_is_casse_on_place(game, casse))
        {
            // Bring the casse
            Rect_translate(&casse->position, dx, dy);
        }
    }
    if (dy != 0)
    {
        SDL_Rect *player = &game->player;
        row = (player->y + player->h) / wall_h + 1;
        col_min = player->x / wall_w;
        col_max = (player->x + player->w) / wall_w; 

        // Check the wall on path
        for (int i = col_min; i <= col_max; i++) {
            if (Map_getElemAt(&game->map, row, i) == WALL)
            {
                int y = row * wall_h;
                if ((y - (player->y + player->h + dy)) < MOVE_SPEED)
                {
                    dy = y - (player->y + player->h) - 1;
                }
            }
        }
    }
    Rect_translate(&game->player, dx, dy);

}

void Move_checkOnUp(Game *game, int dx, int dy)
{
    int wall_w = game->wall_img->w;
    int wall_h = game->wall_img->h;
    int map_w = game->map.width;
    int map_h = game->map.height;
    Casse *casse = NULL;
    int row = 0, col_min = 0, col_max = 0;
    int player_already_moved = 0;
    int tooMuchCasses = 0;
    int delta_y = 0;

    // Check if casse is on the path
    for (int i = 0; i < game->numberOfCasses; i++) {
        // the casse is under
        if (game->casses[i].position.y > game->player.y) continue;

        delta_y = game->player.y - (game->casses[i].position.y + game->casses[i].position.h);
        
        if ( delta_y <= MOVE_SPEED && Rect_is_in_bound(game->casses[i].position.x, game->casses[i].position.x + game->casses[i].position.w,
                                                        game->player.x, game->player.x + game->player.w))
        {
            if (casse != NULL)
            {
                tooMuchCasses++;
            }else
            {
                casse = &game->casses[i];
            }
        }
    }

    // A casse is on the path
    if (casse != NULL)
    {
        delta_y = game->player.y - (casse->position.y + casse->position.h);
        // Here the vector must be adjusted, have to go on the casse
        if (delta_y > 1)
        {
            dy = delta_y - MOVE_SPEED;

        }else
        {
            if (tooMuchCasses || casse->placed)
            {
                dy = 0;
            }
            // Check if the casses is blocked by another one or a wall
            else
            {
                int pathClear = 1;

                // check if another casse in path
                for (int i = 0; i < game->numberOfCasses; i++) {
                    // The same casse
                    if (casse == &game->casses[i] || casse->position.y < game->casses[i].position.y) continue;

                    if ((casse->position.y - (game->casses[i].position.y + game->casses[i].position.h) <= 1) && Rect_is_in_bound(casse->position.x, casse->position.x + casse->position.w,
                                                                                            game->casses[i].position.x, game->casses[i].position.x + game->casses[i].position.w))
                    {
                        pathClear = 0;
                        dy = 0;
                    }
                }

                if (pathClear)
                {

                    // check for a wall
                    row = casse->position.y / wall_h - 1;
                    col_min = casse->position.x / wall_w;
                    col_max = (casse->position.x + casse->position.w) / wall_h;

                    for (int i = col_min; i <= col_max; i++) {
                        if (Map_getElemAt(&game->map, row, i) == WALL)
                        {
                            int y = row * wall_h + wall_h;
                            if ((casse->position.y + dy - y) < MOVE_SPEED)
                            {
                                dy = y - casse->position.y + 1;
                            }
                        }
                    }
                }
            }
        }
        if (!Move_is_casse_on_place(game, casse))
        {
            // Bring the casse
            Rect_translate(&casse->position, dx, dy);
        }
    }
    if (dy != 0)
    {
        SDL_Rect *player = &game->player;
        row = player->y / wall_h - 1;
        col_min = player->x / wall_w;
        col_max = (player->x + player->w) / wall_w; 

        // Check the wall on path
        for (int i = col_min; i <= col_max; i++) {
            if (Map_getElemAt(&game->map, row, i) == WALL)
            {
                int y = row * wall_h + wall_h;
                if ((player->y + dy - y) < MOVE_SPEED)
                {
                    dy = y - player->y + 1;
                }
            }
        }
    }
    Rect_translate(&game->player, dx, dy);
}

void Move_checkOnLeft(Game *game, int dx, int dy)
{
    int wall_w = game->wall_img->w;
    int wall_h = game->wall_img->h;
    int map_w = game->map.width;
    int map_h = game->map.height;
    Casse *casse = NULL;
    int row_min = 0, row_max, col = 0;
    int player_already_moved = 0;
    int tooMuchCasses = 0;
    int delta_x = 0;

    // Check if casse is on the path
    for (int i = 0; i < game->numberOfCasses; i++) {
        // the casse is under
        if (game->casses[i].position.x > game->player.x) continue;

        delta_x = game->player.x - (game->casses[i].position.x + game->casses[i].position.w);
        
        if ( delta_x <= MOVE_SPEED && Rect_is_in_bound(game->casses[i].position.y, game->casses[i].position.y + game->casses[i].position.h,
                                                        game->player.y, game->player.y + game->player.h))
        {
            if (casse != NULL)
            {
                tooMuchCasses++;
            }else
            {
                casse = &game->casses[i];
            }
        }
    }

    // A casse is on the path
    if (casse != NULL)
    {
        delta_x = game->player.x - (casse->position.x + casse->position.w);
        // Here the vector must be adjusted, have to go on the casse
        if (delta_x > 1)
        {
            dx = delta_x - MOVE_SPEED;

        }else
        {
            if (tooMuchCasses || casse->placed)
            {
                dx = 0;
            }
            // Check if the casses is blocked by another one or a wall
            else
            {
                int pathClear = 1;

                // check if another casse in path
                for (int i = 0; i < game->numberOfCasses; i++) {
                    // The same casse or on the other side
                    if (casse == &game->casses[i] || casse->position.x < game->casses[i].position.x) continue;

                    if ((casse->position.x - (game->casses[i].position.x + game->casses[i].position.w) <= 1) && Rect_is_in_bound(casse->position.y, casse->position.y + casse->position.h,
                                                                                            game->casses[i].position.y, game->casses[i].position.y + game->casses[i].position.h))
                    {
                        pathClear = 0;
                        dx = 0;
                    }
                }

                if (pathClear)
                {

                    // check for a wall
                    row_min = casse->position.y / wall_h;
                    row_max = (casse->position.y + casse->position.h) / wall_h;
                    col = casse->position.x / wall_w - 1;

                    for (int i = row_min; i <= row_max; i++) {
                        if (Map_getElemAt(&game->map, i, col) == WALL)
                        {
                            int x = col * wall_w + wall_w;
                            if ((casse->position.x + dx - x) < MOVE_SPEED)
                            {
                                dx = x - casse->position.x + 1;
                            }
                        }
                    }
                }
            }
        }
        if (!Move_is_casse_on_place(game, casse))
        {
            // Bring the casse
            Rect_translate(&casse->position, dx, dy);
        }
    }
    if (dx != 0)
    {
        SDL_Rect *player = &game->player;
        row_min = player->y / wall_h;
        row_max = (player->y + player->h) / wall_h;
        col = player->x / wall_w - 1;
        // Check the wall on path
        for (int i = row_min; i <= row_max; i++) {
            if (Map_getElemAt(&game->map, i, col) == WALL)
            {
                int x = col * wall_w + wall_w;
                if ((player->x + dx - x) < MOVE_SPEED)
                {
                    dx = x - player->x + 1;
                }
            }
        }
    }
    Rect_translate(&game->player, dx, dy);
}

void Move_checkOnRight(Game *game, int dx, int dy)
{
    int wall_w = game->wall_img->w;
    int wall_h = game->wall_img->h;
    int map_w = game->map.width;
    int map_h = game->map.height;
    Casse *casse = NULL;
    int row_min = 0, row_max, col = 0;
    int player_already_moved = 0;
    int tooMuchCasses = 0;
    int delta_x = 0;

    // Check if casse is on the path
    for (int i = 0; i < game->numberOfCasses; i++) {
        // the casse is under
        if (game->casses[i].position.x < game->player.x) continue;

        delta_x = game->casses[i].position.x - (game->player.x + game->player.w);
        
        if ( delta_x <= MOVE_SPEED && Rect_is_in_bound(game->casses[i].position.y, game->casses[i].position.y + game->casses[i].position.h,
                                                        game->player.y, game->player.y + game->player.h))
        {
            if (casse != NULL)
            {
                tooMuchCasses++;
            }else
            {
                casse = &game->casses[i];
            }
        }
    }

    // A casse is on the path
    if (casse != NULL)
    {
        delta_x = casse->position.x - (game->player.x + game->player.w);
        // Here the vector must be adjusted, have to go on the casse
        if (delta_x > 1)
        {
            dx = MOVE_SPEED - delta_x;

        }else
        {
            if (tooMuchCasses || casse->placed)
            {
                dx = 0;
            }
            // Check if the casses is blocked by another one or a wall
            else
            {
                int pathClear = 1;

                // check if another casse in path
                for (int i = 0; i < game->numberOfCasses; i++) {
                    // The same casse or on the other side
                    if (casse == &game->casses[i] || casse->position.x > game->casses[i].position.x) continue;

                    if ((game->casses[i].position.x - (casse->position.x + casse->position.w) <= 1) && Rect_is_in_bound(casse->position.y, casse->position.y + casse->position.h,
                                                                                            game->casses[i].position.y, game->casses[i].position.y + game->casses[i].position.h))
                    {
                        pathClear = 0;
                        dx = 0;
                    }
                }

                if (pathClear)
                {

                    // check for a wall
                    row_min = casse->position.y / wall_h;
                    row_max = (casse->position.y + casse->position.h) / wall_h;
                    col = (casse->position.x + casse->position.w) / wall_w + 1;

                    for (int i = row_min; i <= row_max; i++) {
                        if (Map_getElemAt(&game->map, i, col) == WALL)
                        {
                            int x = col * wall_w;
                            if ((x - (casse->position.x + casse->position.w + dx)) < MOVE_SPEED)
                            {
                                dx = x - (casse->position.x + casse->position.w + 1);
                            }
                        }
                    }
                }
            }
        }
        if (!Move_is_casse_on_place(game, casse))
        {
            // Bring the casse
            Rect_translate(&casse->position, dx, dy);
        }
    }
    if (dx != 0)
    {
        SDL_Rect *player = &game->player;
        row_min = player->y / wall_h;
        row_max = (player->y + player->h) / wall_h;
        col = (player->x + player->w) / wall_w + 1;
        // Check the wall on path
        for (int i = row_min; i <= row_max; i++) {
            if (Map_getElemAt(&game->map, i, col) == WALL)
            {
                int x = col * wall_w;
                if ((x - (player->x + player->w + dx)) < MOVE_SPEED)
                {
                    dx = x - (player->x + player->w + 1);
                }
            }
        }
    }
    Rect_translate(&game->player, dx, dy);
}

int Move_is_casse_on_place(Game *game, Casse *casse)
{
    if (casse->placed) return 1;

    int len = game->numberOfCasses;

    for (int i = 0; i < len; i++) {
        if (Rect_is_nearest_than(DISTANCE_TO_PLACE, casse->position, game->targets[i]))
        {
            casse->placed = 1;
            int dx = 0, dy = 0;
            dx = game->targets[i].x - casse->position.x;
            dy = game->targets[i].y - casse->position.y;
            Rect_translate(&casse->position, dx, dy);
            game->number_casses_placed++;
            
            return 1;
        }
    }
    return 0;
}
