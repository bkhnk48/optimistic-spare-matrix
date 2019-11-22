#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h> 
#include "ErrorReport.c"



void echo(int *Link, int count);

void setAddresses(int **Addresses, int numOfPorts);

void echo(int *Link, int count)
{
    int i = 0, j; int index = 0;
    while(i < count)
    {
        //printf("\tFrom %d to %d at Link[%d]", Link[index], Link[index+1], index);
        printf("\tFrom %d to %d", Link[index], Link[index+1]);
        if(Link[index + 2] == -1)
            printf(", khong co goi tin nao ca");
        printf("\n");
        index += 10;
        //i += 10;
        i++;
    }
}




void setAddresses(int **Addresses, int numOfPorts)
{
    int i = 0, j = 0, p = 0, s = 0, e, h;
    int numOfSwitches = 5 * numOfPorts * numOfPorts / 4;
    int numEachPod = numOfPorts * numOfPorts / 4 + numOfPorts;
    int numServers = numOfPorts * numOfPorts * numOfPorts / 4;
    int numPodSwitches = numOfPorts * numOfPorts; int offset = 0;

    // IDs for pod's switches
    for (p = 0; p < numOfPorts; p++) {
        offset = numEachPod * p;
        for (int s = 0; s < numOfPorts; s++) {
            int switchId = offset + numOfPorts * numOfPorts / 4 + s;
            Addresses[switchId][0] = 10;   Addresses[switchId][1] = p;
            Addresses[switchId][2] = s;    Addresses[switchId][3] = 1;
            //printf("Address of pod switch: %d is %d.%d.%d.%d\n", switchId, 10, p, s, 1);
        }
    }
    offset = numPodSwitches + numServers;
    // IDs for core switches
    for (j = 1; j <= numOfPorts / 2; j++) {
        for (i = 1; i <= numOfPorts / 2; i++) {
            int switchId = offset + (j - 1) * numOfPorts / 2 + i - 1;
            Addresses[switchId][0] = 10;       Addresses[switchId][1] = numOfPorts;
            Addresses[switchId][2] = j;        Addresses[switchId][3] = i;
            //printf("Address of core switch: %d is %d.%d.%d.%d\n", switchId, 10, numOfPorts, j, i);
        }
    }
    offset = numEachPod * p;
    for (p = 0; p < numOfPorts; p++) {
        offset = numEachPod * p;
        for (e = 0; e < numOfPorts / 2; e++) {
            for (h = 2; h <= numOfPorts / 2 + 1; h++) {
                int serverId = offset + e * numOfPorts / 2 + h - 2;
                Addresses[serverId][0] = 10;   Addresses[serverId][1] = p;
                Addresses[serverId][2] = e;    Addresses[serverId][3] = h;
                //printf("Address of server: %d is %d.%d.%d.%d\n", serverId, 10, p, e, h);
            }
        }
    }
}