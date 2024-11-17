
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "globals.h"

// Calculate size of array based on graph informations
int calculateSizeOfArr(int numOfGens, int log) {
    // finds the highest tracked generation divisible by log
    // divides this generation by log -> gets the num of elements in array
    // adds 1 -> logging generation 0
    return (numOfGens - (numOfGens % log)) / log + 1;
}

Graph* Graph_create(int numOfGenerations) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;

    graph->trackedGenerations = calculateSizeOfArr(numOfGenerations-1, logEveryGen_g);

    if (trackAlive_g) {
        graph->creaturesAlive = (int*)malloc(graph->trackedGenerations * sizeof(int));
        if (graph->creaturesAlive == NULL) {
            perror("Failed to allocate memory for graph creatures alive!\n");
            Graph_destroy(graph);
            return NULL;
        }
    }

    return graph;
}

void Graph_destroy(Graph* graph) {
    free(graph->creaturesAlive);
    free(graph);
}

void trackGeneration(Graph* graph, int idx, int alive) {
    if (trackAlive_g) graph->creaturesAlive[idx] = alive;
}

void graphExport(Graph* graph, char fileNameH[16]) {
    char fileNameGraph[64];
    char headLine[128];

    sprintf(fileNameGraph, "./exports/graph_logs/%s_graph.txt", fileNameH);
    FILE* graphFile = fopen(fileNameGraph, "w");

    sprintf(headLine, "head{ver:%s;cre:%d;}\n", GRAPH_EXPORT_VERSION, creaturesInGen_g);
    fprintf(graphFile, headLine);

    if (trackAlive_g) {
        fprintf(graphFile, "alive{");
        for (int i = 0; i < graph->trackedGenerations; i++) {
            fprintf(graphFile, "%d:%d;", i, graph->creaturesAlive[i]);
            //printf("Log %d: %d\n", i, graph->creaturesAlive[i]);
        }
        fprintf(graphFile, "}\n");
    }
    fclose(graphFile);
}