#ifndef IMG_READER_H
#define IMG_READER_H
#include <SDL/SDL_image.h>
#define MAX_CARAC 50

SDL_Surface * Img_reader_getImage(char *name);

#endif