#include <stdio.h>
#include "Link.c"

void addLink(int **Link, int **adjacentMatrix, int width, int numOfLinks, int serialTime, int propagationTime);

void showLink(int **Links, int count);

void assignLink(int **Links, int k, int serialTime, int propagationTime);

void assignPortInfo(int **MapFromNodesToPorts, int **Links, 
                            int *SwitchIndexes, 
                            int numOfLinks, int numOfPorts);

void showLinksInGraph(Graph graph);

