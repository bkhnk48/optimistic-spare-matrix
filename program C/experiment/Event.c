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

int actionB(BufferHost *bufferHost, int link){
    int packetID = -1;
    int generateEventC = 0;
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
    
    generateEventC = link == 0;
    return generateEventC;
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