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
    //int allIntegratedPort[2];
    //IntegratedPort allIntegratedPort[2];
    int idsOfNodes[2];//Quy uoc, phan tu thu 2 luon la ID cua SWITCH
    int idsOfIntegratedPorts[2];//Neu phan tu dau tien la HOST thi phan tu dau tien
                                //cua mang nay la mot so am???
    int isBusy[2];//luon la 0 hoac 1
};

typedef struct LINK *Link;

struct PairInOutPortsOfSwitch
{
    //unsigned char bufferIn;
    //unsigned char bufferOut;
    int creditCount;
    int destID; //ID of SWITCH or HOST
    int linkID;
    unsigned char swFlag;
    unsigned char stFlag;
    Packet inputPort[5];
    Packet outputPort[5];
    //Link link;
};

typedef struct PairInOutPortsOfSwitch *IntegratedPort; 

struct HOST{
    int ID;
    int aSwitch;
    //int outPort;
    int linkID;
    int lastID; //ID cua packet cuoi cung da tao ra, gia tri
                //nay bang -1 nghia la chua gui packet nao ca
    int front;
    int bufferSize;
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
    int DELAY;
    //int *link;//danh sach cac Link ket noi den Switch
    IntegratedPort *integratedPorts;//Cac IntegratedPort
    //int *link;//danh sach cac Link ket noi den Switch
    //Link *link;
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


/*Function to find maximum of x and y*/
int min(int x, int y) 
{ 
    return y ^ ((x ^ y) & -(x < y)); 
} 
  
    
int max(int x, int y) 
{ 
    return x ^ ((x ^ y) & -(x < y));  
} 