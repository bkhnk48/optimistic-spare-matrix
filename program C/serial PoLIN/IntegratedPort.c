#include "IntegratedPort.h"

void addPacketToBuffer(Packet p, IntegratedPort port)
{

}

int getEmptySlot(IntegratedPort port)
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
    unsigned int slot4 = (unsigned int)(port->inputPort[4]);
    unsigned int slot3 = (unsigned int)(port->inputPort[3]);
    unsigned int slot2 = (unsigned int)(port->inputPort[2]);
    unsigned int slot1 = (unsigned int)(port->inputPort[1]);
    unsigned int slot0 = (unsigned int)(port->inputPort[0]);
    #pragma GCC diagnostic pop
    int slot = -1;
    switch (slot4)
    {
        case 0: slot = 4;
        break;
    }
    return 0;
}