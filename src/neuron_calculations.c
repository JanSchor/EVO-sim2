
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
    return (float)currentGenStep / generationSteps_g;
}

float sensoryPositionX(struct Creature* creature, struct Grid* grid) {
    return (float)creature->gridPosX / gridX_g;
}

float sensoryPositionY(struct Creature* creature, struct Grid* grid) {
    return (float)creature->gridPosY / gridY_g;
}

float sensoryBorderDistanceX(struct Creature* creature, struct Grid* grid) {
    float distance = (float)creature->gridPosX / gridX_g;
    if (distance > 0.5) distance = 1.0-distance;
    return distance*2.0;
}

float sensoryBorderDistanceY(struct Creature* creature, struct Grid* grid) {
    float distance = (float)creature->gridPosY / gridX_g;
    if (distance > 0.5) distance = 1.0-distance;
    return distance*2.0;
}


// Inner neurons
float innerUniversal(struct Creature* creature, struct Grid* grid) { // not in use
    //return tanh(inputs);
    return 0.5f;
}


// Action neurons
float actionMoveNorth(struct Creature* creature, struct Grid* grid) {
    if (creature->gridPosY > 0 && grid->grid_array[creature->gridPosY-1][creature->gridPosX] == 0) {
        grid->grid_array[creature->gridPosY][creature->gridPosX] = 0;
        grid->grid_array[creature->gridPosY-1][creature->gridPosX] = creature->creatureId + 10000;
        creature->gridPosX;
        creature->gridPosY--;
    } else return 0;
    return 1.0;
}

float actionMoveSouth(struct Creature* creature, struct Grid* grid) {
    if (creature->gridPosY < gridY_g-1 && grid->grid_array[creature->gridPosY+1][creature->gridPosX] == 0) {
        grid->grid_array[creature->gridPosY][creature->gridPosX] = 0;
        grid->grid_array[creature->gridPosY+1][creature->gridPosX] = creature->creatureId + 10000;
        creature->gridPosX;
        creature->gridPosY++;
    } else return 0;
    return 2.0;
}

float actionMoveEast(struct Creature* creature, struct Grid* grid) {
    if (creature->gridPosX < gridX_g-1 && grid->grid_array[creature->gridPosY][creature->gridPosX+1] == 0) {
        grid->grid_array[creature->gridPosY][creature->gridPosX] = 0;
        grid->grid_array[creature->gridPosY][creature->gridPosX+1] = creature->creatureId + 10000;
        creature->gridPosX++;
        creature->gridPosY;
    } else return 0;
    return 3.0;
}

float actionMoveWest(struct Creature* creature, struct Grid* grid) {
    if (creature->gridPosX > 0 && grid->grid_array[creature->gridPosY][creature->gridPosX-1] == 0) {
        grid->grid_array[creature->gridPosY][creature->gridPosX] = 0;
        grid->grid_array[creature->gridPosY][creature->gridPosX-1] = creature->creatureId + 10000;
        creature->gridPosX--;
        creature->gridPosY;
    } else return 0;
    return 4.0;
}

float actionMoveRandom(struct Creature* creature, struct Grid* grid) {
    float returnVal;
    switch (rand()%4) // mod4 for 4 directions
    {
    case 0:
        returnVal = actionMoveNorth(creature, grid);
        break;
    case 1:
        returnVal = actionMoveSouth(creature, grid);
        break;
    case 2:
        returnVal = actionMoveEast(creature, grid);
        break;
    case 3:
        returnVal = actionMoveWest(creature, grid);
        break;
    default:
        break;
    }
    return returnVal;
}




// Testing computaion
float computeTest(struct Creature* creature, struct Grid* grid) {
    return 0.5f;
}
