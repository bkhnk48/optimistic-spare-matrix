#include <stdio.h> 
#include <stdlib.h> 
  

void addEdge(int **adjacentMatrix, int u, int v)
{
    //printf("\tLink between %d and %d\n", u, v);
    adjacentMatrix[u][v] = 1;
    adjacentMatrix[v][u] = 1;
}

void addLink(int **Link, int **adjacentMatrix, int width, int height)
{
    int i = 0, j = 0; int count = 0, index = 0;
    for(i = 0; i < width; i++)
    {
        for(j = 0; j < height; j++)
        {   
            count += adjacentMatrix[i][j];
            index = count - adjacentMatrix[i][j]*1;
            if(adjacentMatrix[i][j] != 0)
                printf("index = %d at %d , %d adj[][] = %d\n ", index, i, j, adjacentMatrix[i][j]);
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
        }
    }
}

void showLink(int **adjacentMatrix, int width, int height)
{
    int i = 0, j = 0;
    for(i = 0; i < width; i++)
    {
        for(j = 0; j < height; j++)
        {   
            if(adjacentMatrix[i][j] == 1)
                printf("\tLink between %d and %d\n", i, j);
        }
    }
}

void assignLink(int **Link, int k)
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
        {   
            adjacentMatrix[i][j] = 0;
        }
    }
    // each pod has k^2/4 servers and k switches
    int numEachPod = k * k / 4 + k;
    for (p = 0; p < k; p++) {
        offset = numEachPod * p;
        for (e = 0; e < k / 2; e++) {
            edgeSwitch = offset + k * k / 4 + e;
            // between server and edge
            for (s = 0; s < k / 2; s++) {
                server = offset + e * k / 2 + s;
                addEdge(adjacentMatrix, edgeSwitch, server);
            }
            // between agg and edge
            for (a = k / 2; a < k; a++) {
                aggSwitch = offset + k * k / 4 + a;
                addEdge(adjacentMatrix, edgeSwitch, aggSwitch);
            }
            aggSwitch = offset + k * k / 4 + k / 2 + e;
            for (c = 0; c < k / 2; c++) {
                coreSwitch = e * k / 2 + c + numPodSwitches + numServers;
                addEdge(adjacentMatrix, aggSwitch, coreSwitch);
            }
        }
    }


    //addLink(Link, adjacentMatrix, numServers + numSwitches, numServers + numSwitches);
    showLink(adjacentMatrix, numServers + numSwitches, numServers + numSwitches);
}




