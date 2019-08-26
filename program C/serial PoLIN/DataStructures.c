#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h> 

#include "ErrorReport.c"
  
// A structure to represent a queue 
struct PACKET 
{ 
    int src;
    int dst;
    int nHop;
    int id; 
    struct PACKET *next; 
}; 

typedef struct PACKET *Packet; //Define packet as pointer of data type struct Queue

struct HOST{
    int aSwitch;
    int outPort;
    int lastID; //ID cua packet cuoi cung da tao ra
    Packet *queue;//Source queue
    Packet *last;//phan tu cuoi cung trong queue
};



typedef struct HOST *host;


struct PairPortsOfSwitch
{
    Packet inputPort[5];
    Packet outputPort[5];
};

typedef struct PairPortsOfSwitch *IntegratedPort; 


struct SWITCH{
    int host;//-1 nghia la khong ket noi voi host nao ca
    IntegratedPort *integratedPorts;
};


Packet createPacket(int id, int src, int dst, int nHop){
    Packet temp; // declare a node
    temp = (Packet)malloc(sizeof(struct PACKET)); // allocate memory using malloc()
    temp->id = id;
    temp->src = src;
    temp->dst = dst;
    temp->nHop = nHop;
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}