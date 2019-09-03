#include "IntegratedPort.h"

void addPacketToBuffer(Packet p, Host host)
{
    IntegratedPort outPort = host->outPort;
    int index = getEmptySlot(outPort->outputPort);
    outPort -> outputPort[index] = p;
}

