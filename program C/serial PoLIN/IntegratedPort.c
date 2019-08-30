#include "IntegratedPort.h"

void addPacketToBuffer(Packet p, IntegratedPort ports)
{
    int index = getEmptySlot(ports);
    ports->inputPort[index] = p;
}

int getEmptySlot(IntegratedPort ports)
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
    unsigned int slot4 = (unsigned int)(ports->inputPort[4]);
    unsigned int slot3 = (unsigned int)(ports->inputPort[3]);
    unsigned int slot2 = (unsigned int)(ports->inputPort[2]);
    unsigned int slot1 = (unsigned int)(ports->inputPort[1]);
    unsigned int slot0 = (unsigned int)(ports->inputPort[0]);
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