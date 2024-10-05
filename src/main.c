
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "creature.h"
#include "neuron_calculations.c"


// Notes
// Mutation 1:1000 per genome
// https://youtu.be/N3tRFayqVtk?si=DoGmfUdZsobps68d
// fire mechanic and night



// Consts
#define GRID_X 20
#define GRID_Y 20

int grid[GRID_Y][GRID_X];

void printGrid() {
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            printf("%d ", grid[i][j] = 0);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));

    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            grid[i][j] = 0;
        }
    }

    Creature* creature1 = Creature_create(1);

    if (creature1) {
        for (int i = 0; i < BRAIN_SIZE; i++) {
            printf("%x\n", creature1->brain[i]);
        }
    }

    Creature_destroy(creature1);

    
    // printGrid();

    return 0;
}
