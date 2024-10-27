
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
    int workWithFileSteps = 0;
    char fileNameSteps[64];

    FILE* fileBrains;
    int workWithFileBrains = 0;
    char fileNameBrains[64];

    set_aliveZone(ALIVE_START_X, ALIVE_START_Y, ALIVE_END_X, ALIVE_END_Y, 0);
    int stepDone;
    for (int i = 0; i < 300; i++) { // The value of 'n' in (i < 'n') represents number of generations, it is raw now, implemented just for tests
        if (i == 0 || i == 285 || i == 291 || i == 5000 || i == 10000 || i == 20000 || i == 50000 || i == 100000) { // File step exports on generations
            sprintf(fileNameSteps, "./exports/gen_logs/%s_sl_%d.txt", fileNameH, i);
            fileSteps = fopen(fileNameSteps, "w");
            workWithFileSteps = 1;
            fileHeaderSteps(fileSteps, i);
        }
        if (i == 0 || i == 285) { // File brain exports on generations
            sprintf(fileNameBrains, "./exports/brain_logs/%s_br_%d.txt", fileNameH, i);
            fileBrains = fopen(fileNameBrains, "w");
            workWithFileBrains = 1;
            fileHeaderBrains(fileBrains, i);
        }
        if (i % status_g == 0) { // Status print every n generations (based on configuration file)
            printf("%d\n", i);
        }
        if (i == WALL_GEN) set_wall(WALL_START_X, WALL_START_Y, WALL_END_X, WALL_END_Y);
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
        if (workWithFileSteps) filePosPartSteps(fileSteps, genOfCreatures, creaturesInGen_g, 0); // Writing starting positions to gen log
        if (workWithFileBrains) fileCrePartBrains(fileBrains, genOfCreatures, creaturesInGen_g); // Writing brains to brain log
        for (currentGenStep = 0; currentGenStep < generationSteps_g; currentGenStep++) { // Looping through each step of generation
            for (int c = 0; c < creaturesInGen_g; c++) stepDone = creatureStep(genOfCreatures[c]); // Executing step for every creature
            if (workWithFileSteps) filePosPartSteps(fileSteps, genOfCreatures, creaturesInGen_g, currentGenStep + 1); // Writing positions after each step to gen log
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
        if (workWithFileSteps) {
            fprintf(fileSteps, "foot{ave:%d;}", creaturesAlive);
            fclose(fileSteps);
            workWithFileSteps = 0;
        }
        if (workWithFileBrains) {
            fclose(fileBrains);
            workWithFileBrains = 0;
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
