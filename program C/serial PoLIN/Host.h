#include<stdio.h>
#include "DataStructures.c"

int CanReceive(Host host, Switch *switches);

IntegratedPort getOutPort(Host host, Switch *switches);

//Packet 
int dequeue(Host host, Packet *first);
//int dequeue(Host host, Packet** first);