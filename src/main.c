
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



// Notes
// Mutation 1:1000 per genome
// https://youtu.be/N3tRFayqVtk?si=DoGmfUdZsobps68d
// fire mechanic and night
// premade file with scenario (gen 1000 wall)



// Main function
int main() {
    // Error hendeling
    if (checkForErrors()) return 1;

    // Random seed based on time
    srand(time(NULL));

    currentGenStep = 0;
    initializeNeurons();
    Grid* grid = Grid_create();


    // Creating list of creatures
    int (*validGridCoords)[2];
    Creature* genOfCreatures[CREATURES_IN_GEN];
    for (int i = 0; i < CREATURES_IN_GEN; i++) {
        validGridCoords = findEmptySpaceGrid(grid);
        setGrid(grid, (*validGridCoords)[0], (*validGridCoords)[1], 10000+i);
        genOfCreatures[i] = Creature_create(i, (*validGridCoords)[0], (*validGridCoords)[1]);
    }

    
    printInfoCreature(genOfCreatures[0]);
    printBrainCreature(genOfCreatures[0]);
    
    for (int i = 0; i < INNER_NEURONS; i++) {
        printf("%d\n", genOfCreatures[0]->innerSinkCount[i]);
    }
    printf("\n");
    for (int i = 0; i < ACTION_NEURONS; i++) {
        printf("%d\n", genOfCreatures[0]->actionSinkCount[i]);
    }
    
    printf("Action: %d\n", calculateCreatureAction(genOfCreatures[0]));

    // Destroying creatures
    for (int i = 0; i < CREATURES_IN_GEN; i++) {
        Creature_destroy(genOfCreatures[i]);
    }

    destroyNeurons();
    Grid_destroy(grid);
    

    //printf("%f, %f\n", 0.0f, tanh(0.0f));
    return 0;
}
