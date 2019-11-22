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


void mappingNodeToPort(int **MapFromNodesToPorts, int **Links, int *SwitchIndexes, 
                            int numOfLinks, int numOfPorts)
{
    int i = 0, x = 0, y = 0, z = 0; int index = 0;int count = 0; int previousNode = 0;
    for(i = 0; i < numOfLinks; i++)
    {
        int idSrcNode = Links[i][0];
        int idDstNode = Links[i][1];
        index = SwitchIndexes[idSrcNode];
        z = (index)>>(sizeof(int)*8 - 1);
        //Neu index < 0 thi z = -1, nguoc lai z = 0
        z = z + 1; //z = 0 neu day la Host hoac 1 neu day la Switch

        y = (previousNode - idSrcNode)>>(sizeof(int)*8 - 1);
        //Neu previousNode < idSrcnode thi y = -1, nguoc lai y = 0
        count = count*(1 + y);

        MapFromNodesToPorts[z*index][count] = MapFromNodesToPorts[z*index][count] + z*idDstNode;
        count++;
        x = (count - numOfPorts)>>(sizeof(int)*8 - 1);
        //Neu count < numOfPorts thi x = -1, nguoc lai x = 0
        
        count = (-x)*count; 
        previousNode = idSrcNode;
    } 
}