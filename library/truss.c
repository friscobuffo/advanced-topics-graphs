#include "truss.h"
#include "utils.h"
#include "graph.h"
#include <stdlib.h>
#include <stdio.h>

int _computeSupport(Graph* graph, int from, int to);

void _bfsEdgeTruss(Graph* graph, EdgesHashMap* truss);

void _orderTrussArray(Graph* graph, EdgesHashMap* truss, int bin[],
            int** pos, Edge sort[]);

void _decreaseSupport(EdgesHashMap* truss, int bin[], int** pos, Edge sort[], int from, int to);

// assumes graph nodes range from 0 to n-1
EdgesHashMap* computeTruss(Graph* graph) {
    Graph* copy = copyGraph(graph);
    EdgesHashMap* truss = newEdgesHashMap(); // truss
    _bfsEdgeTruss(copy, truss);
    int numberOfNodes = copy->numNodes;
    int numberOfEdges = getEdgesHashMapSize(truss);
    int** pos = (int**)malloc(numberOfNodes*sizeof(int*));
    for (int i = 0; i < numberOfNodes; i++)
        pos[i] = (int*)malloc(numberOfNodes*sizeof(int));
    Edge sort[numberOfEdges]; // sort
    int bin[numberOfNodes-1]; // bin
    _orderTrussArray(copy, truss, bin, pos, sort);
    for (int i = 0; i < numberOfEdges; i++) {
        Edge edge = sort[i];
        int from = edge.from;
        int to = edge.to;
        int t = *(int*)getValueFromEdgesHashMap(truss, from, to);
        if (getListSize(getNeighborsOfNodeInGraph(copy, from)) > getListSize(getNeighborsOfNodeInGraph(copy, from))) {
            int temp = from;
            from = to;
            to = temp;
        }
        LinkedList* neighbors = getNeighborsOfNodeInGraph(copy, from);        
        NodeLinkedList* current = neighbors->first;
        while (current) {
            // from neighbor
            int neighbor = *(int*)current->data;
            current = current->next;
            if (isEdgeInGraph(copy, to, neighbor) && (neighbor != from)) {
                int t2 = *(int*)getValueFromEdgesHashMap(truss, from, neighbor);
                int t3 = *(int*)getValueFromEdgesHashMap(truss, to, neighbor);
                if (t2 > t) _decreaseSupport(truss, bin, pos, sort, from, neighbor);
                if (t3 > t) _decreaseSupport(truss, bin, pos, sort, to, neighbor);
                removeEdgeInGraph(copy, from, neighbor);
            }
        }
    }
    for (int i = 0; i < numberOfNodes; i++)
        free(pos[i]);
    free(pos);
    freeGraph(copy);
    return truss;
}

void _bfsEdgeTruss(Graph* graph, EdgesHashMap* truss) {
    int visited[graph->numNodes];
    for(int i = 0; i < graph->numNodes; i++) visited[i] = 0;
    LinkedList* queue = newList();
    for (int i = 0; i < graph->numNodes; i++) {
        int node = getElementFromDynamicIntArray(graph->nodes, i);
        if (!visited[node]) appendIntToList(queue, node);
        while (!isListEmpty(queue)) {
            int curr = removeFirstFromIntList(queue);
            visited[curr] = 1;
            LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, curr);
            NodeLinkedList* current = neighbors->first;
            while (current) {
                int node = *(int*)current->data;
                if(!visited[node])
                    appendIntToList(queue, node);
                if (!isKeyInEdgesHashMap(truss, curr, node)) {
                    int* s = (int*)malloc(sizeof(int));
                    *s = _computeSupport(graph, curr, node);
                    putInEdgesHashMap(truss, curr, node, s);
                }
                current = current->next;
            }
        }
    }
    freeEmptyList(queue);
}

int _computeSupport(Graph* graph, int from, int to) {
    int support = 0;

    LinkedList* fromNeighbors = getNeighborsOfNodeInGraph(graph, from);
    int fromDegree = getListSize(fromNeighbors);
    LinkedList* toNeighbors = getNeighborsOfNodeInGraph(graph, to);
    int toDegree = getListSize(toNeighbors);

    if (fromDegree < toDegree) {
        LinkedList* tmpNeighbors = fromNeighbors;
        fromNeighbors = toNeighbors;
        toNeighbors = tmpNeighbors;

        int tmpNode = from;
        from = to;
        to = tmpNode;
    }
    NodeLinkedList* current = fromNeighbors->first;
    while (current) {
        int node = *(int*)current->data;
        if (isEdgeInGraph(graph, to, node) && (node != from)) support++;
        current = current->next;
    }
    return support;
}

