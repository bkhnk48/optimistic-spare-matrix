#include <stdio.h>
#include "Init.c"



int main(int argc, char** argv) 
{
    int numOfPorts = 4;
    int numOfSwitches = 20;
    int numOfHosts = 16;
    int numOfInforAboutSwitch = 12;
    //int switches[numOfSwitches][numOfPorts];
    int **switches = NULL;
    switches = malloc( sizeof * switches * numOfSwitches );
    for (int i = 0; i < numOfSwitches; i++ )
    {
      switches[i] = malloc( sizeof *switches[i] * numOfPorts );
    }
    int buffers[numOfSwitches * numOfPorts][numOfInforAboutSwitch];
    Packet hosts[numOfHosts];

    assignAdj(switches, 10, 11);

    for(int i = 0; i < numOfHosts; i++)
    {   
        if(i == 32 - numOfSwitches)
            hosts[i] = createPacket(i + numOfSwitches, i + numOfSwitches, 21, 0);
        else
            hosts[i] = //i + numOfSwitches;  
                    createPacket(i + numOfSwitches, -1, -1, 0);
    }

    //display(hosts, numOfHosts);

    echo(switches, numOfPorts, numOfSwitches);
    //printf("%d", switches[0][1]);

    printf("\nHello World\n");
    return 0;
}

