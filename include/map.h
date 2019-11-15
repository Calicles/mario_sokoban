#ifndef MAP_H
#define MAP_H

#include <stdio.h>

typedef struct Map Map;
struct Map
{
    char **matrix;
    int width;
    int height;
};

int Map_init(Map *map, FILE *lvlFile, const int row_to_skip);
int Map_initVirgin(Map *map);
int Map_getElemAt(const Map *map, const int i, const int j);
void Map_setTileAt(Map *map, const int row, const int col, const int item_const);
void Map_free(Map *map);

#endif