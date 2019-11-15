#ifndef RECT_H
#define RECT_H
#include <SDL/SDL.h>
#include <stdlib.h>

typedef struct Casse Casse;
struct Casse
{
    SDL_Rect position;
    short placed;
};


void Rect_translate(SDL_Rect *rect, const int dx, const int dy);
void Rect_init(SDL_Rect *rect, const int x, const int y, const int w, const int h);
void Rect_init2(SDL_Rect *rect, const int w, const int h);
int Rect_is_in_bound(const int line1_a, const int line1_b, const int line2_a, const int line2_b);
void Rect_findMiddle_rect(const SDL_Rect rect, int *x, int *y);
int Rect_is_nearest_than(const int dist, const SDL_Rect rect1, SDL_Rect rect2);
int Rect_getDistSquare(const int p1x, const int p1y, const int p2x, const int p2y);

#endif