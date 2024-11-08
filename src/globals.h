
#include "config.h"
#include "alive_zone.h"
#include "wall.h"

#ifndef GLOBALS_H
#define GLOBALS_H

extern int currentGenStep;

// Creatures
extern int brainSize_g;
extern int creaturesInGen_g;

// Simulation
extern int generationSteps_g;
extern int mutationRate_g;

// Grid Specifications
extern int gridX_g;
extern int gridY_g;

// Alive zone
extern int aliveZoneCount_g;
extern AliveZone* aliveZone_g[MAX_SAFE_ZONES];

// Wall
extern int wallCount_g;
extern Wall* wall_g[MAX_WALLS];

// Neurons
extern int sensoryNeurons_g;
extern int innerNeurons_g;
extern int actionNeurons_g;

// Status
extern int status_g;

// Exports
extern int workWithFileSteps_g;
extern int workWithFileBrains_g;

#endif // GLOBALS_H
