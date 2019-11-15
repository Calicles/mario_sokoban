#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "window.h"
#include "img_const.h"
#include "game_const.h"
#include "img_reader.h"

int Window_init(Window *window, char *window_title, char *icon_name)
{
    SDL_Init(SDL_INIT_VIDEO);

    if (window_title != NULL)
        SDL_WM_SetCaption(window_title, NULL);

    if (icon_name != NULL)
        SDL_WM_SetIcon(Img_reader_getImage(icon_name), NULL);
    
    window->screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, /*SDL_FULLSCREEN |*/ SDL_SWSURFACE | SDL_DOUBLEBUF);
    
    SDL_EnableKeyRepeat(10, 10);
    
    return window->screen != NULL;
}

void Window_refresh(Window *window)
{
    SDL_Flip(window->screen);
}

void Window_draw(Window *window, SDL_Surface *img, SDL_Rect *position)
{
    SDL_BlitSurface(img, NULL, window->screen, position);
}

void Window_blackScreen(Window *window)
{
    SDL_FillRect(window->screen, NULL, SDL_MapRGB(window->screen->format, 0, 0, 0));
}

int Window_quit()
{
    SDL_Quit();
}