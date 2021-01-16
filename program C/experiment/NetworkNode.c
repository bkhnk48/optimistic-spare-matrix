#include <stdio.h>
#include <stdlib.h>
//#include "FatTreeGraph.c"

#ifndef _LIB_OF_NETWORK_NODE
#define _LIB_OF_NETWORK_NODE
#define BUFFER_SIZE 5
enum TypesOfNode
{
    HOST = 0, //host
    EDGE_SWITCH = 1, //pod switch
    AGG_SWITCH = 2,
    CORE_SWITCH = 3, //core switch
    ERROR = -1
};

enum StatesOfEXB{
    X01 = 1, //EXB is not full and able to transfer packet.
    X00 = 0, /*EXB is not full and unable to transfer packet
                 (due to the next ENB is full).*/
    X10 = 2, /*EXB is full and unable to transfer packet 
                 (due to the next ENB is full).*/
    X11 = 3  //EXB is full and able to transfer packet.
};

enum StatesOfPkt{
    P1 = 1, //the packet is generated.
    P2 = 2, //the packet is located at EXB of the source node.
    P3 = 3, //the packet is moved in a unidirectional way.
    P4 = 4, //the packet is located at ENB of switch.
    P4bis = 7, //the packet is choosen to move on the next EXB
    P5 = 5, //the packet is located at EXB of switch.
    P6 = 6  //the packet is received by the destination node.
};

typedef struct _packet{
    unsigned long id;
    int srcIP;
    int dstIP;
    int currIP;
    enum StatesOfPkt state;
}Packet;

typedef struct _link{
    Packet *pkt;
    int nextPort;
    //int currPort;
    int nextIndex;
    //int currIndex;
} Link;

typedef struct _networkNode{
  int indexInGroup;
  int indexInNodes;
  int ipv4;
  enum TypesOfNode type;
  Link *links;
} NetworkNode;

typedef struct _bufferHost{
    int indexInGroup;
    int countNextENB;
    //int indexInNodes;
    unsigned long firstSQ;
    unsigned long lastSQ;
    unsigned long firstEXB;  
    unsigned long lastEXB;
} BufferHost;

typedef struct _bufferSwitch{
    int indexInGroup;
    enum TypesOfNode type;
    //int indexInNodes;
    Packet **ENB;
    Packet **EXB;
    int *countNextENB;
    enum StatesOfEXB *stsEXB;
} BufferSwitch;

BufferHost *initBufferHosts(int numOfHosts){
    BufferHost *bufferHosts = (BufferHost *)malloc(numOfHosts
                                                *sizeof(BufferHost));
    int i; int pod;
    for(i = 0; i < numOfHosts; i++){
        bufferHosts[i].indexInGroup = i;
        bufferHosts[i].firstSQ = -1;
        bufferHosts[i].lastSQ = -1;
        bufferHosts[i].firstEXB = -1;
        bufferHosts[i].lastEXB = -1;
        bufferHosts[i].countNextENB = BUFFER_SIZE;
    }
    return bufferHosts;
}

BufferSwitch *initBufferSwitches(int numOfSwitches, int k){
    BufferSwitch *bufferSwitches = (BufferSwitch *)malloc(numOfSwitches
                                                *sizeof(BufferSwitch));
    int i, j, buff; int pod;
    for(i = 0; i < numOfSwitches; i++){
        bufferSwitches[i].indexInGroup = i;
        Packet **temp1 = NULL;
        temp1 = malloc(sizeof * temp1 * k);
        for(j = 0; j < k; j++){
            temp1[j] = malloc(sizeof * temp1[j] * BUFFER_SIZE);
            for(buff = 0; buff < BUFFER_SIZE; buff++){
                temp1[j][buff].id = -1;
                temp1[j][buff].srcIP = -1;
                temp1[j][buff].dstIP = -1;
                temp1[j][buff].currIP = -1;
            }
        }
        bufferSwitches[i].ENB = temp1;
        Packet **temp2 = NULL;
        temp2 = malloc(sizeof * temp2 * k);
        for(j = 0; j < k; j++){
            temp2[j] = malloc(sizeof * temp2[j] * BUFFER_SIZE);
            for(buff = 0; buff < BUFFER_SIZE; buff++){
                temp2[j][buff].id = -1;
                temp2[j][buff].srcIP = -1;
                temp2[j][buff].dstIP = -1;
                temp2[j][buff].currIP = -1;
            }
        }
        bufferSwitches[i].EXB = temp2;

        bufferSwitches[i].countNextENB = malloc(k*sizeof(int));
        for(j = 0; j < k; j++){
            bufferSwitches[i].countNextENB[j] = BUFFER_SIZE;
        }
        bufferSwitches[i].stsEXB = malloc(k*sizeof(int));
        for(j = 0; j < k; j++){
            bufferSwitches[i].stsEXB[j] = X01;
        }
    }
    return bufferSwitches;
}



#endif