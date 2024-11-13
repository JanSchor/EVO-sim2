
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
        free(scenario);
        return NULL;
    }
    int lineIdx = 0;
    while (fgets(scenario->indivLine[lineIdx], sizeof(scenario->indivLine[lineIdx]), scenarioFile)) {
        if (strncmp(scenario->indivLine[lineIdx], "graph{", 6) == 0) setGraph(scenario->indivLine[lineIdx]);
        else if (strncmp(scenario->indivLine[lineIdx], "brain{", 6) == 0) setBrains(scenario->indivLine[lineIdx]);
        else lineIdx++;
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
            else if (strcmp(key, "sd") == 0) { // Sudden death
                suddenDeath_g = atoi(value);
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
    while (token != NULL && strcmp(token, "}\n") != 0 && strcmp(token, "}") != 0) {
        char* colonPos = strchr(token, ':');
        if (colonPos != NULL) {
            *colonPos = '\0';
            char* key = token;
            char* value = colonPos + 1;
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
    scenario->current_line++; // incrementing beyond index
    if (scenario->current_line == scenario->lineCount) return;
    scenario->currentGen = getGenNumber(scenario->indivLine[scenario->current_line]);
}

void parseGridValues(char* value) {
    int xGrid, yGrid;
    sscanf(value, "%d,%d", &xGrid, &yGrid);
    gridX_g = xGrid;
    gridY_g = yGrid;
}

void parseAndSetAliveZone(char* value) {
    int xStart, yStart, xEnd, yEnd, param;
    sscanf(value, "%d,%d,%d,%d,%d", &xStart, &yStart, &xEnd, &yEnd, &param);
    set_aliveZone(xStart, yStart, xEnd, yEnd, param);
}

void parseAndSetWall(char* value) {
    int xStart, yStart, xEnd, yEnd;
    sscanf(value, "%d,%d,%d,%d", &xStart, &yStart, &xEnd, &yEnd);
    if (xStart > xEnd) swapVals(&xStart, &xEnd);
    if (yStart > yEnd) swapVals(&yStart, &yEnd);
    set_wall(xStart, yStart, xEnd, yEnd);
}

void clearAliveArea() {
    for (int azIdx = 0; azIdx < aliveZoneCount_g; azIdx++) {
        AliveZone_destroy(aliveZone_g[azIdx]);
    }
    aliveZoneCount_g = 0;
}

void clearWallArea() {
    for (int wallIdx = 0; wallIdx < wallCount_g; wallIdx++) {
        Wall_destroy(wall_g[wallIdx]);
    }
    wallCount_g = 0;
}

void setGraph(char graphLine[SCENARIO_FILE_MAX_LINE_LENGTH]) {
    char* subString = graphLine + 6;
    char* token = strtok(subString, ";");
    while (token != NULL && strcmp(token, "}\n") != 0) {
        char* colonPos = strchr(token, ':');
        if (colonPos != NULL) {
            *colonPos = '\0';
            char* key = token;
            char* value = colonPos + 1;
            if (strcmp(key, "log") == 0) { // Log graph data every n generation
                printf("Log generation: %s\n", value);
            }
            else {
                printf("Unknown key: %s\n", key);
            }

        } else {
            printf("Invalid token format: %s\n", token);
        }

        token = strtok(NULL, ";");
    }
}

void setBrains(char brainLine[SCENARIO_FILE_MAX_LINE_LENGTH]) {
    char* subString = brainLine + 6;
    char* token = strtok(subString, ";");
    while (token != NULL && strcmp(token, "}\n") != 0) {
        char* colonPos = strchr(token, ':');
        if (colonPos != NULL) {
            *colonPos = '\0';
            char* key = token;
            char* value = colonPos + 1;
            // Only one of the path and name is needed.
            // If it is only the name specified, path will automatically lead to the scenarios/brain_imports folder
            if (strcmp(key, "path") == 0) { // Path to the file
                strcpy(brainsImportFilePath_g, value);
                brainsFromStart_g = 1;
            }
            else if (strcmp(key, "name") == 0) { // Name of the file
                printf("File name: %s\n", value);
                char filePath[MAX_FILE_PATH_SIZE] = "";
                sprintf(filePath, "./scenarios/brain_imports/%s", value);
                strcpy(brainsImportFilePath_g, filePath);
                brainsFromStart_g = 1;
            }
            else {
                printf("Unknown key: %s\n", key);
            }

        } else {
            printf("Invalid token format: %s\n", token);
        }

        token = strtok(NULL, ";");
    }
}