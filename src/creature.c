
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "creature.h"
#include "config.h"
#include "neuron.h"
#include "neuron_calculations.c"
#include "globals.h"
#include "genome.h"

Neuron* sensorNeurons[NEURON_LIST_SIZE];
Neuron* actionNeurons[NEURON_LIST_SIZE];
Neuron* innerNeurons[NEURON_LIST_SIZE];
int neuronsInitialized = 0;

void initializeNeurons() {
    if (neuronsInitialized) return;

    // When creating new neurons, update 'SENSORY_NEURONS', 'INNER_NEURONS', 'ACTION_NEURONS' in 'config.h' !!!
    // At least for now, hope to make it more automatic in future.

    // Sensory neurons
    sensorNeurons[0] = Neuron_create(0, 0, "Random", "Rnd", sensoryRandom);
    sensorNeurons[1] = Neuron_create(0, 1, "Age", "Age", sensoryAge);
    sensorNeurons[2] = Neuron_create(0, 2, "PositionX", "PoX", sensoryPositionX);
    sensorNeurons[3] = Neuron_create(0, 3, "PositionY", "PoY", sensoryPositionY);
    sensorNeurons[4] = Neuron_create(0, 4, "BorderDistX", "BoX", sensoryBorderDistanceX);
    sensorNeurons[5] = Neuron_create(0, 5, "BorderDistY", "BoY", sensoryBorderDistanceY);


    // Action neurons creation
    actionNeurons[0] = Neuron_create(2, 0, "MoveRandom", "MvR", computeTest);
    actionNeurons[1] = Neuron_create(2, 1, "MoveNorth", "MvN", computeTest);
    actionNeurons[2] = Neuron_create(2, 2, "MoveSouth", "MvS", computeTest);
    actionNeurons[3] = Neuron_create(2, 3, "MoveEast", "MvE", computeTest);
    actionNeurons[4] = Neuron_create(2, 4, "MoveWest", "MvW", computeTest);

    // Inner neurons creation
    char nameBuffer[32];
    char shortNameBuffer[4];
    for (int i = 0; i < INNER_NEURONS; i++) {
        sprintf(nameBuffer, "Inner%02d", i); // %02d gets nums from 00 - 99 based on 'i' (cap for inner neurons is at 100, stored in 'MAX_INNER_NEURON_COUNT')
        sprintf(shortNameBuffer, "I%02d", i);
        innerNeurons[i] = Neuron_create(1, i, nameBuffer, shortNameBuffer, innerUniversal);
    }

    neuronsInitialized = 1;
}

void destroyNeurons() {
    for (int i = 0; i < NEURON_LIST_SIZE; i++) {
        Neuron_destroy(sensorNeurons[i]);
        Neuron_destroy(innerNeurons[i]);
        Neuron_destroy(actionNeurons[i]);
    }
}


Creature* Creature_create(int creatureId, int gridPosX, int gridPosY) {
    Creature* creature = (Creature*)malloc(sizeof(Creature));
    if (!creature) return NULL;

    creature->creatureId = creatureId;
    creature->gridPosX = gridPosX;
    creature->gridPosY = gridPosY;

    for (int i = 0; i < INNER_NEURONS; i++) {
        creature->innerSinkCount[i] = 0;
        creature->innerBufferedValues[i] = 0.5;
    }
    for (int i = 0; i < ACTION_NEURONS; i++) creature->actionSinkCount[i] = 0;

    // Creating 2d arrays storing pointers to the genomes. Array contains are based on the sink type and id of neuron.
    // Each array is created for inner and action neurons. Then each nester array contains all the genomes pointing to the specific id.
    for (int i = 0; i < BRAIN_SIZE; i++) {
        creature->brain[i] = *Genome_create();
        int sinkId = getSinkId((creature->brain[i]).connection);
        if (getSink((creature->brain[i]).connection) == 0) {
            creature->brainsInnerNeuronsSink[sinkId][creature->innerSinkCount[sinkId]] = &creature->brain[i];
            creature->innerSinkCount[sinkId]++;
        } else {
            creature->brainsActionNeuronsSink[sinkId][creature->actionSinkCount[sinkId]] = &creature->brain[i];
            creature->actionSinkCount[sinkId]++;
        }
    }

    return creature;
}

