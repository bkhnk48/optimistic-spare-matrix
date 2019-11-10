#include <stdio.h>
#include <stdlib.h> 

void showSwitchIDs(int *SwitchIDs, int numOfSwitches)
{
    int i = 0;
    for(i = 0; i < numOfSwitches; i++)
    {
        printf("\tWe have SwitchIDs[%d] with its ID: %d\n", i, SwitchIDs[i]);
    }
}

void assignSwitchIDs(int *SwitchIDs, int numOfSwitches)
{
    int i = 0, j = 0;
    //SwitchIDs = malloc( sizeof * SwitchIDs * numOfSwitches);
    for(i = 0; i < numOfSwitches; i++)
    {
        SwitchIDs[i] = i;
    }   
}


/*void assignSwitchPackets(int **SwitchInportIDPcks, 
                            int **SwitchOutportIDPcks, int bufferSize, int numOfSwitches)
{
    int i = 0, j = 0;
    for(i = 0; i < numOfSwitches; i++)
    {
        for(j = 0; j < bufferSize; j++)
        {
            SwitchInportIDPcks[i][j] = -1;
            SwitchOutportIDPcks[i][j] = -1;
        }
    } 
}*/