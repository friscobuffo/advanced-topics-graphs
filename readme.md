# Large Graph Data Mining

## Table of Contents
- [Description](#description)
  - [Programming Language](#programming-language)
  - [Algorithms](#algorithms)
- [Installation and Usage](#installation-and-usage)
- [Graphs Source](#graphs-source)
- [Visualization of Communities](#visualization-of-communities)
  - [Netscience](#netscience)
  - [Football](#football)
  - [Facebook](#facebook)
- [Authors](#authors)

## Description
This project, made for the course "*Advanced topics for computer science*", is about implementing algorithms to find communities of a graph. In this brief report, we have provided a short description of the algorithms we applied. This will be followed by a summary of the results we obtained.

### Programming Language
The programming language used for this project is C, since we enjoy low level and efficient programming. We implemented everything needed for the project from scratch, without using anything that was not included in the standard library. This choice did not come without challenges, but we had fun :).

### Algorithms
In our study on finding communities within a graph, we chose to use two different algorithms with distinct approaches and goals. Specifically, by applying the algorithm for calculating trussness, we identified communities as all connected components with a truss level above a certain threshold. Similarly, by using the algorithm for finding the densest subgraphs in the "max flow" variant, we identified the community with the highest density in the graph. This approach allowed us to compare the results obtained from both algorithms and evaluate their differences.

## Installation and Usage
To install this project (needed gcc compiler), run:

```bash
git clone https://https://github.com/friscobuffo/advanced-topics-graphs.git
cd advanced-topics-graphs
bash compile.sh
```

To run this project, execute the compiled main file:
```bash
./main
```

## Graphs Source
The graph used to test our algorithms, are found in http://konect.cc/.

In particular, we tested on the following graphs:
 - **netscience** http://konect.cc/networks/dimacs10-netscience
   - nodes: [1,461] edges: [2,742]
 - **ego-facebook** http://konect.cc/networks/ego-facebook
   - nodes: [2,888] edges: [2,981]
 - **dimacs10-football** http://konect.cc/networks/dimacs10-football/
   - nodes: [115] edges: [613]

## Visualization of Communities
The tool utilized to visualize the graphs obtained is *Gephi*. 
 - https://gephi.org/
 - https://github.com/gephi/gephi

The graph drawing algorithm used for all the following plots are the *Fruchterman Reingold* and *Yifan Hu*. We colored with green edges that belong in the communities. 

### Netscience

#### Communities with truss
![Trussness netscience](https://github.com/friscobuffo/advanced-topics-graphs/blob/main/trussness.svg)

In this representation, darker green edges have higher truss value.
It is clear the presence of a really dense community (17-truss, while the graph contains only one edge with higher truss, 18), computing the densest sub graph with the max-flow approach reveals that that community actually is the densest subgraph.

#### Densest subgraph
![Densest netscience](https://github.com/friscobuffo/advanced-topics-graphs/blob/main/dsg.svg)

This example confirms our intuition about trussness having a correlation with density, however in the next example provided, the 5-truss component of the graph (the max is 7) does not coincide with the densest subgraph.

### Football

#### Communities with truss
![Trussness football](https://github.com/friscobuffo/advanced-topics-graphs/blob/main/foot_5_truss.svg)

#### Densest subgraph
![Densest football](https://github.com/friscobuffo/advanced-topics-graphs/blob/main/foot_dsg.svg)

In this particular case, the densest subgraph coincides with the entire graph. However, this seems fair, since the graph itself is pretty dense.

### Facebook
This graph is really big, which makes visualizing it a bit difficult.
#### Communities with truss
Since most components of the graph have the shape of a star (or a dandelion), it is expected to contain few edges with truss value higher than 0 (actually, more than 90% of the edges have truss 0 in this case). This plot highlights in green edges with truss greater than 1.

![Trussness facebook](https://github.com/friscobuffo/advanced-topics-graphs/blob/main/fb_2_truss.svg)

#### Densest subgraph
![Densest facebook](https://github.com/friscobuffo/advanced-topics-graphs/blob/main/fb_dsg.svg)

However, even in this case, the edges with truss greater than 1, are a subset of the edges in the densest subgraph.

## Authors
 - Albero Simone: https://github.com/Simone-Albero
 - Andreola Giordano: https://github.com/friscobuffo