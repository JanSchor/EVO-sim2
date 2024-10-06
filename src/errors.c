#include <stdio.h>
#include <stdlib.h>
#include "config.h"

int checkForErrors() {

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
    return 0;
}