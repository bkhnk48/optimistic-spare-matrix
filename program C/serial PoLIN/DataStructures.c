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

struct LINK{
    long bandwidth;
    double length;
    int allIntegratedPort[2];
    int isBusy[2];
};

typedef struct LINK *Link;

struct PairInOutPortsOfSwitch
{
    //unsigned char bufferIn;
    //unsigned char bufferOut;
    int creditCount;
    int destID; //ID of SWITCH or HOST
    //int linkID;
    unsigned char swFlag;
    unsigned char stFlag;
    Packet inputPort[5];
    Packet outputPort[5];
    Link link;
};

typedef struct PairInOutPortsOfSwitch *IntegratedPort; 

struct HOST{
    int ID;
    int aSwitch;
    //int outPort;
    int lastID; //ID cua packet cuoi cung da tao ra, gia tri
                //nay bang -1 nghia la chua gui packet nao ca
    int front;
    //unsigned char swFlag;
    //unsigned char stFlag;
    IntegratedPort outPort;
    Packet queue;//Source queue
    Packet last;//phan tu cuoi cung trong queue

    //Packet outputPort[5];
    
};


typedef struct HOST *Host;

struct SWITCH{
    int ID;
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