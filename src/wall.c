
#include <stdio.h>
#include <stdlib.h>
#include "wall.h"

Wall* Wall_create(int startX, int startY, int endX, int endY) {
    Wall* wall = (Wall*)malloc(sizeof(Wall));
    if (!wall) return NULL;
    wall->startX = startX;
    wall->startY = startY;
    wall->endX = endX;
    wall->endY = endY;
    return wall;
}

void Wall_destroy(Wall* wall) {
    if (wall) {
        free(wall);
    }
}

void wallInfoDebug(Wall* wall) {
    printf("%d, %d, %d, %d\n", wall->startX, wall->startY, wall->endX, wall->endY);
}