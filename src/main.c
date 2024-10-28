
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
#include "help_lib.h"
#include "scenario.h"


// Notes
// fire mechanic and night
// premade file with scenario (gen 1000 wall)

// Main function
int main() {
    // Setting clock to measure time the program took
    clock_t begin = clock();
    Scenario* scenario = Scenario_create("./export_formats/example_scenario.txt");
    setGeneration(scenario);
    nextGenLine(scenario);

    // Error hendeling
    if (checkForErrors()) return 1;

    // Getting unix time to set random seed and define header of export file name
    int unixTimestamp = time(NULL);
    char fileNameH[16];
    convertToTime(unixTimestamp, fileNameH, sizeof(fileNameH));

    // Random seed based on time
    srand(unixTimestamp);

    currentGenStep = 0;
    Grid* grid = Grid_create();
    initializeNeurons(grid);
    unsigned int* brain_alive_block = (unsigned int*)malloc(creaturesInGen_g * brainSize_g * sizeof(unsigned int));
    unsigned int** brain_alive = (unsigned int**)malloc(creaturesInGen_g * sizeof(unsigned int*));
    for (int i = 0; i < creaturesInGen_g; i++) {
        brain_alive[i] = brain_alive_block + i * brainSize_g;
    }

    unsigned int creaturesAlive;
    int generationNum = 0;


    // Creating list of creatures
    int (*validGridCoords)[2];
    Creature** genOfCreatures = (Creature**)malloc(creaturesInGen_g * sizeof(Creature*));

    FILE* fileSteps;
    char fileNameSteps[64];

    FILE* fileBrains;
    char fileNameBrains[64];

    int stepDone;
    for (int i = scenario->startingGen; i < scenario->endingGen+1; i++) { // The value of 'n' in (i < 'n') represents number of generations, it is raw now, implemented just for tests
        if (i == scenario->currentGen) {
            setGeneration(scenario);
            nextGenLine(scenario);
        }
        if (workWithFileSteps_g) {
            sprintf(fileNameSteps, "./exports/gen_logs/%s_sl_%d.txt", fileNameH, i);
            fileSteps = fopen(fileNameSteps, "w");
            fileHeaderSteps(fileSteps, i);
        }
        if (workWithFileBrains_g) {
            sprintf(fileNameBrains, "./exports/brain_logs/%s_br_%d.txt", fileNameH, i);
            fileBrains = fopen(fileNameBrains, "w");
            fileHeaderBrains(fileBrains, i);
        }
        if (i % status_g == 0) { // Status print every n generations (based on configuration file or scenario)
            printf("Status log on gen %d:\n", i);
        }
        buildWall(grid); // Building all walls
        for (int j = 0; j < creaturesInGen_g; j++) {
            validGridCoords = findEmptySpaceGrid(grid);
            setGrid(grid, (*validGridCoords)[0], (*validGridCoords)[1], 10000+j);
            if (generationNum == 0 || creaturesAlive == 0) {
                genOfCreatures[j] = Creature_create(j, (*validGridCoords)[0], (*validGridCoords)[1], NULL); // might make it more optimal in future, dont like two same lines
            } else {
                genOfCreatures[j] = Creature_create(j, (*validGridCoords)[0], (*validGridCoords)[1], brain_alive[rand()%creaturesAlive]);
            }
        }
        if (workWithFileSteps_g) filePosPartSteps(fileSteps, genOfCreatures, creaturesInGen_g, 0); // Writing starting positions to gen log
        if (workWithFileBrains_g) fileCrePartBrains(fileBrains, genOfCreatures, creaturesInGen_g); // Writing brains to brain log
        for (currentGenStep = 0; currentGenStep < generationSteps_g; currentGenStep++) { // Looping through each step of generation
            for (int c = 0; c < creaturesInGen_g; c++) stepDone = creatureStep(genOfCreatures[c]); // Executing step for every creature
            if (workWithFileSteps_g) filePosPartSteps(fileSteps, genOfCreatures, creaturesInGen_g, currentGenStep + 1); // Writing positions after each step to gen log
        }
        
        creaturesAlive = 0;
        for (int c = 0; c < creaturesInGen_g; c++) {
            if (isCreatureSafe(genOfCreatures[c])) {
                    creaturesAlive++;
                    for (int con = 0; con < brainSize_g; con++) {
                        brain_alive[creaturesAlive-1][con] = genOfCreatures[c]->brain[con].connection;
                    }
            isCreatureSafe(genOfCreatures[c]);
            }
        }

        generationNum++;
        // Destroying creatures
        for (int c = 0; c < creaturesInGen_g; c++) {
            Creature_destroy(genOfCreatures[c]);
        }

        clearGrid(grid);

        // Closing and ending with file working
        if (workWithFileSteps_g) {
            fprintf(fileSteps, "foot{ave:%d;}", creaturesAlive);
            fclose(fileSteps);
            workWithFileSteps_g = 0;
        }
        if (workWithFileBrains_g) {
            fclose(fileBrains);
            workWithFileBrains_g = 0;
        }

    }

    // Program end
    free(genOfCreatures);
    destroyNeurons();
    Grid_destroy(grid);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time: %f\n", time_spent);
    return 0;
}
