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
    int lastID; //ID cua packet cuoi cung da tao ra, gia tri
                //nay bang -1 nghia la chua gui packet nao ca
    int front;
    Packet queue;//Source queue
    Packet last;//phan tu cuoi cung trong queue
    
};


typedef struct HOST *Host;

/* struct LINK{
    double length;
};*/

struct PairInOutPortsOfSwitch
{
    unsigned char bufferIn;
    unsigned char bufferOut;
    int destID; //ID of SWITCH or HOST
    int linkID;
    unsigned char swFlag;
    Packet inputPort[5];
    Packet outputPort[5];
};

typedef struct PairInOutPortsOfSwitch *IntegratedPort; 


struct SWITCH{
    int host;//0 nghia la khong ket noi voi host nao ca
    IntegratedPort *integratedPorts;//Cac IntegratedPort
    //int *link;//danh sach cac Link ket noi den Switch
    int DELAY;
};

typedef struct SWITCH *Switch;

struct SIMULATOR{
    int currentTime;
};

typedef struct SIMULATOR *DiscreteEventSimulator;

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