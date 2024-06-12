#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "library/graph.h"
#include "library/utils.h"
#include "library/truss.h"
#include "library/parser.h"
#include "library/densestSubGraph.h"

Graph* customGraph() {
    Graph* graph = newGraph();
    for (int i = 0; i < 15; i++)
        addNode(graph, i);
    
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 4);
    addEdge(graph, 0, 6);
    addEdge(graph, 0, 7);

    addEdge(graph, 1, 2);
    addEdge(graph, 1, 5);
    addEdge(graph, 1, 6);
    addEdge(graph, 1, 7);
    addEdge(graph, 1, 8);

    addEdge(graph, 2, 3);
    addEdge(graph, 2, 9);
    addEdge(graph, 2, 10);

    addEdge(graph, 3, 9);
    addEdge(graph, 3, 10);

    addEdge(graph, 4, 5);
    addEdge(graph, 4, 6);
    addEdge(graph, 4, 7);
    
    addEdge(graph, 5, 6);
    addEdge(graph, 5, 7);
    
    addEdge(graph, 6, 11);
    addEdge(graph, 6, 12);
    addEdge(graph, 6, 13);

    addEdge(graph, 7, 8);
    addEdge(graph, 7, 11);
    addEdge(graph, 7, 12);
    addEdge(graph, 7, 13);
    addEdge(graph, 7, 14);

    addEdge(graph, 8, 9);

    addEdge(graph, 9, 10);
    
    addEdge(graph, 10, 14);

    addEdge(graph, 11, 13);
    addEdge(graph, 11, 12);
    
    addEdge(graph, 12, 13);
    return graph;
}

Graph* miniGraph() {
    Graph* mini = newGraph();
    for (int i = 0; i < 3; ++i)
        addNode(mini, i);
    addEdge(mini, 0, 1);
    addEdge(mini, 0, 2);
    addEdge(mini, 1, 2);
    return mini;
}

Graph* cliqueTree() {
    Graph* graph = newGraph();
    for (int i = 0; i < 6; ++i)
        addNode(graph, i);
    for (int i = 0; i < 5; ++i)
        for (int j = i+1; j < 6; j++)
            addEdge(graph, i, j);
    int prev = 5;
    for (int i = 6; i < 13; ++i) {
        addNode(graph, i);
        addEdge(graph, prev, i);
        prev = i;
    }
    return graph;
}

void computeTrussToFile(Graph* graph) {
    EdgesHashMap* truss = computeTruss(graph);
    saveEdgeMapToFile(graph, truss, "truss.csv");
    saveTrussCountToFile(graph, truss);
    freeEdgeMap(truss, free);
}

EdgesHashMap* colorEdgesDsg(Graph* graph, LinkedList* densestNodes) {
    EdgesHashMap* color = newEdgesHashMap();
    DynamicIntArray* nodes = graph->nodes;
    for (int i = 0; i < nodes->size; ++i) {
        int node = nodes->array[i];
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, node);
        NodeLinkedList* current = neighbors->first;
        while (current) {
            int neighbor = *(int*)current->data;
            current = current->next;
            if (neighbor > node) continue;
            int* c = (int*)malloc(sizeof(int));
            if (isInList(densestNodes, &neighbor, arePointedIntsEqual) && isInList(densestNodes, &node, arePointedIntsEqual))
                *c = 1;
            else
                *c = 0;
            putInEdgesHashMap(color, node, neighbor, c);
        }
    }
    return color;
}

void computeDsgToFile(Graph* graph) {
    LinkedList* densestSubGraphNodes = computeDSG(graph);
    saveDSGToFile(graph, densestSubGraphNodes);
    EdgesHashMap* edgeColor = colorEdgesDsg(graph, densestSubGraphNodes);
    saveEdgeMapToFile(graph, edgeColor, "color.csv");
    freeList(densestSubGraphNodes, free);
    freeEdgeMap(edgeColor, free);
}

int main() {
    Graph* graph = parse("facebookgraph");
    computeTrussToFile(graph);
    computeDsgToFile(graph);
    freeGraph(graph);
    return 0;
}