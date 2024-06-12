#ifndef MY_GRAPH_H
#define MY_GRAPH_H

#include <stdbool.h>
#include "utils.h"

//////////////////////////
// GRAPH (UNDIRECTED)
//////////////////////////

typedef struct Graph {
    int numNodes;
    DynamicIntArray* nodes;
    HashMap* _adjacencyList;
    EdgesHashMap* _nodePosMap; // where from < to
    EdgesHashMap* _nodePosMap2; // where from > to
    int numEdges;
} Graph;

Graph* newGraph();

void addNode(Graph* graph, int node);

void removeEdgesOfNode(Graph* graph, int node);

void addEdge(Graph* graph, int from, int to);

void printGraph(Graph* graph);

void freeGraph(void* graph);

LinkedList* getNeighborsOfNodeInGraph(Graph* graph, int node);

bool isEdgeInGraph(Graph* graph, int from, int to);

void removeEdgeInGraph(Graph* graph, int from, int to);

bool isGraphEmpty(Graph* graph);

Graph* copyGraph(Graph* graph);

#endif