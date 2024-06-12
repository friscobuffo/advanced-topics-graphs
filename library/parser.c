#include "parser.h"
#include "graph.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Graph* parse(char filePath[]) {
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }
    Graph* graph = newGraph();
    int src, dest;
    while (fscanf(file, "%d\t%d", &src, &dest) == 2) {
        src--;
        dest--;
        addNode(graph, src);
        addNode(graph, dest);
        if (src != dest)
            addEdge(graph, src, dest);
    }
    fclose(file);
    return graph;
}