#include "minCut.h"
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <math.h>

bool bfs(Graph* graph, int source, int sink, HashMap* parent, EdgesHashMap* edgeCapacityG, EdgesHashMap* edgeCapacityL,
            EdgesHashMap* edgeFlowG, EdgesHashMap* edgeFlowL) {
    HashMap* visited = newHashMap();
    putInMap(visited, source, NULL);
    LinkedList* queue = newList();
    appendIntToList(queue, source);
    while (!isListEmpty(queue)) {
        int u = removeFirstFromIntList(queue);
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, u);
        NodeLinkedList* current = neighbors->first;
        while (current) {
            int v = *(int*)current->data;
            current = current->next;
            EdgesHashMap* flow2use;
            EdgesHashMap* capacity2use;
            if (u < v) {
                flow2use = edgeFlowL;
                capacity2use = edgeCapacityL;
            }
            else {
                flow2use = edgeFlowG;
                capacity2use = edgeCapacityG;
            }
            if (!isKeyInEdgesHashMap(capacity2use, u, v)) continue;
            float capacity = *(float*)getValueFromEdgesHashMap(capacity2use, u, v);
            float flow;
            if (isKeyInEdgesHashMap(flow2use, u, v))
                flow = *(float*)getValueFromEdgesHashMap(flow2use, u, v);
            else
                flow = 0;
            if (!isKeyInMap(visited, v) && (capacity > flow)) {
                appendIntToList(queue, v);
                if (!isKeyInMap(parent, v)) {
                    int* uElem = (int*)malloc(sizeof(int));
                    *uElem = u;
                    putInMap(parent, v, uElem);
                }
                else {
                    int* p = getFromMap(parent, v);
                    *p = u;
                }
                putInMap(visited, v, NULL);
            }
        }
    }
    bool isSinkVisited = isKeyInMap(visited, sink);
    freeMap(visited, doNothing);
    freeEmptyList(queue);
    return isSinkVisited;
}

float computeMaxFlow(Graph* graph, int source, int sink, EdgesHashMap* edgeCapacityG, EdgesHashMap* edgeCapacityL,
            EdgesHashMap* edgeFlowG, EdgesHashMap* edgeFlowL) {
    HashMap* parent = newHashMap();
    float maxFlow = 0;
    while (bfs(graph, source, sink, parent, edgeCapacityG, edgeCapacityL, edgeFlowG, edgeFlowL)) {
        float pathFlow = INFINITY;
        EdgesHashMap* flow2use;
        EdgesHashMap* capacity2use;
        // Find bottleneck capacity of the augmenting path
        for (int v = sink; v != source; v = *(int*)getFromMap(parent, v)) {
            int u = *(int*)getFromMap(parent, v);
            if (u < v) {
                flow2use = edgeFlowL;
                capacity2use = edgeCapacityL;
            }
            else {
                flow2use = edgeFlowG;
                capacity2use = edgeCapacityG;
            }
            float flow;
            if (isKeyInEdgesHashMap(flow2use, u, v))
                flow = *(float*)getValueFromEdgesHashMap(flow2use, u, v);
            else
                flow = 0;
            float capacity = *(float*)getValueFromEdgesHashMap(capacity2use, u, v);
            if (capacity-flow < pathFlow) pathFlow = capacity-flow;
        }
        // Update flow along the augmenting path
        for (int v = sink; v != source; v = *(int*)getFromMap(parent, v)) {
            int u = *(int*)getFromMap(parent, v);
            if (u < v) {
                flow2use = edgeFlowL;
                capacity2use = edgeCapacityL;
            }
            else {
                flow2use = edgeFlowG;
                capacity2use = edgeCapacityG;
            }
            if (isKeyInEdgesHashMap(flow2use, u, v)) {
                float* flowElem = getValueFromEdgesHashMap(flow2use, u, v);
                *flowElem = *flowElem+pathFlow;
            }
            else {
                float* flowElem = (float*)malloc(sizeof(int));
                *flowElem = pathFlow;
                putInEdgesHashMap(flow2use, u, v, flowElem);
            }
            if (flow2use == edgeFlowG) flow2use = edgeFlowL;
            else flow2use = edgeFlowG;
            if (isKeyInEdgesHashMap(flow2use, u, v)) {
                float* flowElem = getValueFromEdgesHashMap(flow2use, u, v);
                *flowElem = *flowElem-pathFlow;
            }
            else {
                float* flowElem = (float*)malloc(sizeof(int));
                *flowElem = -pathFlow;
                putInEdgesHashMap(flow2use, u, v, flowElem);
            }
        }
        maxFlow += pathFlow;
    }
    freeMap(parent, free);
    return maxFlow;
}

void dfs(Graph* graph, int node, LinkedList* nodes, EdgesHashMap* capacitiesG, EdgesHashMap* capacitiesL,
            EdgesHashMap* flowsG, EdgesHashMap* flowsL) {
    HashMap* visited = newHashMap();
    DynamicIntArray* stack = newDynamicIntArray();
    appendToDynamicIntArray(stack, node);
    while (stack->size > 0) {
        node = removeLastFromDynamicIntArray(stack);
        if (isKeyInMap(visited, node))
            continue;
        putInMap(visited, node, NULL);
        appendIntToList(nodes, node);
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, node);
        NodeLinkedList* current = neighbors->first;
        while (current) {
            int neighbor = *(int*)current->data;
            current = current->next;

            EdgesHashMap* capacity2use;
            if (node < neighbor) capacity2use = capacitiesL;
            else capacity2use = capacitiesG;
            EdgesHashMap* flow2use;
            if (node < neighbor) flow2use = flowsL;
            else flow2use = flowsG;

            if (!isKeyInEdgesHashMap(capacity2use, node, neighbor)) continue;
            float capacity = *(float*)getValueFromEdgesHashMap(capacity2use, node, neighbor);
            float flow;
            if (!isKeyInEdgesHashMap(flow2use, node, neighbor))
                flow = 0;
            else 
                flow = *(float*)getValueFromEdgesHashMap(flow2use, node, neighbor);
            if (!isKeyInMap(visited, neighbor) && capacity-flow>0) {
                appendToDynamicIntArray(stack, neighbor);
            }
        }
    }
    freeDynamicIntArray(stack);
    freeMap(visited, doNothing);
}

void computeMinCut(Graph* graph, LinkedList* s, int source, int sink,
            EdgesHashMap* edgeCapacityG, EdgesHashMap* edgeCapacityL, EdgesHashMap* edgeFlowG, EdgesHashMap* edgeFlowL) {
    assert(s->size == 0);
    computeMaxFlow(graph, source, sink, edgeCapacityG, edgeCapacityL, edgeFlowG, edgeFlowL);
    dfs(graph, source, s, edgeCapacityG, edgeCapacityL, edgeFlowG, edgeFlowL);
    removeFirstFromIntList(s);
}