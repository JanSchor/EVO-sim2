
#include "config.h"

#ifndef GRID_H
#define GRID_H

typedef struct Grid {
    int grid_array[GRID_Y][GRID_X];
} Grid;

Grid* Grid_create();
void Grid_destroy(Grid* grid);

void printGrid(Grid* grid);
void clearGrid(Grid* grid);
void setGrid(Grid* grid, int x, int y, int val);
void buildWall(Grid* grid);
int (*findEmptySpaceGrid(Grid* grid))[2];

#endif // GRID_H
