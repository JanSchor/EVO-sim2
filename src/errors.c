#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "globals.h"

int checkForErrors() {

    if (innerNeurons_g > MAX_INNER_NEURON_COUNT) {
        fprintf(stderr, "You can not have more than %d inner neurons!\n", MAX_INNER_NEURON_COUNT);
        return 1;
    }
    if (innerNeurons_g > NEURON_LIST_SIZE) {
        fprintf(stderr, "You can not have more inner neurons than max cap stored in 'NEURON_LIST_SIZE'!\n");
        return 1;
    }
    if (creaturesInGen_g > (gridX_g * gridY_g)) {
        fprintf(stderr, "There is no space on the grid for new creatures, check 'CREATURES_IN_GEN' value relative to 'GRID_X' and 'GRID_Y'!\n");
        return 1;        
    }
    return 0;
}