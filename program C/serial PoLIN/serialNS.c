#include <stdio.h>
#include "Init.c"



int main(int argc, char** argv) 
{
    int numOfPorts = 4;
    int numOfSwitches = 20;
    int numOfHosts = 16;
    int numOfInforAboutSwitch = 12;
    //int switches[numOfSwitches][numOfPorts];
    int **AdjOfSwitches = NULL;//Mang luu tru cac Adjancent cua SWiTCHES
    IntegratedPort **IntegratedPortOfSwitches = NULL;//Mang luu tru cac Integrated Port cua SWITCHES
    AdjOfSwitches = malloc( sizeof * AdjOfSwitches * numOfSwitches );

    IntegratedPortOfSwitches = malloc( sizeof * IntegratedPortOfSwitches * numOfSwitches );

    for (int i = 0; i < numOfSwitches; i++ )
    {
      IntegratedPortOfSwitches[i] = malloc( sizeof *IntegratedPortOfSwitches[i] * numOfPorts );
    }

    for (int i = 0; i < numOfSwitches; i++ )
    {
      AdjOfSwitches[i] = malloc( sizeof *AdjOfSwitches[i] * numOfPorts );
    }

    echoIntegratedPorts(IntegratedPortOfSwitches, numOfPorts, numOfSwitches);
    //int buffers[numOfSwitches * numOfPorts][numOfInforAboutSwitch];
    Packet hosts[numOfHosts];

    assignAdj(AdjOfSwitches, 10, 11);

    for(int i = 0; i < numOfHosts; i++)
    {   
        if(i == 32 - numOfSwitches)
            hosts[i] = createPacket(i + numOfSwitches, i + numOfSwitches, 21, 0);
        else
            hosts[i] = //i + numOfSwitches;  
                    createPacket(i + numOfSwitches, -1, -1, 0);
    }

    //display(hosts, numOfHosts);

    //echo(AdjOfSwitches, numOfPorts, numOfSwitches);
    //printf("%d", switches[0][1]);

    //printf("\nHello World\n");
    return 0;
}