void _orderTrussArray(Graph* graph, EdgesHashMap* truss, int bin[],
            int** pos, Edge sort[]) {
    const int numberOfNodes = graph->numNodes;
    const int maxTrussness = numberOfNodes-2;
    // populating buckets
    for (int i = 0; i <= maxTrussness; ++i)
        bin[i] = 0;
    for (int from = 0; from < numberOfNodes; ++from) {
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, from);
        NodeLinkedList* current = neighbors->first;
        while (current) {
            int to = *(int*)current->data;
            current = current->next;
            if (from > to) continue;
            int support = *(int*)getValueFromEdgesHashMap(truss, from, to);
            ++bin[support];
        }
    }
    int start = 0;
    for (int i = 0; i <= maxTrussness; ++i) {
        int val = bin[i];
        bin[i] = start;
        start += val;
    }
    for (int from = 0; from < numberOfNodes; ++from) {
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, from);
        NodeLinkedList* current = neighbors->first;
        while (current) {
            int to = *(int*)current->data;
            current = current->next;
            if (from > to) continue;
            int support = *(int*)getValueFromEdgesHashMap(truss, from, to);
            int p = bin[support];
            bin[support]++;
            sort[p].from = from;
            sort[p].to = to;
            pos[from][to] = p;
            pos[to][from] = p;
        }
    }
    for (int i = maxTrussness; i > 0; i--)
        bin[i] = bin[i-1];
    bin[0] = 0;
}

void _decreaseSupport(EdgesHashMap* truss, int bin[], int** pos, Edge sort[], int from, int to) {
    int suppValue = *(int*)getValueFromEdgesHashMap(truss, from, to);
    int bucketIndex = bin[suppValue];
    int realIndex = pos[from][to];
    Edge swapEdge = sort[bucketIndex];

    Edge tmp = sort[bucketIndex];
    sort[bucketIndex] = sort[realIndex];
    sort[realIndex] = tmp;

    pos[from][to] = bucketIndex;
    pos[swapEdge.from][swapEdge.to] = bucketIndex;

    int* trussValuePtr = getValueFromEdgesHashMap(truss, from, to);
    *trussValuePtr = suppValue-1;
    bin[suppValue]++;
}

void saveEdgeMapToFile(Graph* graph, EdgesHashMap* truss, char filePath[]) {
    FILE *file = fopen(filePath, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    fputs("source,target,value\n", file);
    for (int i = 0; i < graph->numNodes; ++i) {
        int node1 = getElementFromDynamicIntArray(graph->nodes, i);
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, node1);
        NodeLinkedList* current = neighbors->first;
        while (current) {
            int node2 = *(int*)current->data;
            current = current->next;
            if (node1>node2) continue;
            int t = *(int*)getValueFromEdgesHashMap(truss, node1, node2);
            fprintf(file, "%d,%d,%d\n", node1, node2, t);
        }
    }
    fclose(file);
}

void saveTrussCountToFile(Graph* graph, EdgesHashMap* truss) {
    int maxTruss = 0;
    for (int i = 0; i < graph->numNodes; ++i) {
        int node1 = getElementFromDynamicIntArray(graph->nodes, i);
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, node1);
        NodeLinkedList* current = neighbors->first;
        while (current) {
            int node2 = *(int*)current->data;
            current = current->next;
            if (node1>node2) continue;
            int t = *(int*)getValueFromEdgesHashMap(truss, node1, node2);
            if (t > maxTruss)
                maxTruss = t;
        }
    }
    int trussesCount[maxTruss+1];
    for (int i = 0; i <= maxTruss; ++i)
        trussesCount[i] = 0;
    for (int i = 0; i < graph->numNodes; i++) {
        int node1 = getElementFromDynamicIntArray(graph->nodes, i);
        LinkedList* neighbors = getNeighborsOfNodeInGraph(graph, node1);
        NodeLinkedList* current = neighbors->first;
        while (current) {
            int node2 = *(int*)current->data;
            current = current->next;
            if (node1>node2) continue;
            int t = *(int*)getValueFromEdgesHashMap(truss, node1, node2);
            ++trussesCount[t];
        }
    }
    FILE *file = fopen("trussCount.csv", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    fputs("truss,count\n", file);
    for (int i = 0; i <= maxTruss; ++i)
        fprintf(file, "%d,%d\n", i, trussesCount[i]);
    fclose(file);
}