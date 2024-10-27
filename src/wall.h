
#ifndef WALL_H
#define WALL_H

typedef struct Wall {
    int startX;
    int startY;
    int endX;
    int endY;
} Wall;

Wall* Wall_create(int startX, int startY, int endX, int andY);
void Wall_destroy(Wall* az);

#endif // WALL_H
