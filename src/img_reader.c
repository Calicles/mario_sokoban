#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string.h>
#include "img_reader.h"
#include "img_const.h"

SDL_Surface * Img_reader_getImage(char *name)
{
    char path[MAX_CARAC] = {'\0'};
    strcat(path, IMG_PATH);
    strcat(path, "/");
    strcat(path, name);
    SDL_Surface *img = IMG_Load(path);
    return img;
}