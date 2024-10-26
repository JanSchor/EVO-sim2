
// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <math.h>
#include "creature.h"
#include "neuron.h"
#include "grid.h"
#include "config.h"
#include "globals.h"
#include "errors.c"
#include "genome.h"
#include "help_lib.c"



// Notes
// fire mechanic and night
// premade file with scenario (gen 1000 wall)



// Main function
int main() {
    // Setting clock to measure time the program took
    clock_t begin = clock();

    // Error hendeling
    if (checkForErrors()) return 1;

    // Getting unix time to set random seed and define header of export file name
    int unixTimestamp = time(NULL);
    char fileNameH[16];
    convertToTime(unixTimestamp, fileNameH, sizeof(fileNameH));
    printf("File header: %s\n", fileNameH);

    // Random seed based on time
    srand(unixTimestamp);

    currentGenStep = 0;
    Grid* grid = Grid_create();
    initializeNeurons(grid);
    unsigned int brain_alive[CREATURES_IN_GEN][BRAIN_SIZE];
    unsigned int creaturesAlive;
    int generationNum = 0;


    // Creating list of creatures
    int (*validGridCoords)[2];
    Creature* genOfCreatures[CREATURES_IN_GEN];
    char fileNameSteps[64];
    char filePosSteps[CREATURES_IN_GEN*15];

    FILE* fileSteps;
    int workWithFileSteps = 0;
    int stepDone;
    for (int i = 0; i < 10; i++) { // The value of 'n' in (i < 'n') represents number of generations, it is raw now, implemented just for tests
        if (i == 5) { // File export on generation 5
            sprintf(fileNameSteps, "./exports/%s_steps_log_%d.txt", fileNameH, i);
            fileSteps = fopen(fileNameSteps, "w");
            workWithFileSteps = 1;
            printf("work ini %d\n", workWithFileSteps);
            fileHeaderSteps(fileSteps, i);
            strcpy(filePosSteps, "pos{");
            
        }
        if (i > WALL_GEN && WALL_GEN >= 0) buildWall(grid); // Building wall after set generation in config
        for (int j = 0; j < CREATURES_IN_GEN; j++) {
            validGridCoords = findEmptySpaceGrid(grid);
            setGrid(grid, (*validGridCoords)[0], (*validGridCoords)[1], 10000+j);
            if (generationNum == 0 || creaturesAlive == 0) {
                genOfCreatures[j] = Creature_create(j, (*validGridCoords)[0], (*validGridCoords)[1], NULL); // might make it more optimal in future, dont like two same lines
            } else {
                genOfCreatures[j] = Creature_create(j, (*validGridCoords)[0], (*validGridCoords)[1], brain_alive[rand()%creaturesAlive]);
            }
        }
        if (workWithFileSteps) {
            filePosPartSteps(fileSteps, &genOfCreatures, 0);
        }
        for (currentGenStep = 0; currentGenStep < GENERATION_STEPS; currentGenStep++) {
            //if (workWithFileSteps) fprintf(fileSteps, "%d{", currentGenStep);
            for (int c = 0; c < CREATURES_IN_GEN; c++) {
                stepDone = creatureStep(genOfCreatures[c]);
                // if (workWithFileSteps) fprintf(fileSteps, "%d:%s;", c, getStringAction(stepDone));
            }
            //if (workWithFileSteps) fprintf(fileSteps, "}\n");
            if (workWithFileSteps) filePosPartSteps(fileSteps, &genOfCreatures, currentGenStep+1);
        }
        creaturesAlive = 0;
        for (int c = 0; c < CREATURES_IN_GEN; c++) {
            if (genOfCreatures[c]->gridPosX >= ALIVE_START_X &&
                genOfCreatures[c]->gridPosX <= ALIVE_END_X &&
                genOfCreatures[c]->gridPosY >= ALIVE_START_Y &&
                genOfCreatures[c]->gridPosY <= ALIVE_END_Y) {
                    creaturesAlive++;
                    for (int con = 0; con < BRAIN_SIZE; con++) {
                        brain_alive[creaturesAlive-1][con] = genOfCreatures[c]->brain[con].connection;
                    }
                    //memcpy(brain_alive[creaturesAlive-1], genOfCreatures[c]->brain, sizeof(genOfCreatures[c]->brain));
            }
        }
        
        //printf("Creatures alive: %d\n", creaturesAlive);
        // --- Something to handle 0 creatures alive needs to be here in future ---
        generationNum++;
        // Destroying creatures
        for (int c = 0; c < CREATURES_IN_GEN; c++) {
            Creature_destroy(genOfCreatures[c]);
        }
        // test
        if (i == 0 || i == 10 || i == 50 || i == 299 || i == 1000 || i == 2000 || i == 2999 || i == 5000 || i == 10000 || i == 20000 || i == 30000 || i == 50000 || i == 100000) {
            printf("Creatures alive: %d\n", creaturesAlive);
            printGrid(grid);
            printf("\n");
        }
        clearGrid(grid);

        if (workWithFileSteps) {
            fclose(fileSteps);
            workWithFileSteps = 0;
        }

    }
    destroyNeurons();
    Grid_destroy(grid);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Time: %f\n", time_spent);

    return 0;
}
