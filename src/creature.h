
#include "genome.h"
#include "config.h"

#ifndef CREATURE_H
#define CREATURE_H

typedef struct Creature {
    int creatureId;
    Genome brain[BRAIN_SIZE];
    int gridPosX;
    int gridPosY;
} Creature;

void initializeNeurons();
void destroyNeurons();

Creature* Creature_create(int creatureId, int gridPosX, int gridPosY);
void Creature_destroy(Creature* creature);

void printInfoCreature(Creature* creature);

void calculateCreatureSensory(Creature* creature);



#endif // CREATURE_H

