#ifndef MY_MIN_CUT_H
#define MY_MIN_CUT_H

#include "graph.h"
#include "utils.h"

void computeMinCut(Graph* graph, LinkedList* s, int source, int sink,
            EdgesHashMap* edgeCapacityG, EdgesHashMap* edgeCapacityL, EdgesHashMap* edgeFlowG, EdgesHashMap* edgeFlowL);

#endif