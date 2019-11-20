#include <stdio.h>
#include <stdlib.h> 

void showSwitchIDs(int *SwitchIDs, int numOfSwitches)
{
    int i = 0;
    for(i = 0; i < numOfSwitches; i++)
    {
        printf("\tWe have SwitchIDs[%d] along with its index is: %d\n", i, SwitchIDs[i]);
    }
}

void showSwitchPacketsAtPort(int **SwitchPortPID, 
                int **SwitchPortSrcIDs, int **SwitchPortDstIDs,
                int bufferSize, int numOfSwitches)
{
    int i = 0, j = 0;
    for(i = 0; i < numOfSwitches; i++)
    {
        for(j = 0; j < bufferSize; j++)
        {
            if(SwitchPortPID[i][j] != -1)
            {
                printf("\tWe have packet id = %d stays in port of switch: %d. ", 
                        SwitchPortPID[i][j],
                        i 
                    );
                printf("It is moving from %d to %d", SwitchPortSrcIDs[i][j], 
                            SwitchPortDstIDs[i][j]);
            }
        }
    }
}

void showEvents(char **SwitchEvtTypes, int **SwitchEvtTimes, int numOfPorts, int numOfSwitches)
{
    int i = 0, j = 0;
    for(i = 0; i < numOfSwitches; i++)
    {
        for(j = 0; j < 8*numOfPorts; j++)
        {
            if(SwitchEvtTypes[i][j] != 0)
            {
                if(j < 4*numOfPorts){
                    printf("\tEvent type %c at outport of Switch %d will occur at the time: %d", 
                            SwitchEvtTypes[i][j], i, SwitchEvtTimes[i][j]);
                }
                else
                {
                    if(j < 6*numOfPorts)
                    {    printf("\tEvent type %c at channel of Switch %d will occur at the time: %d", 
                            SwitchEvtTypes[i][j], i, SwitchEvtTimes[i][j]);
                    }
                    else{
                        if(j < 8*numOfPorts)
                            printf("\tEvent type %c at inport of Switch %d will occur at the time: %d", 
                                SwitchEvtTypes[i][j], i, SwitchEvtTimes[i][j]);
                    }
                }
                
            }
        }
    }
}

void assignSwitchIndexes(int *SwitchIndexes, int *IsHost, int size)
{
    int i = 0, index = 0;
    for(i = 0; i < size; i++)
    {
        SwitchIndexes[i] = index*(1 - IsHost[i]) - IsHost[i];
        index+= 1 - IsHost[i];
    }
}


void assignSwitchPackets(int **SwitchInportPID, 
                int **SwitchInportSrcIDs,
                int **SwitchInportDstIDs,
                int bufferSize, int numOfSwitches)
{
    int i = 0, j = 0;
    for(i = 0; i < numOfSwitches; i++)
    {
        for(j = 0; j < bufferSize; j++)
        {
            SwitchInportPID[i][j] = -1;
            SwitchInportSrcIDs[i][j] = -1;
            SwitchInportDstIDs[i][j] = -1;
        }
    } 
}


void assignEvents(char **SwitchEvtTypes, int **SwitchEvtTimes, 
                int numOfPorts, int numOfSwitches)
{
    int i = 0, j = 0;
    for(i = 0; i < numOfSwitches; i++)
    {
        for(j = 0; j < 8*numOfPorts; j++)
        {
            SwitchEvtTypes[i][j] = 0;
            SwitchEvtTimes[i][j] = 0;
        }
    } 
}