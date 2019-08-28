#include<stdio.h>
#include "DataStructures.c"

int CanReceive(Host host, Switch *switches)
{
    int switchID = host->aSwitch;
    Switch theSwitch = switches[switchID];
    int i = 0;
    int count = 0;
    IntegratedPort ports = (IntegratedPort)(theSwitch->integratedPorts[host->outPort]);
    //Packet outputPort[5] ;
    //outputPort = ports->outputPort;
    if(ports->outputPort == NULL)
    {
        return 0;
    }
    for(i = 0; i < 5; i++)
    {
        if(ports->outputPort[i] == NULL)
            count++;
    }
    switch (count)
    {
        case 0: return 0;       break;
        default:    return 1;   break;
    }
}