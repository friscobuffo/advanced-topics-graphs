#include "densestSubGraph.h"
#include "utils.h"
#include "graph.h"
#include "minCut.h"
#include <stdlib.h>
#include <stdio.h>

Graph* _initFlowNet(Graph* graph, int source, int target, EdgesHashMap* capacitiesG, EdgesHashMap* capacitiesL);
void _updateFlowNet(Graph* graph, int target, EdgesHashMap* capacitiesG, float* m, float guess);
void _fixFlowNet(Graph* graph, LinkedList* S, int source, EdgesHashMap* capacitiesL,  float* m);

LinkedList* computeDSG(Graph* graph) {
    int source = -1;
    int target = -2;
    float l = 0;
    float n = graph->numNodes;
    float u = graph->numEdges;
    float m = u;

    LinkedList* V = newList();
    EdgesHashMap* capacitiesG = newEdgesHashMap();
    EdgesHashMap* capacitiesL = newEdgesHashMap();

    Graph* flowNet = _initFlowNet(graph, source, target, capacitiesG, capacitiesL);

    EdgesHashMap* flowG = newEdgesHashMap();
    EdgesHashMap* flowL = newEdgesHashMap();
    LinkedList* S = newList();

    int iteration = 0;
    while (u-l >= (1/(n*(n-1)))) {
        printf("\riteration: [%d]", iteration);
        fflush(stdout);
        float guess = (u+l) / 2.0;

        _updateFlowNet(flowNet, target, capacitiesG, &m, guess);
        computeMinCut(flowNet, S, source, target, capacitiesG, capacitiesL, flowG, flowL);

        freeEdgeMap(flowG, free);
        freeEdgeMap(flowL, free);
        flowG = newEdgesHashMap();
        flowL = newEdgesHashMap();

        if (S->size == 0)
            u = guess;
        else {
            l = guess;
            _fixFlowNet(flowNet, S, source, capacitiesL, &m);
            appendListToList(V, S);
        }
    }
    printf("\n");
    freeEdgeMap(capacitiesG, free);
    freeEdgeMap(capacitiesL, free);
    freeEdgeMap(flowG, free);
    freeEdgeMap(flowL, free);
    freeGraph(flowNet);
    freeEmptyList(S);
    return V;
}

Graph* _initFlowNet(Graph* graph, int source, int target, EdgesHashMap* capacitiesG, EdgesHashMap* capacitiesL) {
    Graph* flowNet = copyGraph(graph);
    float m = graph->numEdges;
    
    addNode(flowNet, source);
    addNode(flowNet, target); 

    for (int i = 0; i < graph->numNodes; ++i) {
        int from = getElementFromDynamicIntArray(graph->nodes, i);
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, from);
        float degree = neighbors->size;
        addEdge(flowNet, source, from);
        addEdge(flowNet, from, target);
        float* elem = (float*)malloc(sizeof(float));
        *elem = m;
        putInEdgesHashMap(capacitiesL, source, from, elem);

        elem = (float*)malloc(sizeof(float));
        *elem = 2 * m - degree;
        putInEdgesHashMap(capacitiesG, from, target, elem);

        NodeLinkedList* current = neighbors->first;
        while (current) {
            int to = *(int*)current->data;
            if (from < to) {                
                elem = (float*)malloc(sizeof(float));
                *elem = 1;
                putInEdgesHashMap(capacitiesL, from, to, elem);

                elem = (float*)malloc(sizeof(float));
                *elem = 1;
                putInEdgesHashMap(capacitiesG, to, from, elem);
            }
            current = current->next;
        }
    }
    return flowNet;
}

void _updateFlowNet(Graph* graph, int target, EdgesHashMap* capacitiesG, float* m, float guess){
    for (int i = 0; i < graph->nodes->size - 1; i++) { 
        int node = getElementFromDynamicIntArray(graph->nodes, i);
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, node);
        int degree = neighbors->size - 2;
        if (isEdgeInGraph(graph, node, target)) {
            float* elem = getValueFromEdgesHashMap(capacitiesG, node, target);;
            *elem = (*m) + 2 * guess - degree;
        }
    }
}

void _fixFlowNet(Graph* graph, LinkedList* S, int source, EdgesHashMap* capacitiesL,  float* m){
    NodeLinkedList* current = S->first;
    while (current) {
        int node = *(int*)current->data;
        current = current->next;
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, node);
        int degree = neighbors->size;
        removeEdgesOfNode(graph, node);
        *m -= degree - 2;
    }
    for (int i = 0; i < graph->nodes->size - 1; i++) { 
        int node = getElementFromDynamicIntArray(graph->nodes, i);
        if (isEdgeInGraph(graph, source, node)) {
            float* elem = getValueFromEdgesHashMap(capacitiesL, source, node);
            *elem = *m;
        }
    }
}

void saveDSGToFile(Graph* graph, LinkedList* dsg) {
    const char *filePath = "node2dsg.csv";
    FILE *file = fopen(filePath, "w");

    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    fputs("Id,Label\n", file);
    for (int i = 0; i < graph->numNodes; ++i) {
        int node = getElementFromDynamicIntArray(graph->nodes, i);
        int label = 0;

        if (isInList(dsg, &node, arePointedIntsEqual)) label = 1;
        fprintf(file, "%d,%d\n", node, label);
    }

    fclose(file);
}