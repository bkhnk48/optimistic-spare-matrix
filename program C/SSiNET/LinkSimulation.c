#include<stdio.h>
#include<stdlib.h>
#include "Simulation.c"
#include <limits.h>

void runLinks(Graph g, int *path, int curr)
{
    int i, j;
    Host *Hosts = g->Hosts;
    //int **Hosts;
    int **Links = g->Links;
    int *SwitchIndexes = g->SwitchIndexes;
    int numOfLinks = g->numOfLinks;
    int BUFFER_SIZE = g->BUFFER_SIZE;
    int CYCLE_PACKET = g->CYCLE_PACKET;
    int **MapFromNodesToPorts = g->MapFromNodesToPorts;
    int minNextTime = INT_MAX;
    int sizeOfShift = (8*sizeof(int)- 1);
    
}