
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"
#include "config.h"
#include "globals.h"
#include "wall.h"


Grid* Grid_create() {
    Grid* grid = (Grid*)malloc(sizeof(Grid));
    if (!grid) return NULL;

    clearGrid(grid);

    return grid;
}

void Grid_destroy(Grid* grid) {
    if (grid) {
        free(grid);
    }
}

void printGrid(Grid* grid) {
    char sign[6];
    for (int i = 0; i < gridY_g; i++) {
        for (int j = 0; j < gridX_g; j++) {
            sprintf(sign, "%05d", grid->grid_array[i][j]);
            printf("%s ", sign);
        }
        printf("\n");
    }
}

void clearGrid(Grid* grid) {
    // fill the array with all 0
    for (int i = 0; i < gridY_g; i++) {
        for (int j = 0; j < gridX_g; j++) {
            grid->grid_array[i][j] = 0;
        }
    }
}

void setGrid(Grid* grid, int x, int y, int val) {
    grid->grid_array[y][x] = val;
}

void buildWall(Grid* grid) {
    int pivotY;
    int pivotX;
    Wall wWall;
    for (int wallI = 0; wallI < wallCount_g; wallI++) {
        wWall = wall_g[wallI];
        for (pivotY = wWall.startY; pivotY < wWall.endY+1; pivotY++) {
            for (pivotX = wWall.startX; pivotX < wWall.endX+1; pivotX++) {
                setGrid(grid, pivotX, pivotY, 1);
            }
        }
    }
}

int (*findEmptySpaceGrid(Grid* grid))[2] {
    static int coords[2];
    int randomX;
    int randomY;
    for (int i = 0; i < 10; i++) {
        randomX = rand()%gridX_g;
        randomY = rand()%gridY_g;
        if (grid->grid_array[randomY][randomX] == 0) break;
    }
    while (grid->grid_array[randomY][randomX] != 0) {
        randomX++;
        if (randomX == gridX_g) {
            randomX = 0;
            randomY++;
            if (randomY == gridY_g) {
                randomY = 0;
            }
        }
    }
    coords[0] = randomX;
    coords[1] = randomY;
    return &coords;

}

/*
GRID legend:
    0 - empty space
    1 - wall
    10000 to 19999 - creature with id from 0 to 9999 (starting with 1)
*/
