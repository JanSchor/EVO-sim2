
#ifndef CONFIG_H
#define CONFIG_H

// Creatures
#define BRAIN_SIZE 6
#define CREATURES_IN_GEN 300

// Simulation
#define GENERATION_STEPS 500
#define MUTATION_RATE 40 // Gives the chance of mutation 1 / n

// Grid specifications
#define GRID_X 100
#define GRID_Y 100

// Alive zone
#define ALIVE_START_X 0
#define ALIVE_START_Y 0
#define ALIVE_END_X 5
#define ALIVE_END_Y 99 // handle error for exceeding boundaries

// Wall
#define WALL_GEN 290 // -1 for no wall
#define WALL_START_X 20
#define WALL_START_Y 20
#define WALL_END_X 20
#define WALL_END_Y 80

// Neurons
#define NEURON_LIST_SIZE 64
#define MAX_INNER_NEURON_COUNT 100 // better to not change, naming will go brrr

#define SENSORY_NEURONS 6
#define INNER_NEURONS 3
#define ACTION_NEURONS 5

// Status
#define GIVE_STATUS 5000 // Gives status every n generations

// File working
#define GEN_EXPORT_VERSION "1.0"
#define BRAIN_EXPORT_VERSION "1.0"

// Configuration params
#define MAX_SAFE_ZONES 10
#define MAX_WALLS 16


#endif // CONFIG_H
