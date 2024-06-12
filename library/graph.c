#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

Graph* newGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numNodes = 0;
    graph->_adjacencyList = newHashMap();
    graph->nodes = newDynamicIntArray();
    graph->_nodePosMap = newEdgesHashMap();
    graph->_nodePosMap2 = newEdgesHashMap();
    graph->numEdges = 0;
    return graph;
}

void addNode(Graph* graph, int node) {
    if (isKeyInMap(graph->_adjacencyList, node)) return;
    graph->numNodes = graph->numNodes+1;
    appendToDynamicIntArray(graph->nodes, node);
    putInMap(graph->_adjacencyList, node, newList());
}

// adds edges <from-to> and <to-from>
void addEdge(Graph* graph, int from, int to) {
    if (from > to) {
        int temp = from;
        from = to;
        to = temp;
    }
    LinkedList* neighborsFrom = getFromMap(graph->_adjacencyList, from);
    appendIntToList(neighborsFrom, to);
    LinkedList* neighborsTo = getFromMap(graph->_adjacencyList, to);
    appendIntToList(neighborsTo, from);
    putInEdgesHashMap(graph->_nodePosMap, from, to, neighborsFrom->last);
    putInEdgesHashMap(graph->_nodePosMap2, to, from, neighborsTo->last);
    ++graph->numEdges;
}

void printGraph(Graph* graph) {
    for (int i = 0; i < graph->numNodes; i++) {
        int node = getElementFromDynamicIntArray(graph->nodes, i);
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, node);
        printf("node: %d neighbors: %d [ ", node, getListSize(neighbors));
        printList(neighbors, printIntPtr);
        printf(" ]\n");
    }
}

void freeGraph(void* graph) {
    Graph* g = (Graph*)graph;
    freeDynamicIntArray(g->nodes);
    freeEdgeMap(g->_nodePosMap, doNothing);
    freeEdgeMap(g->_nodePosMap2, doNothing);
    freeMap(g->_adjacencyList, freeIntList);
    free(g);
}

LinkedList* getNeighborsOfNodeInGraph(Graph* graph, int node) {
    return getFromMap(graph->_adjacencyList, node);
}

bool isEdgeInGraph(Graph* graph, int from, int to) {
    if (from < to)
        return (isKeyInEdgesHashMap(graph->_nodePosMap, from, to));
    return (isKeyInEdgesHashMap(graph->_nodePosMap2, from, to));
}

void removeEdgeInGraph(Graph* graph, int from, int to) {
    if (!isEdgeInGraph(graph, from, to)) {
        printf("error edge not in graph, cant remove.\n");
        exit(1);
    }
    if (from > to) {
        int temp = from;
        from = to;
        to = temp;
    }

    NodeLinkedList* node = removeEdgeFromMap(graph->_nodePosMap, from, to);
    int* content = removeNodeFromList(getFromMap(graph->_adjacencyList, from), node);
    free(content);

    node = removeEdgeFromMap(graph->_nodePosMap2, to, from);
    content = removeNodeFromList(getFromMap(graph->_adjacencyList, to), node);
    free(content);
    --graph->numEdges;
}

bool isGraphEmpty(Graph* graph) {
    return (graph->numEdges == 0);
}

Graph* copyGraph(Graph* graph) {
    Graph* copy = newGraph();
    DynamicIntArray* nodes = graph->nodes;
    for (int i = 0; i < nodes->size; i++) {
        int node = getElementFromDynamicIntArray(nodes, i);
        addNode(copy, node);
    }
    for (int i = 0; i < nodes->size; i++) {
        int node = getElementFromDynamicIntArray(nodes, i);
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, node);
        NodeLinkedList* current = neighbors->first;
        while (current) {
            int neighbor = *(int*)current->data;
            if (node < neighbor)
                addEdge(copy, node, neighbor);
            current = current->next;
        }
    }
    return copy;
}

void removeEdgesOfNode(Graph* graph, int node) {
    LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, node);
    while (!isListEmpty(neighbors)) {
        int neighbor = *(int*)neighbors->first->data;
        removeEdgeInGraph(graph, node, neighbor);
    }
}