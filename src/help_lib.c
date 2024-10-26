
#include <time.h>
#include "globals.h"
#include "creature.h"
#include "config.h"

// Convert to format yymmdd_hhmmss
void convertToTime(int unixTimestamp, char* buffer, size_t bufferSize) {
    struct tm* timeInfo;
    time_t rawTime = (time_t)unixTimestamp;
    timeInfo = localtime(&rawTime);
    strftime(buffer, bufferSize, "%y%m%d-%H%M%S", timeInfo);
}

void fileHeaderSteps(FILE* file, int gen) {
    fprintf(file, "grid{%d,%d;}\n", GRID_X, GRID_Y);
    if (WALL_GEN != -1 && gen >= WALL_GEN) fprintf(file, "wall1{%d,%d,%d,%d;}", WALL_START_X, WALL_START_Y, WALL_END_X, WALL_END_Y);
    fprintf(file, "s1{%d,%d,%d,%d;}\n", ALIVE_START_X, ALIVE_START_Y, ALIVE_END_X, ALIVE_END_Y);
}

void filePosPartSteps(FILE* file, Creature* (*listOfC)[CREATURES_IN_GEN]) {
    char creaturesPosString[CREATURES_IN_GEN*15] = "pos{";
    for (int indivC = 0; indivC < CREATURES_IN_GEN; indivC++) {
        //printInfoCreature((*listOfC)[indivC]);
        sprintf(creaturesPosString, "%s%d:%d,%d;", creaturesPosString, (*listOfC)[indivC]->creatureId, (*listOfC)[indivC]->gridPosX, (*listOfC)[indivC]->gridPosY);
    }
    //printf("%s}\n", creaturesPosString);
    fprintf(file, "%s}\n", creaturesPosString);
}

char* getStringAction(int action) {
    char returnString[4];
    switch (action) {
        case 0:
            return "not";
        case 1:
            return "mvn";
        case 2:
            return "mvs";
        case 3:
            return "mve";
        case 4:
            return "mvw";
        default:
            return "inv";
    }
}