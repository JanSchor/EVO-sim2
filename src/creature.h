
#include "genome.h"
#include "config.h"
#include "grid.h"

#ifndef CREATURE_H
#define CREATURE_H

typedef struct Creature {
    int creatureId;
    Genome brain[BRAIN_SIZE];
    int gridPosX;
    int gridPosY;
    int innerSinkCount[INNER_NEURONS];
    int actionSinkCount[ACTION_NEURONS];
    Genome* brainsInnerNeuronsSink[INNER_NEURONS][BRAIN_SIZE];
    Genome* brainsActionNeuronsSink[ACTION_NEURONS][BRAIN_SIZE];
    double innerBufferedValues[INNER_NEURONS];
    
} Creature;

void initializeNeurons(Grid* grid);
void destroyNeurons();

Creature* Creature_create(int creatureId, int gridPosX, int gridPosY, unsigned int* passBrain);
void Creature_destroy(Creature* creature);

void printInfoCreature(Creature* creature);
void printBrainCreature(Creature* creature);

int calculateCreatureAction(Creature* creature);
void creatureStep(struct Creature* creature);


#endif // CREATURE_H

