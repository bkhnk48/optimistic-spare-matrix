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
    Link *links = NULL;
    int *linkID = NULL;

    //IntegratedPort **IntegratedPortOfSwitches = NULL;//Mang luu tru cac Integrated Port cua SWITCHES
    AdjOfSwitches = malloc( sizeof * AdjOfSwitches * numOfSwitches );
    linkID = malloc(sizeof * linkID * numOfSwitches);

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
      hosts[i] -> linkID = 0;
      hosts[i]->bufferSize = 0;
      IntegratedPort integratedPorts = malloc(sizeof * integratedPorts);
      (hosts[i] -> outPort) = integratedPorts;
      (hosts[i] -> outPort) -> swFlag = 0;
      (hosts[i] -> outPort) -> stFlag = 0;
      
      hosts[i] -> queue = NULL;
      hosts[i] -> last = NULL;
    }

    for (int i = 0; i < numOfSwitches; i++ )
    {
      IntegratedPort *integratedPorts = malloc(sizeof * integratedPorts * numOfPorts);
      //Link *link = malloc(sizeof * link * numOfPorts);
      //int *linkIDOfSwitch = malloc(sizeof * linkIDOfSwitch * numOfPorts);
      Switch aSwitch = malloc(sizeof aSwitch);
      aSwitch-> host = 0;
      //aSwitch-> link = linkIDOfSwitch;
      switches[i] = aSwitch;
      switches[i] -> ID = i;
      switches[i] -> integratedPorts = integratedPorts;
      //switches[i] -> link = link;//This statement will cause "malloc(): corrupted top size"
      AdjOfSwitches[i] = malloc( sizeof *AdjOfSwitches[i] * numOfPorts );
    }


    int count = assignAdj(AdjOfSwitches, linkID, 4, 20);
    //int x = (21 - 17)>>(sizeof(int)*8 - 1);
    //printf("\n\t\t x = %d", -x);
    //x = (11 - 17)>>(sizeof(int)*8 - 1);
    //printf("\n\t\t x = %d", -x);

    links = malloc(sizeof * links * count);

    //int smaller = min(17, 21);
    //printf("\n MIN: %d", smaller);
    //int bigger = max(17, 21);
    //printf("\n MAX: %d", bigger);

    assignAdjant(switches, hosts, AdjOfSwitches, /*links,*/ numOfSwitches, numOfPorts);

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

