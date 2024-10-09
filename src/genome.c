
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genome.h"
#include "config.h"

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


// Genome information functions - applying mask and shifting to the right
// Informations are stored as hex value. It is exact principle from this part of the video https://youtu.be/N3tRFayqVtk?si=N4hTzwLgKcWLvfyX&t=2473
// 0 - Source type
// 1 to 7 - Source ID
// 8 - Sink type
// 9 to 15 - Sink ID
// 16 to 31 - Connection weight
int getSource(unsigned int connection) {
    return (connection & 0x80000000) >> 31; // 0 - sensory; 1 - inner
}

int getSink(unsigned int connection) {
    return (connection & 0x800000) >> 23; // 0 - inner; 1 - action
}

int getSourceId(unsigned int connection) {
    int divider = SENSORY_NEURONS;
    if (getSource(connection)) divider = INNER_NEURONS;
    return ((connection & 0x7f000000) >> 24) % divider;
}

int getSinkId(unsigned int connection) {
    int divider = INNER_NEURONS;
    if (getSink(connection)) divider = ACTION_NEURONS;
    return ((connection & 0x7F0000) >> 16) % divider;
}

// Only difference here is that there is no shifting (it is already shifted), but subtracting 32767 (2^15) to get value between -32767 and 32768
double getWeight(unsigned int connection) {
    return (double)((signed int)((connection & 0x0000ffff) - 32767)) / 8191;
}