
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neuron.h"

Neuron* Neuron_create(int neuronId, const char* neuronName, const char* neuronShort, float (*neuronCalculation)(Neuron*)) {
    Neuron* neuron = (Neuron*)malloc(sizeof(Neuron));
    if (!neuron) return NULL;

    neuron->neuronId = neuronId;
    strncpy(neuron->neuronName, neuronName, sizeof(neuron->neuronName) - 1);
    neuron->neuronName[sizeof(neuron->neuronName) - 1] = '\0';
    strncpy(neuron->neuronShort, neuronShort, sizeof(neuron->neuronShort) - 1);
    neuron->neuronShort[sizeof(neuron->neuronShort) - 1] = '\0';
    neuron->neuronCalculation = neuronCalculation;

    return neuron;
}

void Neuron_destroy(Neuron* neuron) {
    if (neuron) {
        free(neuron);
    }
}
