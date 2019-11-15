#ifndef MOVE_MANAGER_H
#define MOVE_MANAGER_H

void Move_checkOnUp(Game *game, int dx, int dy);
void Move_checkOnDown(Game *game, int dx, int dy);
void Move_checkOnLeft(Game *game, int dx, int dy);
void Move_checkOnRight(Game *game, int dx, int dy);
void Move_translate(SDL_Rect *player, Casse *casse, int dx, int dy);
int Move_is_casse_on_place(Game *game, Casse *casse);


#endif