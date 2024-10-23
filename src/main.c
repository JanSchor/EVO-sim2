
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
//#include "neuron_calculations.c"



// Notes
// Mutation 1:1000 per genome
// https://youtu.be/N3tRFayqVtk?si=DoGmfUdZsobps68d
// fire mechanic and night
// premade file with scenario (gen 1000 wall)



// Main function
int main() {
    clock_t begin = clock();
    // Error hendeling
    if (checkForErrors()) return 1;

    // Random seed based on time
    srand(time(NULL));

    currentGenStep = 0;
    Grid* grid = Grid_create();
    initializeNeurons(grid);
    unsigned int brain_alive[CREATURES_IN_GEN][BRAIN_SIZE];
    unsigned int creaturesAlive;
    int generationNum = 0;


    // Creating list of creatures
    int (*validGridCoords)[2];
    Creature* genOfCreatures[CREATURES_IN_GEN];

    for (int i = 0; i < 5001; i++) { // The value of 'n' in (i < 'n') represents number of generations, it is raw now, implemented just for tests
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
        for (currentGenStep = 0; currentGenStep < GENERATION_STEPS; currentGenStep++) {
            for (int c = 0; c < CREATURES_IN_GEN; c++) {
                creatureStep(genOfCreatures[c]);
            }
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
    }
    destroyNeurons();
    Grid_destroy(grid);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Time: %f\n", time_spent);

    return 0;
}
