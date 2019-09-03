#include "Host.h"

/* int CanReceive(Host host, Switch *switches)
{
    IntegratedPort ports = getOutPort(host, switches);
    return ports->bufferIn;
}*/

IntegratedPort getOutPort(Host host, Switch *switches)
{
    /*int switchID = host->aSwitch;
    Switch theSwitch = switches[switchID];
    int t = host->outPort;
    IntegratedPort ports = (theSwitch->integratedPorts[t]);
    return ports;*/
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

int getEmptySlot(Packet ports[])
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
    unsigned int slot4 = (unsigned int)(ports[4]);
    unsigned int slot3 = (unsigned int)(ports[3]);
    unsigned int slot2 = (unsigned int)(ports[2]);
    unsigned int slot1 = (unsigned int)(ports[1]);
    unsigned int slot0 = (unsigned int)(ports[0]);
    #pragma GCC diagnostic pop
    switch (slot0)
    {   case 0: return 0;  break;     }
    switch (slot1)
    {   case 0: return 1;  break;     }
    switch (slot2)
    {   case 0: return 2;  break;     }
    switch (slot3)
    {   case 0: return 3;  break;     }
    switch (slot4)
    {   case 0: return 4;  break;     }
    return -1;
}

void forwardToLink(Host self)
{
    int emptySlots = getEmptySlot((self->outPort)->outputPort);
    //neu emptySlots = 0 nghia la buffer cua outputPort trong hoan toan
    int creditCount = (self->outPort)->creditCount;
    //creditCount > 0 nghia la noi nhan packet van con cho trong
    
}