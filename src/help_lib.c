
#include <time.h>
#include <stdio.h>
#include "globals.h"
#include "creature.h"
#include "config.h"
#include "alive_zone.h"
#include "wall.h"
#include "globals.h"

// Convert to format yymmdd_hhmmss
void convertToTime(int unixTimestamp, char* buffer, size_t bufferSize) {
    struct tm* timeInfo;
    time_t rawTime = (time_t)unixTimestamp;
    timeInfo = localtime(&rawTime);
    strftime(buffer, bufferSize, "%y%m%d-%H%M%S", timeInfo);
}

void fileHeaderSteps(FILE* file, int gen) {
    fprintf(file, "head{ver:%s;gen:%d;}\n", GEN_EXPORT_VERSION, gen);
    fprintf(file, "grid{%d,%d;}\n", GRID_X, GRID_Y);
    if (wallCount_g > 0) {
        fprintf(file, "wall{");
        for (int wallI = 0; wallI < wallCount_g; wallI++) {
            fprintf(file, "%d,%d,%d,%d;", wall_g[wallI].startX, wall_g[wallI].startY, wall_g[wallI].endX, wall_g[wallI].endY);
        }
        fprintf(file, "}\n");
    }
    fprintf(file, "safe{%d,%d,%d,%d;}\n", ALIVE_START_X, ALIVE_START_Y, ALIVE_END_X, ALIVE_END_Y);
}

void fileHeaderBrains(FILE* file, int gen) {
    fprintf(file, "head{ver:%s;gen:%d;}\n", BRAIN_EXPORT_VERSION, gen);
}

void filePosPartSteps(FILE* file, Creature* (*listOfC)[CREATURES_IN_GEN], int step) {
    char creaturesPosString[(CREATURES_IN_GEN+1)*15];
    sprintf(creaturesPosString, "pos%d{", step);
    for (int indivC = 0; indivC < creaturesInGen_g; indivC++) {
        sprintf(creaturesPosString, "%s%d:%d,%d;", creaturesPosString, (*listOfC)[indivC]->creatureId, (*listOfC)[indivC]->gridPosX, (*listOfC)[indivC]->gridPosY);
    }
    fprintf(file, "%s}\n", creaturesPosString);
}

void fileCrePartBrains(FILE* file, Creature* (*listOfC)[CREATURES_IN_GEN]) {
    char creatureBrainString[(BRAIN_SIZE+1)*10];
    char endChar;
    for (int indivC = 0; indivC < creaturesInGen_g; indivC++) {
        sprintf(creatureBrainString, "cre%d{", indivC);
        endChar = ',';
        for (int con = 0; con < brainSize_g; con++) {
            if (con+1 == brainSize_g) endChar = ';';
            sprintf(creatureBrainString, "%s%x%c", creatureBrainString, (*listOfC)[indivC]->brain[con], endChar);
        }
        fprintf(file, "%s}\n", creatureBrainString);
    }
}

char* getStringAction(int action) { // Prob useless
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

int isCreatureSafe(Creature* c) {
    int posX = c->gridPosX;
    int posY = c->gridPosY;
    AliveZone workingAliveZone;
    for (int i = 0; i < aliveZoneCount_g; i++) {
        workingAliveZone = aliveZone_g[i];
        if (posX >= workingAliveZone.startAliveX &&
            posX <= workingAliveZone.endAliveX &&
            posY >= workingAliveZone.startAliveY &&
            posY <= workingAliveZone.endAliveY) { return 1; }
    }
    return 0;
}

void set_aliveZone(int sx, int sy, int ex, int ey, int param) {
    aliveZone_g[aliveZoneCount_g] = *AliveZone_create(sx, sy, ex, ey, param);
    aliveZoneCount_g++;
}

void set_wall(int sx, int sy, int ex, int ey) {
    wall_g[wallCount_g] = *Wall_create(sx, sy, ex, ey);
    wallCount_g++;
}