
#include <time.h>
#include <stdio.h>
#include "creature.h"
#include "scenario.h"

#ifndef HELP_LIB_H
#define HELP_LIB_H

void convertToTime(int unixTimestamp, char* buffer, size_t bufferSize);
void fileHeaderSteps(FILE* file, int gen);
void fileHeaderBrains(FILE* file, int gen);
void filePosPartSteps(FILE* file, Creature** listOfC, int size, int step);
void fileCrePartBrains(FILE* file, Creature** listOfC, int size);
char* getStringAction(int action);
int isCreatureSafe(Creature* c);
void set_aliveZone(int sx, int sy, int ex, int ey, int param);
void set_wall(int sx, int sy, int ex, int ey);
void swapVals(int* v1, int* v2);
void printStatus(int gen, int alive, clock_t start, int numOfGens);
void loadBrainsOnStart(char filePath[MAX_FILE_PATH_SIZE], unsigned int** brain_alive, Scenario* scenario);

#endif // HELP_LIB_H