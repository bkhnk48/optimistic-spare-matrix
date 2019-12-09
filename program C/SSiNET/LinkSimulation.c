#include<stdio.h>
#include<stdlib.h>
#include "Simulation.c"
#include <limits.h>

void runLinks(Graph g, int *path, int curr)
{
    /* Link[index][0] id cua nut nguon
            Link[index][1] id cua nut dich
            Link[index][2] id cua goi tin
            Link[index][3] id cua host nguon
            Link[index][4] id cua host dich
            Link[index][5] nix-vector
            Link[index][6] kich thuoc cua goi tin
            Link[index][7] thoi diem bit dau den
            Link[index][8] thoi diem bit cuoi den
            Link[index][9] bandwidth
            Link[index][10] bang 0 nghia la nut nguon ko phai la Host
            Link[index][11] bang 1 nghia la nut dich la Host
            Link[index][12] = 0;//Hop count
    */
    int i, j;
    Host *Hosts = g->Hosts;
    int **Links = g->Links;
    int *SwitchIndexes = g->SwitchIndexes;
    int numOfLinks = g->numOfLinks;
    int BUFFER_SIZE = g->BUFFER_SIZE;
    int CYCLE_PACKET = g->CYCLE_PACKET;
    int **MapFromNodesToPorts = g->MapFromNodesToPorts;
    int minNextTime = INT_MAX;
    int sizeOfShift = (8*sizeof(int)- 1);
    
    for(i = 0; i < numOfLinks; i++)
    {
    }
}