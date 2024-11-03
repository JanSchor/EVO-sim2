
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scenario.h"
#include "globals.h"
#include "help_lib.h"

Scenario* Scenario_create(char filePath[MAX_FILE_PATH_SIZE]) {
    Scenario* scenario = (Scenario*)malloc(sizeof(Scenario));
    if (!scenario) return NULL;


    FILE* scenarioFile = fopen(filePath, "r");
    if (scenarioFile == NULL) {
        perror("Failed to open scenario file");
        free(scenario);
        return NULL;
    }
    
    fgets(scenario->indivLine[0], sizeof(scenario->indivLine[0]), scenarioFile);
    if (setHead(scenario->indivLine[0])) {
        perror("Failed to open scenario file");
        free(scenario);
        return NULL;
    }
    int lineIdx = 0;
    while (fgets(scenario->indivLine[lineIdx], sizeof(scenario->indivLine[lineIdx]), scenarioFile)) {
        lineIdx++;
    }
    fclose(scenarioFile);
    scenario->lineCount = lineIdx;
    strcpy(scenario->version, "0");
    scenario->startingGen = getGenNumber(scenario->indivLine[0]);
    scenario->endingGen = getGenNumber(scenario->indivLine[scenario->lineCount-1]);
    scenario->currentGen = scenario->startingGen;
    scenario->current_line = 0;

    return scenario;
}

void Scenario_destroy(Scenario* scenario) {
    if (scenario) {
        clearAliveArea();
        clearWallArea();
        free(scenario);
    }
}

int setHead(char headLine[SCENARIO_FILE_MAX_LINE_LENGTH]) {
    if (strncmp(headLine, "head{", 5) != 0) {
        perror("Invalid head key, check your file format for head");
        return 1;
    }

    char* subString = headLine + 5;
    char* token = strtok(subString, ";");
    while (token != NULL && strcmp(token, "}\n") != 0) {
        char* colonPos = strchr(token, ':');
        if (colonPos != NULL) {
            *colonPos = '\0';
            char* key = token;
            char* value = colonPos + 1;
            if (strcmp(key, "ver") == 0) { // Version
                printf("Scenario version: %s\n", value);
            }
            else if (strcmp(key, "status") == 0) { // Status
                status_g = atoi(value);
            }
            else if (strcmp(key, "grid") == 0) { // Grid
                parseGridValues(value);
            }
            else if (strcmp(key, "bs") == 0) { // Brain size
                brainSize_g = atoi(value);
            }
            else if (strcmp(key, "inn") == 0) { // Inner neurons count
                innerNeurons_g = atoi(value);
            }
            else {
                printf("Unknown key: %s\n", key);
            }

        } else {
            printf("Invalid token format: %s\n", token);
        }

        token = strtok(NULL, ";");
    }
    return 0;
}

int setGeneration(Scenario* scenario) {
    char generationLine[SCENARIO_FILE_MAX_LINE_LENGTH];
    strcpy(generationLine, scenario->indivLine[scenario->current_line]);
    
    char* strippedLine = strchr(generationLine, '{');
        if (strippedLine != NULL) {
            strippedLine++; // Move past the '{' character
        } else {
            printf("Invalid generation line format: %s\n", generationLine);
            return 1;
        }

    char* token = strtok(strippedLine, ";");
    while (token != NULL && strcmp(token, "}\n") != 0) {
        char* colonPos = strchr(token, ':');
        if (colonPos != NULL) {
            *colonPos = '\0';
            char* key = token;
            char* value = colonPos + 1;

            //printf("Key: %s, Value: %s\n", key, value);

            if (strcmp(key, "alive") == 0) { // Alive zone
                parseAndSetAliveZone(value);
            }
            else if (strcmp(key, "steps") == 0) { // Steps
                generationSteps_g = atoi(value);
            }
            else if (strcmp(key, "wall") == 0) { // Wall
                parseAndSetWall(value);
            }
            else if (strcmp(key, "mr") == 0) { // Mutation rate
                mutationRate_g = atoi(value);
            }
            else if (strcmp(key, "bs") == 0) { // Brain size
                brainSize_g = atoi(value);
            }
            else if (strcmp(key, "cre") == 0) { // Creatures
                creaturesInGen_g = atoi(value);
            }
            else if (strcmp(key, "export") == 0) { // Export
                if (strcmp(value, "s") == 0) workWithFileSteps_g = 1;
                else if (strcmp(value, "b") == 0) workWithFileBrains_g = 1;
            }
            else if (strcmp(key, "clear") == 0) { // Clear
                if (strcmp(value, "wall") == 0) {
                    clearWallArea();
                }
                else if (strcmp(value, "alive") == 0) {
                    clearAliveArea();
                }
            }
            else {
                printf("Unknown key: %s\n", key);
            }

        } else {
            printf("Invalid token format: %s\n", token);
        }

        token = strtok(NULL, ";");
    }

    return 0;
}

