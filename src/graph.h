
#ifndef GRAPH_H
#define GRAPH_H

typedef struct Graph {
    int trackedGenerations;
    int* creaturesAlive;
} Graph;

int calculateSizeOfArr(int numOfGens, int log);

Graph* Graph_create(int numOfGenerations);
void Graph_destroy(Graph* graph);

void trackGeneration(Graph* graph, int idx, int alive);

void graphExport(Graph* graph, char fileNameH[16]);

#endif // GRAPH_H
