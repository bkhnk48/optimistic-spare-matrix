#include <stdio.h>
#include <stdlib.h> 
#include "Host.h"

void showSwitchIDs(int *SwitchIndexes, int numOfNodes)
{
    int i = 0;
    for(i = 0; i < numOfNodes; i++)
    {
        printf("\tWe have SwitchIDs[%d] along with its index is: %d\n", i, SwitchIndexes[i]);
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
    for(i = 0; i < numOfSwitches*numOfPorts; i++)
    {
        for(j = 0; j < 4; j++)
        {
            //printf("\nshowevent %d %d", i, j);
            if(SwitchEvtTypes[i][j] != 0)
            {
                switch (j)
                {
                    case 0:
                    case 1:
                        printf("\tEvent type %c at outport of Switch %d will occur at the time: %d", 
                            SwitchEvtTypes[i][j], i, SwitchEvtTimes[i][j]);
                        break;
                    case 2:
                        printf("\tEvent type %c at channel of Switch %d will occur at the time: %d", 
                            SwitchEvtTypes[i][j], i, SwitchEvtTimes[i][j]);
                        break;
                    case 3:
                        printf("\tEvent type %c at inport of Switch %d will occur at the time: %d", 
                                SwitchEvtTypes[i][j], i, SwitchEvtTimes[i][j]);
                    default:
                    break;
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


void assignSwitchPackets(int **SwitchPortPID, 
                int **Links,
                int bufferSize, int numOfLinks
                , int numOfPorts)
{
    int i = 0, x = 0, y = 0, z = 0; 
    int index = 0;int count = 0; int previousNode = 0;
    int srcIsHost = 0, dstIsHost = 0;
    for(i = 0; i < numOfLinks; i++)
    {
        int idSrcNode = Links[i][0];
        int idDstNode = Links[i][1];
        srcIsHost = Links[i][10];//index = 1 nghia la nut hien tai la Host, otherwise switch
        z = 1 - srcIsHost; //z = 0 neu day la Host hoac 1 neu day la Switch
        SwitchPortPID[z*index + (1-z)*previousNode][0] = 
                        SwitchPortPID[z*index + (1-z)*previousNode][0] + z*idSrcNode;
        SwitchPortPID[z*index + (1-z)*previousNode][6] = 
                        SwitchPortPID[z*index + (1-z)*previousNode][6] + z*idDstNode;
        //printf("\n\t Access: %d", z*i + (1-z)*previousNode);
        previousNode = index;
        index += z;
    }

    /*for(i = 0; i < index; i++)
    {
        if(Links[i][10] == 0)
        {
            printf("\t port of %d connect from %d to %d\n", SwitchPortPID[i][0],
                                   SwitchPortPID[i][0],  SwitchPortPID[i][6]);             
        }
    }*/
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
       
        //printf("\n\tAt switch %d port %d gonna connect to %d + %d", idSrcNode, count, 
        //        MapFromNodesToPorts[z*index][count]
        //            , z*idDstNode
        //            );
        
        MapFromNodesToPorts[z*index][count] = MapFromNodesToPorts[z*index][count] + z*idDstNode;
        count++;
        x = (count - numOfPorts)>>(sizeof(int)*8 - 1);
        //Neu count < numOfPorts thi x = -1, nguoc lai x = 0
        
        count = (-x)*count; 
        previousNode = idSrcNode;
    } 
}

void showSwitchGraph(Graph graph)
{
    showSwitchIDs(graph->SwitchIndexes, (graph->numOfSwitches) 
                                        + (graph->numOfHosts));
}

void showPortOfSwitch(int **MapFromNodesToPorts, int **Links, 
                            int *SwitchIndexes, 
                            int numOfLinks, int numOfPorts)
{
    int i, j, index, previousNode = -1;
    for(i = 0; i < numOfLinks; i++)
    {
        int idSrcNode = Links[i][0];
        index = SwitchIndexes[idSrcNode];
        if(index >= 0 && idSrcNode != previousNode)
        {
            printf("\nSwitch %d has", idSrcNode);
            for(j = 0; j < numOfPorts; j++)
            {
                printf("\n\tport %d connect to %d", j, MapFromNodesToPorts[index][j]);
            }
        }
        previousNode = idSrcNode;
    }
}