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
#endif