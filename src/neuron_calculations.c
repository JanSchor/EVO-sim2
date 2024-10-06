
#include <stdlib.h>
#include <math.h>
#include "creature.h"
#include "config.h"
#include "globals.h"

// Sensory neurons
float sensoryRandom(struct Creature* creature) {
    return (float)rand() / RAND_MAX; // 'RAND_MAX' is defined in <stdlib.h> it is used here to return value between 0 and 1
}

float sensoryAge(struct Creature* creature) {
    return (float)currentGenStep / GENERATION_STEPS;
}

float sensoryPositionX(struct Creature* creature) {
    return (float)creature->gridPosX / GRID_X;
}

float sensoryPositionY(struct Creature* creature) {
    return (float)creature->gridPosY / GRID_Y;
}

float sensoryBorderDistanceX(struct Creature* creature) {
    float distance = (float)creature->gridPosX / GRID_X;
    if (distance > 0.5) distance = 1.0-distance;
    return distance*2.0;
}

float sensoryBorderDistanceY(struct Creature* creature) {
    float distance = (float)creature->gridPosY / GRID_Y;
    if (distance > 0.5) distance = 1.0-distance;
    return distance*2.0;
}


// Inner neurons
float innerUniversal(struct Creature* creature) {
    //return tanh(inputs);
    return 0.5f;
}




// Testing computaion
float computeTest(struct Creature* creature) {
    return 0.5f;
}
