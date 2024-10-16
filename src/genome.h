
#ifndef GENOME_H
#define GENOME_H

typedef struct Genome {
    unsigned int connection;
} Genome;

Genome* Genome_create(unsigned int preMadeConnection);
void Genome_destroy(Genome* genome);

int getSource(unsigned int connection);
int getSink(unsigned int connection);
int getSourceId(unsigned int connection);
int getSinkId(unsigned int connection);
double getWeight(unsigned int connection);

#endif // GENOME_H
