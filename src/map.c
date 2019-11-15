#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"
#include "game_const.h"

int Map_init(Map *map, FILE *lvlFile, const int row_to_skip)
{
    char line[MAP_MAX] = {'\0'};
    int count = 0;
    if (lvlFile != NULL)
    {
        while(fgets(line, MAP_MAX, lvlFile) != NULL)
        {
            count++;
        }

        map->width = strlen(line);
        map->height = count;
        map->matrix = (char**) malloc(sizeof(char*) * count);

        rewind(lvlFile);

        for (int i = 0; i < row_to_skip; i++) {
            fgets(line, MAP_MAX, lvlFile);
        }

        for (int i = 0; i < count; i++) {
            map->matrix[i] = (char*) malloc(sizeof(char) * map->width + 1);
            fgets(line, MAP_MAX, lvlFile);
            line[map->width] = '\0';
            strcpy(map->matrix[i], line);
        }

        return EXIT_SUCCESS;

    }else{
        return EXIT_FAILURE;
    }
}

int Map_initVirgin(Map *map)
{
    int length = MAP_NB_TILE_LEN, height = MAP_NB_TILE_HEIGHT;

    map->width = length;
    map->height = height;
    map->matrix = (char**) malloc(sizeof(char*) * height);

    for (int i = 0; i < height; i++)
    {
        map->matrix[i] = (char*) malloc(sizeof(char) * length);
        for(int j = 0; j < length; j++)
        {
            map->matrix[i][j] = '0';
        }
    }
}

int Map_getElemAt(const Map *map, const int i, const int j)
{
    int elem = 0;
    char c = map->matrix[i][j];
    elem = (c - 48);
    return elem;
}

void Map_setTileAt(Map *map, const int row, const int col, const int item_const)
{
    char c = (48 + item_const);
    map->matrix[row][col] = c;
}

void Map_free(Map *map)
{
    if (map->matrix != NULL)
    {
        for (int i = 0; i < map->height; i++) {
            if (map->matrix[i] != NULL)
                free(map->matrix[i]);
        }
        free(map->matrix);
    }
}
