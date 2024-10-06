
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "creature.h"
#include "config.h"

Creature* Creature_create(int creatureId) {
    Creature* creature = (Creature*)malloc(sizeof(Creature));
    if (!creature) return NULL;

    creature->creatureId = creatureId;

    for (int i = 0; i < BRAIN_SIZE; i++) {
        creature->brain[i] = *Genome_create();
    }

    return creature;
}

void Creature_destroy(Creature* creature) {
    if (creature) {
        free(creature);
        // destroy brain
    }
}
