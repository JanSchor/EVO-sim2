#include "globals.h"
#include "config.h"
#include "alive_zone.h"
#include "wall.h"

// Current step
int currentGenStep = 0;

int brainSize_g = BRAIN_SIZE;
int creaturesInGen_g = CREATURES_IN_GEN;

// Simulation
int generationSteps_g = GENERATION_STEPS;
int mutationRate_g = MUTATION_RATE;

// Grid Specifications
int gridX_g = GRID_X;
int gridY_g = GRID_Y;

// Alive zone
int aliveZoneCount_g = 0;
AliveZone aliveZone_g[MAX_SAFE_ZONES];

// Wall
int wallCount_g = 0;
Wall wall_g[MAX_WALLS];

// Neurons
int innerNeurons_g = INNER_NEURONS;

// Status
int status_g = GIVE_STATUS;

/*
Problems with globals to fix:
creature.c
    139
creature.h
    entire struct
grid.h
    struct
help_lib.c
    filePosPartSteps
        entire function
    fileCrePartBrains
        entire function
main.c
    70
*/