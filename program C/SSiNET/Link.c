#include <stdio.h> 
#include <stdlib.h> 
#include "Switch.h"

void addLink(int **Link, int **adjacentMatrix, int width, int numOfLinks)
{
    int i = 0, j = 0; int count = 0, index = 0;
    int height = width;
    
    for(i = 0; i < width; i++)
    {
        for(j = 0; j < height; j++)
        {   
            count += adjacentMatrix[i][j];
            index = (count - adjacentMatrix[i][j]*1) % numOfLinks;
            Link[index][0] += i*adjacentMatrix[i][j];/*id cua nut nguon*/
            Link[index][1] += j*adjacentMatrix[i][j];/*id cua nut dich*/
            Link[index][2] = -1;/*id cua goi tin*/
            Link[index][3] = -1;/*id cua host nguon*/
            Link[index][4] = -1;/*id cua host dich*/
            Link[index][5] = 0;/*nix-vector*/
            Link[index][6] = 0;/*kich thuoc cua goi tin*/
            Link[index][7] = 0;/*thoi diem bit dau den*/
            Link[index][8] = 0;/*thoi diem bit cuoi den*/
            Link[index][9] = 0;/*bandwidth*/
            Link[index][10] = 0;/*bang 0 nghia la nut nguon ko phai la Host*/
            Link[index][11] = 0;/*bang 1 nghia la nut dich la Host*/

        }
    }
}

void showLink(int **Links, int count)
{
    int i = 0;
    /*for(i = 0; i < width; i++)
    {
        for(j = 0; j < height; j++)
        {   
            if(adjacentMatrix[i][j] == 1)
                printf("\tLink between %d and %d\n", i, j);
        }
    }*/
    for(i = 0; i < count; i++)
    {
        printf("\tLink between %d and %d\n", Links[i][0], Links[i][1]);
    }
}

void assignLink(int **Links, int k)
{
    int p = 0, offset = 0, e, edgeSwitch, server, s, a;
    int aggSwitch, c, coreSwitch;
    int numServers = k * k * k / 4; int numSwitches = 5 * k * k / 4;
    int numPodSwitches = k * k;
    int **adjacentMatrix = NULL;
    adjacentMatrix = malloc(sizeof * adjacentMatrix * (numServers + numSwitches));
    int i = 0, j = 0;
    for(i = 0; i < numServers + numSwitches; i++)
    {
        adjacentMatrix[i] = malloc(sizeof * adjacentMatrix[i] * (numServers + numSwitches));
        for(j = 0; j < numServers + numSwitches; j++)
        {   adjacentMatrix[i][j] = 0;  }
    }
    // each pod has k^2/4 servers and k switches
    int numOfLinks = 0;
    int numEachPod = k * k / 4 + k;
    for (p = 0; p < k; p++) {
        offset = numEachPod * p;
        for (e = 0; e < k / 2; e++) {
            edgeSwitch = offset + k * k / 4 + e;
            // between server and edge
            for (s = 0; s < k / 2; s++) {
                server = offset + e * k / 2 + s;
                adjacentMatrix[edgeSwitch][server] = adjacentMatrix[server][edgeSwitch] = 1;
                numOfLinks+= 2;
            }
            // between agg and edge
            for (a = k / 2; a < k; a++) {
                aggSwitch = offset + k * k / 4 + a;
                adjacentMatrix[edgeSwitch][aggSwitch] = adjacentMatrix[aggSwitch][edgeSwitch] = 1;
                numOfLinks+= 2;
            }
            aggSwitch = offset + k * k / 4 + k / 2 + e;
            for (c = 0; c < k / 2; c++) {
                coreSwitch = e * k / 2 + c + numPodSwitches + numServers;
                adjacentMatrix[aggSwitch][coreSwitch] = adjacentMatrix[coreSwitch][aggSwitch] = 1;
                numOfLinks+= 2;
            }
        }
    }

    addLink(Links, adjacentMatrix, numServers + numSwitches, numOfLinks);
    //showLink(Links, numOfLinks);
    free(adjacentMatrix);
}


void showLinksInGraph(Graph graph)
{
    showLink(graph->Links, graph->numOfLinks);
}



