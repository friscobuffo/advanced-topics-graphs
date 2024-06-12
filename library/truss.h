#ifndef MY_TRUSS_H
#define MY_TRUSS_H

#include "graph.h"

EdgesHashMap* computeTruss(Graph* graph);

void saveEdgeMapToFile(Graph* graph, EdgesHashMap* truss, char filePath[]);

void saveTrussCountToFile(Graph* graph, EdgesHashMap* truss);

#endif