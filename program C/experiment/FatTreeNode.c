#include <stdio.h>
#include <stdlib.h>
#include "FatTreeGraph.c"

#ifndef _LIB_OF_FATTREE_NODE
#define _LIB_OF_FATTREE_NODE
#define BUFFER_SIZE 5
typedef struct _networkNode{
  int indexInGroup;
  int indexInNodes;
  int ipv4;
  enum TypesOfNode type;
} NetworkNode;

typedef struct _bufferHost{
    int indexInGroup;
    int indexInNodes;
    unsigned long firstSQ;
    unsigned long lastSQ;
    unsigned long firstEXB;  
    unsigned long lastEXB;
} BufferHost;

typedef struct _packet{
    unsigned long id;
    int srcIP;
    int dstIP;
    int currIP;
}Packet;

typedef struct _bufferSwitch{
    int indexInGroup;
    int indexInNodes;
    Packet **ENB;
    Packet **EXB;
} BufferSwitch;

BufferHost *initBufferHosts(int k){
    BufferHost *bufferHosts = (BufferHost *)malloc((k*k*k/4)
                                                *sizeof(BufferHost));
    int i; int pod;
    for(i = 0; i < k*k*k/4; i++){
        bufferHosts->indexInGroup = i;
        pod = i / (k*k/4);
        bufferHosts->indexInNodes = i % (k*k/4) + pod*((k*k/4) + k);
        bufferHosts->firstSQ = -1;
        bufferHosts->lastSQ = -1;
        bufferHosts->firstEXB = -1;
        bufferHosts->lastEXB = -1;
    }
    return bufferHosts;
}

BufferSwitch *initBufferSwitches(int k){
    BufferSwitch *bufferSwitches = (BufferSwitch *)malloc((k*k*5/4)
                                                *sizeof(BufferSwitch));
    int i, j; int pod;
    for(i = 0; i < k*k*5/4; i++){
        bufferSwitches->indexInGroup = i;
        //pod = i / (k*k/4);
        //bufferSwitches->indexInNodes = i % (k*k/4) + pod*((k*k/4) + k);
        Packet **temp1 = NULL;
        temp1 = malloc(sizeof * temp1 * k);
        for(j = 0; j < k; j++){
            temp1[j] = malloc(sizeof * temp1[j] * BUFFER_SIZE);
            temp1[j]->id = -1;
            temp1[j]->srcIP = -1;
            temp1[j]->dstIP = -1;
            temp1[j]->currIP = -1;
        }
        bufferSwitches->ENB = temp1;
        Packet **temp2 = NULL;
        temp2 = malloc(sizeof * temp2 * k);
        for(j = 0; j < k; j++){
            temp2[j] = malloc(sizeof * temp2[j] * BUFFER_SIZE);
            temp2[j]->id = -1;
            temp2[j]->srcIP = -1;
            temp2[j]->dstIP = -1;
            temp2[j]->currIP = -1;
        }
        bufferSwitches->EXB = temp2;
    }
    return bufferSwitches;
}

#endif