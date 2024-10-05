
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genome.h"

static unsigned int generateHex() {
    unsigned int
    x = rand() & 0xff;
    x |= (rand() & 0xff) << 8;
    x |= (rand() & 0xff) << 16;
    x |= (rand() & 0xff) << 24;
    return x;
}

Genome* Genome_create() {
    Genome* genome = (Genome*)malloc(sizeof(Genome));
    if (!genome) return NULL;

    genome->connection = generateHex();
    return genome;
}

void Genome_destroy(Genome* genome) {
    if (genome) {
        free(genome);
    }
}
