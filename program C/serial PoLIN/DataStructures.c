#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h> 
  
// A structure to represent a queue 
struct Queue 
{ 
    int src;
    int dst;
    int nHop;
    int id; 
    struct Queue *next; 
}; 

typedef struct Queue *Packet; //Define packet as pointer of data type struct Queue

Packet createPacket(int id, int src, int dst, int nHop){
    Packet temp; // declare a node
    temp = (Packet)malloc(sizeof(struct Queue)); // allocate memory using malloc()
    temp->id = id;
    temp->src = src;
    temp->dst = dst;
    temp->nHop = nHop;
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}