void Creature_destroy(Creature* creature) {
    if (creature) {
        free(creature);
    }
}

void printInfoCreature(Creature* creature) {
    printf(
        "Creature id: %d\nCreature x pos: %d\nCreature y pos: %d\n",
        creature->creatureId,
        creature->gridPosX,
        creature->gridPosY
    );
}

void printBrainCreature(Creature* creature) {
    for (int i = 0; i < BRAIN_SIZE; i++) {
        printf("%x\n", creature->brain[i]);
    }
}

void calculateCreatureSensory(Creature* creature) {
    float result = sensorNeurons[2]->neuronCalculation(creature);
    printf("%f\n", result);
}

int calculateCreatureAction(Creature* creature) { // Returns id of action neuron that fired, -1 if none
    double innerSinkBuffer;
    double actionSinkBuffer;
    unsigned int connectionBuffer = 0;
    double innerBuffers[INNER_NEURONS];
    // First the output value for every inner neuron is calculted. These values alre stored in the 'innerBuffers' array.
    // The programm loops through the arrays defined above in 'Create_creature'. By this it cal calculate end value for each inner neuron specificly.
    for (int i = 0; i < INNER_NEURONS; i++) {
        for (int j = 0; j < creature->innerSinkCount[i]; j++) {
            connectionBuffer = (*(creature->brainsInnerNeuronsSink[i][j])).connection; // In the 'connectionBuffer' is stored current genome connection the programm works with.
            if (!getSource(connectionBuffer)) { // This if statement figures out the source (sensory or inner neuron)
                innerSinkBuffer += sensorNeurons[getSourceId(connectionBuffer)]->neuronCalculation(creature) * getWeight(connectionBuffer);
            } else {
                innerSinkBuffer += creature->innerBufferedValues[getSourceId(connectionBuffer)] * getWeight(connectionBuffer);
            }
            // The computed addition to the 'innerSinkBuffer' above is made from the source output multiplied by weight of the connection.
        }
        innerBuffers[i] = tanh(innerSinkBuffer); // This puts the value of each inner neuron between -1.0 and 1.0 (smaller differences)
        innerSinkBuffer = 0;
    }
    innerSinkBuffer = 0;
    memcpy(creature->innerBufferedValues, innerBuffers, sizeof(innerBuffers));
    int indexMaxValAction = -1; // -1 represents the 'no neuron fired' outcome
    double maxValTrack = 0; // we eliminate every neurons that have value less than 0
    double curValTrack = 0;
    // Same loop as for inner neurons, but with action neurons.
    for (int i = 0; i < ACTION_NEURONS; i++) {
        for (int j = 0; j < creature->actionSinkCount[i]; j++) {
            connectionBuffer = (*(creature->brainsActionNeuronsSink[i][j])).connection;
            //printf("Source: %d\n", !getSource(connectionBuffer));
            if (!getSource(connectionBuffer)) { // Computation works the same
                innerSinkBuffer += sensorNeurons[getSourceId(connectionBuffer)]->neuronCalculation(creature) * getWeight(connectionBuffer);
            } else {
                innerSinkBuffer += creature->innerBufferedValues[getSourceId(connectionBuffer)] * getWeight(connectionBuffer);
            }
        }
        curValTrack = tanh(innerSinkBuffer); // Again getting values from action neurons between -1.0 and 1.0
        printf("%d: %f\n", i, curValTrack);
        if (curValTrack > maxValTrack) {
            maxValTrack = curValTrack;
            indexMaxValAction = i;
        }
        innerSinkBuffer = 0;
    }
    return indexMaxValAction;
}