#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include "img_const.h"
#include "rect.h"
#include "img_reader.h"
#include "window.h"
#include <SDL/SDL.h>
#include "game_const.h"
#include "game.h"
#include "menu.h"

int main(int arc, char *argv[])
{
        /*Window window;
        if (Window_init(&window, "title", MARIO_B))
        {
            Game_loop(&window, "level1.lvl");

        }
        Window_quit();

        //Game_test("level1.lvl");*/
        Menu_loop();
        Window_quit();
}