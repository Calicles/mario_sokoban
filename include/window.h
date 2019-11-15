#ifndef WINDOW_H
#define WINDOW_H
#include <SDL/SDL.h>

typedef struct Window Window;
struct Window
{
    SDL_Surface *screen;
    
};

int Window_init(Window *window, char *window_title, char *icon_name);
void Window_refresh(Window *window);
int Window_quit();
void Window_draw(Window *window, SDL_Surface *img, SDL_Rect *position);
void Window_blackScreen(Window *window);

#endif