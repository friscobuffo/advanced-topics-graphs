gcc -O3 -pg main.c \
    library/graph.c \
    library/utils.c \
    library/parser.c \
    library/truss.c \
    library/minCut.c \
    library/densestSubGraph.c \
    -o main \
    -Wall \
    -Wextra