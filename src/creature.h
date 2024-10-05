
#include "genome.h"
#include "config.h"

#ifndef CREATURE_H
#define CREATURE_H

typedef struct Creature {
    int creatureId;
    Genome brain[BRAIN_SIZE];
} Creature;

Creature* Creature_create(int creatureId); //params

void Creature_destroy(Creature* creature);


#endif // CREATURE_H

