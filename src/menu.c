#include <stdlib.h>
#include <SDL/SDL.h>
#include "menu.h"
#include "game.h"
#include "editor.h"
#include "game_const.h"
#include "img_const.h"
#include "img_reader.h"
#include "window.h"


void Menu_loop(void)
{
    int user_choice = 0;
    SDL_Event event;
    Window window;
    SDL_Surface *menu_img = Img_reader_getImage(MENU);
    SDL_Rect menu_pos;
    menu_pos.x = 0;
    menu_pos.y = 0;
    menu_pos.w = SCREEN_W;
    menu_pos.h = SCREEN_H;

    if (!Window_init(&window, GAME_NAME, GAME_ICON_NAME))
    {
        return;
    }

    do
    {
        Window_blackScreen(&window);
        Window_draw(&window, menu_img, &menu_pos);
        Window_refresh(&window);
        SDL_WaitEvent(&event);

        switch (event.type)
        {

        case SDL_QUIT:
            user_choice = 3;
            break;

        case SDL_KEYDOWN:
            
            switch (event.key.keysym.sym)
            {
            case SDLK_KP1:
                Game_loop(&window, LVL_PATH);
                break;
            
            case SDLK_KP2:
                Editor_loop(&window);
                break;

            case SDLK_ESCAPE:
            case SDLK_KP3:
                user_choice = 3;
                break;

            default:
                break;
            }
            break;
        
        default:
            break;
        }
    } while (user_choice != USER_QUIT);
}