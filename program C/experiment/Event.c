#include <stdio.h>
#include <stdlib.h>
#include "NetworkNode.c"

#ifndef _TYPES_OF_EVENT_
#define _TYPES_OF_EVENT_
enum TypesOfEvent
{
    A = 0, //GenerationEvent
    B = 1, //LeavingSourceQueueEvent
    C = 2, //LeavingEXBEvent
    H_HOST = 3, //NotificationEvent at hosts
    D = 5, //ReachingENBEvent
    E = 6, //MovingInSwitchEvent
    F = 7, //LeavingSwitchEvent
    G = 4, //ReachingDestinationEvent
    H = 8, //NotificationEvent
    X = 9  //RandomEvent
};

enum Side{LEFT, RIGHT};

int actionD(int portENB, //int *generateEventE,
                Packet *EXB,
                enum StatesOfEXB stateEXB
                );

int actionA(int T, 
                unsigned long currentTime,
                BufferHost* bufferHost
                ){
    //Cap nhat source queue
    if(bufferHost->firstSQ == -1){
        bufferHost->firstSQ = currentTime/T;
    }
    else{
        bufferHost->lastSQ = currentTime/T;
    }

    //Kiem tra EXB cua host
    int generateEventB = 0;
    if(bufferHost->firstEXB == -1 || (bufferHost->lastEXB == -1)){
        generateEventB = 1;
    }
    else {
        generateEventB = 
            (bufferHost->lastEXB - bufferHost->firstEXB + 1) 
                    < BUFFER_SIZE;
    }
    return generateEventB;
}

int actionB(BufferHost *bufferHost, Packet *pktInLink//, Packet *connectedENB
                ){
    int packetID = -1;
    int generateEventC = 0;

    #pragma region modify source queue and EXB of HOST
    if(bufferHost->firstSQ != -1){
        if(bufferHost->lastSQ == -1)
        {
            packetID = bufferHost->firstSQ;
            bufferHost->firstSQ = -1;
        }
        else{
            packetID = bufferHost->lastSQ;
            if(bufferHost->lastSQ == bufferHost->firstSQ + 1){
                bufferHost->lastSQ = -1;
            }
            else{
                bufferHost->lastSQ--;
            }
        }
    }

    if(bufferHost->firstEXB == -1){
        bufferHost->firstEXB = packetID;
    }
    else{
        bufferHost->lastEXB = packetID;
    }
    #pragma endregion 
    
    generateEventC = (pktInLink->id == -1);
    if(generateEventC){
        //Check buffer of next switch
        generateEventC = //(connectedENB[BUFFER_SIZE - 1].id == -1);
                        bufferHost->countNextENB > 0;
    }
    return generateEventC;
}

int actionC(BufferHost *bufferHost, Link *link, int *generateEventB,
                int dstIP
                ){
    int packetID = -1;
    int isFull = 0; 

    if(bufferHost->firstEXB != -1){
        if(bufferHost->lastEXB == -1)
        {
            packetID = bufferHost->firstEXB;
            bufferHost->firstEXB = -1;
        }
        else{
            isFull = (bufferHost->lastEXB - bufferHost->firstEXB + 1);
            packetID = bufferHost->lastEXB;
            if(bufferHost->lastEXB == bufferHost->firstEXB + 1){
                bufferHost->lastEXB = -1;
            }
            else{
                bufferHost->lastEXB--;
            }
        }
    }

    if(packetID == -1)
    {
        *generateEventB = 0;
        return 0;
    }
    
    link[0].pkt->id = packetID;
    link[0].pkt->dstIP = dstIP;
    link[0].pkt->state = P3;//the packet is moved in a unidirectional way.

    bufferHost->countNextENB--;

    if(isFull && bufferHost->firstSQ != -1){
        *generateEventB = 1;
    }
    return 1;
}

int receivePacket(enum StatesOfENB *stateENB, 
                    Packet *ENB, 
                Packet *pkt){
    /* This func returns 1 means the event E will be generated
     * otherwise, no new event E will occur.
    */
    int i = 0;
    for(i = 0; i < BUFFER_SIZE; i++){
        if(ENB[i].id == -1)
            break;//found empty slot in ENB
    }
    if(i < BUFFER_SIZE - 1){
        ENB[i].id = pkt->id;
        pkt->id = -1;
        ENB[i].srcIP = pkt->srcIP;
        
        ENB[i].dstIP = pkt->dstIP;
        ENB[i].state = P4;
        pkt->srcIP = -1;
        pkt->dstIP = -1;
        pkt->currIP = -1;
        pkt->state = P_NULL;
    }
    if(i == BUFFER_SIZE - 1)
        *stateENB = N1;
    return i;
}

int actionD(int portENB, //int *generateEventE,
                Packet *EXB,
                enum StatesOfEXB stateEXB
                ){
    
    int generateEventE = 0;
    int i = 0;
    if(stateEXB == X01 || stateEXB == X00){
        //EXB is not full
        for(i = 0; i < BUFFER_SIZE; i++){
            if(EXB[i].srcIP == -1 && 
                   EXB[i].dstIP == -1)
                break;//found empty slot in EXB
        }
        EXB[i].id = portENB;
        EXB[i].srcIP = -1;
        EXB[i].dstIP = -1;
        EXB[i].state = P_NULL;
        generateEventE = 1;
    }
    return generateEventE;
}

/*void loadArray(int a[1000]){
    printf("fsdfsdf");
   int num;
   int tmp;
   FILE *fptr;

   if ((fptr = fopen("random1000.txt","r")) == NULL){
       printf("\nError! opening file\n");
       exit(1);
       return;
   }
   int i = 0;
   while(!feof(fptr)){
    tmp = fscanf(fptr,"%d", &num);
    a[i] = num;
    i++;
   }

   fclose(fptr);
}*/
#endif