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

void assignSwitchIDs(int *SwitchIDs, int numOfPorts)
{
    int i = 0, j = 0, p = 0, index = 0;
    int numOfSwitches = 5 * numOfPorts * numOfPorts / 4;
    int numEachPod = numOfPorts * numOfPorts / 4 + numOfPorts;
    int numServers = numOfPorts * numOfPorts * numOfPorts / 4;
    int numPodSwitches = numOfPorts * numOfPorts;

    // IDs for pod's switches
    for (p = 0; p < numOfPorts; p++) {
        int offset = numEachPod * p;
        for (int s = 0; s < numOfPorts; s++) {
            int switchId = offset + numOfPorts * numOfPorts / 4 + s;
            SwitchIDs[index] = switchId;  //address[switchId] = new Address(10, p, s, 1);
            index++;
        }
    }

    // IDs for core switches
    for (j = 1; j <= numOfPorts / 2; j++) {
        for (i = 1; i <= numOfPorts / 2; i++) {
            int offset = numPodSwitches + numServers;
            int switchId = offset + (j - 1) * numOfPorts / 2 + i - 1;
            SwitchIDs[index] = switchId;
            index++;  //address[switchId] = new Address(10, k, j, i);
        }
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