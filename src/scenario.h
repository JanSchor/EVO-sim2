
#include <stdio.h>
#include "config.h"

#ifndef SCENARIO_H
#define SCENARIO_H

typedef struct Scenario {
    char indivLine[SCENARIO_FILE_MAX_LINE_COUNT][SCENARIO_FILE_MAX_LINE_LENGTH];
    int lineCount;
    char version[16];
    int startingGen;
    int endingGen;
    int currentGen;
    int current_line;
    int isGraph;
} Scenario;

Scenario* Scenario_create(char filePath[MAX_FILE_PATH_SIZE]);
void Scenario_destroy(Scenario* scenario);

int setHead(char headLine[SCENARIO_FILE_MAX_LINE_LENGTH]);
int setGeneration(Scenario* scenario);
int getGenNumber(char generationLine[SCENARIO_FILE_MAX_LINE_LENGTH]);
void nextGenLine(Scenario* scenario);
void parseGridValues(char* value);
void parseAndSetAliveZone(char* value);
void parseAndSetWall(char* value);
void clearAliveArea();
void clearWallArea();
void setGraph(Scenario* scenario, char graphLine[SCENARIO_FILE_MAX_LINE_LENGTH]);
void setBrains(char brainLine[SCENARIO_FILE_MAX_LINE_LENGTH]);

#endif // SCENARIO_H