// Returns number of generation based on start of the line, -1 if invalid
int getGenNumber(char generationLine[SCENARIO_FILE_MAX_LINE_LENGTH]) {
    int generationNumber;
    if (sscanf(generationLine, "%d{", &generationNumber) == 1) {
        return generationNumber;
    } else {
        printf("Invalid generation line format: %s\n", generationLine);
        return -1; // Return an error code if the format is invalid
    }
}

void nextGenLine(Scenario* scenario) {
    scenario->current_line++;
    scenario->currentGen = getGenNumber(scenario->indivLine[scenario->current_line]);
}

void parseGridValues(char* value) {  // try to replace some parts with sscanf
    int i = 0, j = 0;
    char gridXStr[10], gridYStr[10];
    int commaFound = 0;

    // Initialize buffers
    memset(gridXStr, 0, sizeof(gridXStr));
    memset(gridYStr, 0, sizeof(gridYStr));

    while (value[i] != '\0') {
        if (value[i] == ',') {
            commaFound = 1;
            j = 0; // Reset index for the second buffer
        } else {
            if (!commaFound) {
                gridXStr[j++] = value[i];
            } else {
                gridYStr[j++] = value[i];
            }
        }
        i++;
    }
    // Convert to integers and assign to global variables
    if (strlen(gridXStr) > 0 && strlen(gridYStr) > 0) {
        gridX_g = atoi(gridXStr);
        gridY_g = atoi(gridYStr);
    }
}

void parseAndSetAliveZone(char* value) { // try to replace some parts with sscanf
    int i = 0, j = 0, field = 0;
    char aSxStr[10], aSyStr[10], aExStr[10], aEyStr[10], parStr[10];

    // Initialize buffers
    memset(aSxStr, 0, sizeof(aSxStr));
    memset(aSyStr, 0, sizeof(aSyStr));
    memset(aExStr, 0, sizeof(aExStr));
    memset(aEyStr, 0, sizeof(aEyStr));
    memset(parStr, 0, sizeof(parStr));

    while (value[i] != '\0') {
        if (value[i] == ',') {
            field++;
            j = 0; // Reset index for the next buffer
        } else {
            switch (field) {
                case 0: aSxStr[j++] = value[i]; break;
                case 1: aSyStr[j++] = value[i]; break;
                case 2: aExStr[j++] = value[i]; break;
                case 3: aEyStr[j++] = value[i]; break;
                case 4: parStr[j++] = value[i]; break;
            }
        }
        i++;
    }
    // Convert to integers and call set_aliveZone
    if (strlen(aSxStr) > 0 && strlen(aSyStr) > 0 && strlen(aExStr) > 0 && strlen(aEyStr) > 0 && strlen(parStr) > 0) {
        set_aliveZone(atoi(aSxStr), atoi(aSyStr), atoi(aExStr), atoi(aEyStr), atoi(parStr));
    }
}

void parseAndSetWall(char* value) { // try to replace some parts with sscanf
    int i = 0, j = 0, field = 0;
    char wSxStr[10], wSyStr[10], wExStr[10], wEyStr[10];

    // Initialize buffers
    memset(wSxStr, 0, sizeof(wSxStr));
    memset(wSyStr, 0, sizeof(wSyStr));
    memset(wExStr, 0, sizeof(wExStr));
    memset(wEyStr, 0, sizeof(wEyStr));

    while (value[i] != '\0') {
        if (value[i] == ',') {
            field++;
            j = 0; // Reset index for the next buffer
        } else {
            switch (field) {
                case 0: wSxStr[j++] = value[i]; break;
                case 1: wSyStr[j++] = value[i]; break;
                case 2: wExStr[j++] = value[i]; break;
                case 3: wEyStr[j++] = value[i]; break;
            }
        }
        i++;
    }
    // Convert to integers and call set_wall
    if (strlen(wSxStr) > 0 && strlen(wSyStr) > 0 && strlen(wExStr) > 0 && strlen(wEyStr) > 0) {
        set_wall(atoi(wSxStr), atoi(wSyStr), atoi(wExStr), atoi(wEyStr));
    }
}


void clearAliveArea() {
    for (int azIdx = 0; azIdx < aliveZoneCount_g; azIdx++) {
        AliveZone_destroy(aliveZone_g[azIdx]);
    }
    aliveZoneCount_g = 0;
}

void clearWallArea() { // Replace with destroy
    for (int wallIdx = 0; wallIdx < wallCount_g; wallIdx++) {
        Wall_destroy(wall_g[wallIdx]);
    }
    wallCount_g = 0;
}
