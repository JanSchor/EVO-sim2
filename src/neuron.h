
#include "creature.h"

#ifndef NEURON_H
#define NEURON_H

typedef struct Neuron {
    int neuronType; // 0 - sensory; 1 - inner; 2 - action;
    int neuronId;
    char neuronName[32];
    char neuronShort[4];
    float (*neuronCalculation)(struct Creature*);
    float neuronDefValue;
} Neuron;

Neuron* Neuron_create(
    const int neuronType,
    const int neuronId,
    const char* neuronName,
    const char* neuronShort,
    float (*neuronCalculation)(struct Creature*)
);

void Neuron_destroy(Neuron* neuron);


#endif // NEURON_H

