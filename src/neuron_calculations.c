
#include <stdlib.h>
#include <math.h>
#include "creature.h"
#include "config.h"
#include "globals.h"
#include "grid.h"

// Sensory neurons
float sensoryRandom(struct Creature* creature, struct Grid* grid) {
    return (float)rand() / RAND_MAX; // 'RAND_MAX' is defined in <stdlib.h> it is used here to return value between 0 and 1
}

float sensoryAge(struct Creature* creature, struct Grid* grid) {
    return (float)currentGenStep / GENERATION_STEPS;
}

float sensoryPositionX(struct Creature* creature, struct Grid* grid) {
    return (float)creature->gridPosX / GRID_X;
}

float sensoryPositionY(struct Creature* creature, struct Grid* grid) {
    return (float)creature->gridPosY / GRID_Y;
}

float sensoryBorderDistanceX(struct Creature* creature, struct Grid* grid) {
    float distance = (float)creature->gridPosX / GRID_X;
    if (distance > 0.5) distance = 1.0-distance;
    return distance*2.0;
}

float sensoryBorderDistanceY(struct Creature* creature, struct Grid* grid) {
    float distance = (float)creature->gridPosY / GRID_Y;
    if (distance > 0.5) distance = 1.0-distance;
    return distance*2.0;
}


// Inner neurons
float innerUniversal(struct Creature* creature, struct Grid* grid) { // not in use
    //return tanh(inputs);
    return 0.5f;
}


// Action neurons
float actionMoveRandom(struct Creature* creature, struct Grid* grid) {
    return 1.0;
}

float actionMoveNorth(struct Creature* creature, struct Grid* grid) {
    if (creature->gridPosY > 0 && grid->grid_array[creature->gridPosY-1][creature->gridPosX] == 0) {
        printf("North\n");
    } else printf("Nope N\n");
    return 1.0;
}

float actionMoveSouth(struct Creature* creature, struct Grid* grid) {
    if (creature->gridPosY < GRID_Y-1 && grid->grid_array[creature->gridPosY+1][creature->gridPosX] == 0) {
        printf("South\n");
    } else printf("Nope S\n");
    return 1.0;
}

float actionMoveEast(struct Creature* creature, struct Grid* grid) {
    if (creature->gridPosX < GRID_X-1 && grid->grid_array[creature->gridPosY][creature->gridPosX+1] == 0) {
        printf("East\n");
    } else printf("Nope E\n");
    return 1.0;
}

float actionMoveWest(struct Creature* creature, struct Grid* grid) {
    if (creature->gridPosX > 0 && grid->grid_array[creature->gridPosY][creature->gridPosX-1] == 0) {
        printf("West\n");
    } else printf("Nope W\n");
    return 1.0;
}




// Testing computaion
float computeTest(struct Creature* creature, struct Grid* grid) {
    return 0.5f;
}
