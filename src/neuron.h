
#ifndef NEURON_H
#define NEURON_H

typedef struct Neuron {
    int neuronId;
    char neuronName[32];
    char neuronShort[4];
    float (*neuronCalculation)(struct Neuron)
} Neuron;

Neuron* Neuron_create(int neuronId, const char* neuronName, const char* neuronShort, float (*neuronCalculation)(Neuron*)); //params

void Neuron_destroy(Neuron* neuron);


#endif // NEURON_H

