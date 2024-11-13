#include "globals.h"
#include "config.h"
#include "alive_zone.h"
#include "wall.h"

// Current step
int currentGenStep = 0;

// Creatures
int brainSize_g = BRAIN_SIZE;
int creaturesInGen_g = CREATURES_IN_GEN;

// Simulation
int generationSteps_g = GENERATION_STEPS;
int mutationRate_g = MUTATION_RATE;
int suddenDeath_g = SUDDEN_DEATH;


// Grid Specifications
int gridX_g = GRID_X;
int gridY_g = GRID_Y;

// Alive zone
int aliveZoneCount_g = 0;
AliveZone* aliveZone_g[MAX_SAFE_ZONES];

// Wall
int wallCount_g = 0;
Wall* wall_g[MAX_WALLS];

// Neurons
int sensoryNeurons_g = SENSORY_NEURONS;
int innerNeurons_g = INNER_NEURONS;
int actionNeurons_g = ACTION_NEURONS;

// Brain import
int brainsFromStart_g = BRAINS_FROM_START;
char brainsImportFilePath_g[MAX_FILE_PATH_SIZE] = "";

// Exports
int workWithFileSteps_g = 0;
int workWithFileBrains_g = 0;

// Status
int status_g = GIVE_STATUS;
