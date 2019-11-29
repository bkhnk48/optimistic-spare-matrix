#include<stdio.h>
#include "TrafficPattern.h"

struct GRAPH{
    int numOfLinks;
    int numOfHosts;
    int numOfPorts;
    int numOfSwitches;
    int BUFFER_SIZE;
    int CYCLE_PACKET;
    int **Links;
    int *IsHost;
    int **Hosts;
    int *SwitchIndexes;//This array holds the indexes of switch IDs. 
                        //For example, we have 4 switches with IDs: 4, 8, 9, 10
                        //their indexes are: 1, 2, 3, 4
    int **MapFromNodesToPorts;
    int **Addresses;
};

	
typedef struct GRAPH *Graph;

struct STEP{
    int node;
    int port;
    struct STEP *next;
};

typedef struct STEP *Step;



void showAddresses(int **Addresses, int nodes)
{
    int i = 0;
    //int **Addresses = graph->Addresses;
    //int nodes = (graph->numOfHosts) + (graph->numOfSwitches);
    for(i = 0; i < nodes;i++)
    {
        printf("\nAddress of Node: %d is %d.%d.%d.%d", i, Addresses[i][0], Addresses[i][1],
                            Addresses[i][2], Addresses[i][3]);
    }
    printf("\n===============================\n");
}