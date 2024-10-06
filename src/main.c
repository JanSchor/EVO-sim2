
// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<windows.h>
#include "creature.h"
#include "neuron.h"
#include "grid.h"
#include "neuron_calculations.c"
#include "config.h"


// Notes
// Mutation 1:1000 per genome
// https://youtu.be/N3tRFayqVtk?si=DoGmfUdZsobps68d
// fire mechanic and night
// premade file with scenario



// Defines
#define NEURON_LIST_SIZE 64
#define MAX_INNER_NEURON_COUNT 100 // better to not change, naming will go brrr

// Grid
int grid[GRID_Y][GRID_X];



// Functions



// Main function
int main() {
    // Random seed based on time
    srand(time(NULL));


    // Error hendeling
    if (INNER_NEURONS > MAX_INNER_NEURON_COUNT) {
        fprintf(stderr, "You can not have more than %d inner neurons!\n", MAX_INNER_NEURON_COUNT);
        return 1;
    }
    if (INNER_NEURONS > NEURON_LIST_SIZE) {
        fprintf(stderr, "You can not have more inner neurons than max cap stored in 'NEURON_LIST_SIZE'!\n");
        return 1;
    }
    if (CREATURES_IN_GEN > (GRID_X * GRID_Y)) {
        fprintf(stderr, "There is no space on the grid for new creatures, check 'CREATURES_IN_GEN' value relative to 'GRID_X' and 'GRID_Y'!\n");
        return 1;        
    }


    Neuron* sensorNeurons[NEURON_LIST_SIZE];
    Neuron* actionNeurons[NEURON_LIST_SIZE];
    Neuron* innerNeurons[NEURON_LIST_SIZE];

    // Sensory neurons creation
    sensorNeurons[0] = Neuron_create(0, 0, "Random", "Rnd", computeTest);

    // Action neurons creation
    actionNeurons[0] = Neuron_create(2, 0, "MoveRandom", "MvR", computeTest);

    // Inner neurons creation
    char nameBuffer[32];
    char shortNameBuffer[4];
    for (int i = 0; i < INNER_NEURONS; i++) {
        sprintf(nameBuffer, "Inner%02d", i); // %02d gets nums from 00 - 99 based on 'i' (cap for inner neurons is at 100, stored in 'MAX_INNER_NEURON_COUNT')
        sprintf(shortNameBuffer, "I%02d", i);
        innerNeurons[i] = Neuron_create(1, i, nameBuffer, shortNameBuffer, computeInner);
    }

    Grid* grid = Grid_create();


    // Creating list of creatures
    int (*validGridCoords)[2];
    Creature* genOfCreatures[CREATURES_IN_GEN]; //check ids
    for (int i = 0; i < CREATURES_IN_GEN; i++) {
        validGridCoords = findEmptySpaceGrid(grid);
        setGrid(grid, (*validGridCoords)[0], (*validGridCoords)[1], 10000+i);
        genOfCreatures[i] = Creature_create(i, (*validGridCoords)[0], (*validGridCoords)[1]);
    }

    //printGrid(grid);
    //printInfoCreature(genOfCreatures[0]);
    
    // Destroying creatures
    for (int i = 0; i < CREATURES_IN_GEN; i++) {
        Creature_destroy(genOfCreatures[i]);
    }

    // Destroying neurons
    for (int i = 0; i < NEURON_LIST_SIZE; i++) {
        Neuron_destroy(sensorNeurons[i]);
        Neuron_destroy(innerNeurons[i]);
        Neuron_destroy(actionNeurons[i]);
    }
    Grid_destroy(grid);
    
    // printGrid();

    return 0;
}
