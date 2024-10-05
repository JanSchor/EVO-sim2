
#ifndef GENOME_H
#define GENOME_H

typedef struct Genome {
    unsigned int connection;
} Genome;

Genome* Genome_create(); //params

void Genome_destroy(Genome* genome);


#endif // GENOME_H
