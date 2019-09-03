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
    Link *link = NULL;

    //IntegratedPort **IntegratedPortOfSwitches = NULL;//Mang luu tru cac Integrated Port cua SWITCHES
    AdjOfSwitches = malloc( sizeof * AdjOfSwitches * numOfSwitches );

    //IntegratedPortOfSwitches = malloc( sizeof * IntegratedPortOfSwitches * numOfSwitches );
    switches = malloc(sizeof * switches * numOfSwitches);
    hosts = malloc(sizeof * hosts * numOfHosts);


    for (int i = 0; i < numOfSwitches; i++ )
    {
      //IntegratedPortOfSwitches[i] = malloc( sizeof *IntegratedPortOfSwitches[i] * numOfPorts );
    }


    for (int i = 0; i < numOfHosts; i++ )
    {
      Host tempHost = malloc(sizeof tempHost);
      
      hosts[i] = tempHost;
      hosts[i] -> ID = i + numOfSwitches;
      hosts[i] -> lastID = -1;//nghia la chua gui packet nao ca
      hosts[i] -> front = -1;
      IntegratedPort integratedPorts = malloc(sizeof * integratedPorts);
      (hosts[i] -> outPort) = integratedPorts;
      (hosts[i] -> outPort) -> swFlag = 0;
      (hosts[i] -> outPort) -> stFlag = 0;
      /*hosts[i] -> outputPort[0] = NULL;
      hosts[i] -> outputPort[1] = NULL;
      hosts[i] -> outputPort[2] = NULL;
      hosts[i] -> outputPort[3] = NULL;
      hosts[i] -> outputPort[4] = NULL;*/
      hosts[i] -> queue = NULL;
      hosts[i] -> last = NULL;
    }

    for (int i = 0; i < numOfSwitches; i++ )
    {
      IntegratedPort *integratedPorts = malloc(sizeof * integratedPorts * numOfPorts);
      Switch aSwitch = malloc(sizeof aSwitch);
      aSwitch-> host = 0;
      switches[i] = aSwitch;
      switches[i] -> ID = i;
      switches[i] -> integratedPorts = integratedPorts;
      AdjOfSwitches[i] = malloc( sizeof *AdjOfSwitches[i] * numOfPorts );
    }

    assignAdj(AdjOfSwitches, 10, 11);

    assignAdjant(switches, hosts, AdjOfSwitches, numOfSwitches, numOfPorts);

    //InitIntegratedPorts(IntegratedPortOfSwitches, numOfPorts, numOfSwitches);

    
    insertSourceQueue(hosts, 32, 21, numOfSwitches);

    display(hosts, numOfHosts);

    printf("\n====================\n");
    insertSourceQueue(hosts, 32, 21, numOfSwitches);
    display(hosts, numOfHosts);

    //printf("%d", hosts[12]->aSwitch);

    sendToOutPort(hosts[12], switches);

    Packet p = //(switches[12]->integratedPorts[0])->inputPort[0];
                (hosts[12]-> outPort)->outputPort[0];

    printf("\n Packet:  %d from %d to %d\n",p->id, p->src, p->dst);

    return 0;
}

