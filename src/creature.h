
#include "genome.h"
#include "config.h"
#include "grid.h"

#ifndef CREATURE_H
#define CREATURE_H

typedef struct Creature {
    int creatureId;
    Genome* brain;
    int gridPosX;
    int gridPosY;
    int* innerSinkCount;
    int* actionSinkCount;
    Genome*** brainsInnerNeuronsSink;
    Genome*** brainsActionNeuronsSink;
    double* innerBufferedValues;
} Creature;

void initializeNeurons(Grid* grid);
void destroyNeurons();

Creature* Creature_create(int creatureId, int gridPosX, int gridPosY, unsigned int* passBrain);
void Creature_destroy(Creature* creature);

void printInfoCreature(Creature* creature);
void printBrainCreature(Creature* creature);

int calculateCreatureAction(Creature* creature);
int creatureStep(struct Creature* creature);


#endif // CREATURE_H

