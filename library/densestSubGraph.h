#ifndef MY_DSG_H
#define MY_DSG_H

#include "graph.h"
#include "utils.h"

LinkedList* computeDSG(Graph* graph);
void saveDSGToFile(Graph* graph, LinkedList* dsg);

#endif