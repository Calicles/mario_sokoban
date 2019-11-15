#include <SDL/SDL.h>
#include <math.h>
#include "rect.h"

void Rect_translate(SDL_Rect *r, const int dx, const int dy)
{
    r->x += dx;
    r->y += dy;
}

void Rect_init(SDL_Rect *r, const int x, const int y, const int w, const int h)
{
    r->x = x;
    r->y = y;
    r->w = w;
    r->h = h;
}

void Rect_init2(SDL_Rect *r, const int w, const int h)
{
    Rect_init(r, 0, 0, w, h);
}

int Rect_is_in_bound(const int line1_a, int line1_b, int line2_a, int line2_b)
{
    // If line2 includes in line1's width
    if ((line1_a <= line2_a) && (line1_b >= line2_b)) return 1;
    // If line1 includes in line2's width
    if ((line2_a <= line1_a) && (line2_b >= line1_b)) return 1;
    // If line2 start is in line1 range
    if ((line1_a <= line2_a) && (line1_b >= line2_a)) return 1;
    // If line2 end is in line1 range
    if ((line1_a <= line2_b) && (line1_b >= line2_b)) return 1;

    return 0;
}

int Rect_getDistSquare(const int p1x, const int p1y, const int p2x, const int p2y)
{
    int delta_x = abs(p1x - p2x);
    int delta_y = abs(p1y - p2y);
    return (delta_x + delta_y) * (delta_x + delta_y);
}

void Rect_findMiddle_rect(const SDL_Rect rect, int *x, int *y)
{
    *x = rect.x + rect.w / 2;
    *y = rect.y + rect.h / 2;
}

int Rect_is_nearest_than(const int dist, const SDL_Rect rect1, SDL_Rect rect2)
{
    int deltaSquare = 0;
    int distSquare = dist * dist;
    int mid1X = 0, mid1Y = 0, mid2X = 0, mid2Y = 0;
    Rect_findMiddle_rect(rect1, &mid1X, &mid1Y);
    Rect_findMiddle_rect(rect2, &mid2X, &mid2Y);

    deltaSquare = Rect_getDistSquare(mid1X, mid1Y, mid2X, mid2Y);
    return (deltaSquare < distSquare);
}
