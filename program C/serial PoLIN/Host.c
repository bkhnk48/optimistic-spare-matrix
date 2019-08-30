#include "Host.h"

int CanReceive(Host host, Switch *switches)
{
    IntegratedPort ports = getOutPort(host, switches);
    return ports->bufferIn;
}

IntegratedPort getOutPort(Host host, Switch *switches)
{
    int switchID = host->aSwitch;
    Switch theSwitch = switches[switchID];
    int t = host->outPort;
    IntegratedPort ports = (theSwitch->integratedPorts[t]);
    return ports;
}

//Packet 
int dequeue(Host host, Packet* first)
{
    if(host -> queue != NULL)
    {
        *first = (host->queue);
        host->queue = (host->queue)->next;
        //return first;
        return 1;
    }
    //return NULL;
    return 0;
}