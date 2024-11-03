
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

    grid->grid_array = (int**)malloc(gridY_g * sizeof(int*));
    if (grid->grid_array == NULL) {
        perror("Failed to allocate memory for grid_array");
        free(grid);
        return NULL;
    }

    for (int i = 0; i < gridY_g; i++) {
        grid->grid_array[i] = (int*)malloc(gridX_g * sizeof(int));
        if (grid->grid_array[i] == NULL) {
            perror("Failed to allocate memory for grid_array[i]");
            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(grid->grid_array[j]);
            }
            free(grid->grid_array);
            free(grid);
            return NULL;
        }
    }
    //clearGrid(grid);
    return grid;
}

void Grid_destroy(Grid* grid) {
    for (int i = 0; i < gridY_g; i++) {
        free(grid->grid_array[i]);
    }
    free(grid->grid_array);
    free(grid);
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
    if (x >= 0 && x < gridX_g && y >= 0 && y < gridY_g) {
        grid->grid_array[y][x] = val;
    }
}

void buildWall(Grid* grid) {
    int pivotY;
    int pivotX;
    Wall wWall;
    for (int wallI = 0; wallI < wallCount_g; wallI++) {
        wWall = *wall_g[wallI];
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
