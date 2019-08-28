#include <stdio.h>
#include "Init.c"



int main(int argc, char** argv) 
{
    int numOfPorts = 4;
    int numOfSwitches = 20;
    int numOfHosts = 16;
    int numOfInforAboutSwitch = 12;

    int **AdjOfSwitches = NULL;//Mang luu tru cac Adjancent cua SWiTCHES, tam thoi de lai bien nay
    //vi chua tao ra duoc ham doc file
    Switch *switches = NULL;//Mang luu tru cac switches cua network
    Host *hosts = NULL;

    IntegratedPort **IntegratedPortOfSwitches = NULL;//Mang luu tru cac Integrated Port cua SWITCHES
    AdjOfSwitches = malloc( sizeof * AdjOfSwitches * numOfSwitches );

    IntegratedPortOfSwitches = malloc( sizeof * IntegratedPortOfSwitches * numOfSwitches );
    switches = malloc(sizeof * switches * numOfSwitches);
    hosts = malloc(sizeof * hosts * numOfHosts);

    for (int i = 0; i < numOfSwitches; i++ )
    {
      IntegratedPortOfSwitches[i] = malloc( sizeof *IntegratedPortOfSwitches[i] * numOfPorts );
    }

    for (int i = 0; i < numOfHosts; i++ )
    {
      Host tempHost = malloc(sizeof tempHost);
      hosts[i] = tempHost;
    }

    for (int i = 0; i < numOfSwitches; i++ )
    {
      IntegratedPort *integratedPorts = malloc(sizeof * integratedPorts * numOfPorts);
      Switch aSwitch = malloc(sizeof aSwitch);
      aSwitch-> host = 0;
      switches[i] = aSwitch;
      switches[i] -> integratedPorts = integratedPorts;
      AdjOfSwitches[i] = malloc( sizeof *AdjOfSwitches[i] * numOfPorts );
    }

    //Packet *hosts = NULL; //Source Queue cua cac Hosts
    //hosts = malloc(numOfHosts * sizeof(Packet));
    
    Packet *lasts = NULL;//Luu tru phan tu cuoi cung trong Source Queue cua cac Hosts
    lasts = malloc(numOfHosts * sizeof(Packet));

    int *lastIDs = NULL;
    lastIDs = calloc(numOfHosts, sizeof(int));
    
    assignAdj(AdjOfSwitches, 10, 11);
    assignAdjant(switches, hosts, AdjOfSwitches, numOfSwitches, numOfPorts);

    //InitIntegratedPorts(IntegratedPortOfSwitches, numOfPorts, numOfSwitches);


    //printf("\nLAST ID %d", lastIDs[32 - numOfSwitches]);
    //insertToSourceQueue(hosts, lasts, 32, 21, numOfSwitches, lastIDs);
    
    insertSourceQueue(hosts, 32, 21, numOfSwitches);

    //display(hosts, numOfHosts);

    //printf("\n%d", lastIDs[32 - numOfSwitches]);
    printf("\n====================\n");
    insertSourceQueue(hosts, 32, 21, numOfSwitches);
    //display(hosts, numOfHosts);
    //printf("\n%d", lastIDs[32 - numOfSwitches]);

    return 0;
}

