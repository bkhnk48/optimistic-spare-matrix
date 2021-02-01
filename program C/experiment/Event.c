#include <stdio.h>
#include <stdlib.h>
#include "NetworkNode.c"
#include "ShiftBits.c"

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
                StoredPacket *EXB,
                enum StatesOfEXB *stateEXB,
                int last,
                unsigned long currentTime
                );

void changeForRemove(int *firstLastBuffer);

void changeForInsert(int *firstLastBuffer);

int countEmptySlots(int first, int last);

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
                int dstIP, int T
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
    link[0].pkt->generatedTime = (unsigned long)packetID*T;
    //Khong on neu viec sinh goi tin khong theo chu ky
    link[0].pkt->dstIP = dstIP;
    link[0].pkt->state = P3;//the packet is moved in a unidirectional way.

    bufferHost->countNextENB--;

    if(isFull && bufferHost->firstSQ != -1){
        *generateEventB = 1;
    }
    return 1;
}

int receivePacket(/*enum StatesOfENB *stateENB, 
                    Packet *ENB, 
                    unsigned long *requestedTime,*/
                    int portID,
                    BufferSwitch *bufferSwitch,
                    unsigned long currentTime,
                    Packet *pkt){
    /* This func returns 1 means the event E will be generated
     * otherwise, no new event E will occur.
    */

    int i = -1;
    int first = bufferSwitch->firstLastENBs[portID][0];
    int last = bufferSwitch->firstLastENBs[portID][1];
    if(last == -1){
        i = 0;
    }
    else{
        if(first != (last + 1) % BUFFER_SIZE){
            i = (last + 1) % BUFFER_SIZE;
        }
    }
    /*for(i = 0; i < BUFFER_SIZE; i++){
        if(ENB[i].id == -1)
            break;//found empty slot in ENB
    }*/
    //if(i < BUFFER_SIZE - 1){
    if(i != -1){
        bufferSwitch->ENB[portID][i].id = pkt->id;
        pkt->id = -1;
        bufferSwitch->ENB[portID][i].srcIP = pkt->srcIP;
        
        bufferSwitch->ENB[portID][i].dstIP = pkt->dstIP;
        bufferSwitch->ENB[portID][i].state = P4;
        pkt->srcIP = -1;
        pkt->dstIP = -1;
        pkt->currIP = -1;
        pkt->state = P_NULL;
        if(i == first){
            bufferSwitch->requestedTimeOfENB[portID] = currentTime;
        }
        bufferSwitch->firstLastENBs[portID][1] = i;
    }
    if((i + 1) % BUFFER_SIZE == first)
        bufferSwitch->stsENBs[portID] = N1;
    return i;
}

int actionD(int portENB, //int *generateEventE,
                StoredPacket *EXB,
                enum StatesOfEXB *stateEXB,
                int last,
                unsigned long currentTime
                ){
    
    int generateEventE = 0;
    int i = 0;
    int couldSendPacket = 0;
    //int first = firstLastEXB[0];
    //int last = firstLastEXB[1];
    if(*stateEXB == X01 || *stateEXB == X00){
        //EXB is not full
        if(last == -1)//empty EXB
            i = 0;
        else{
            i = (last + 1) % BUFFER_SIZE;
        }
        /*for(i = 0; i < BUFFER_SIZE; i++){*/
        if(EXB[i].srcIP == -1 && 
                EXB[i].dstIP == -1
                //&& EXB[i].id == -1
                ){
            couldSendPacket = 1;
            //found empty slot in EXB
        }
        /*}*/
        if(couldSendPacket){
            //int j = i - 1;//Kiem tra goi tin o truoc o trong
            if(EXB[i].id == -1){//o thu (i) cua EXB thuc su trong
                generateEventE = 1;//tao event E
            }
            else {//neu o thu (i) do dang danh cho 1 packet nao do
                
                if((EXB[i].requestedTime < currentTime 
                    && EXB[i].requestedTime == 0//Gia tri 0 nay de reset, bao rang EXB nay chua co
                    //nhan duoc yeu cau gui goi tin tu ENB sang
                    ) || EXB[i].requestedTime == currentTime 
                    ){
                    generateEventE = (EXB[i].requestedTime != currentTime);
                        //(BUFFER_SIZE - 1 >= i + getCount(EXB[i].id) + 1);
                    couldSendPacket = 1;
                }
                else{
                    //it means
                    //EXB[j].requestedTime > currentTime
                    generateEventE = 0;
                    couldSendPacket = 0;
                    //i = j;//backward to the previous index j
                }
                //}
            }
            if(generateEventE || couldSendPacket){
                //EXB[i].id = portENB;
                EXB[i].id = increase(EXB[i].id, portENB);
                if(EXB[i].srcIP != -1){
                    EXB[i].srcIP = -1;
                    EXB[i].dstIP = -1;
                }
                if(EXB[i].state != P_NULL)
                    EXB[i].state = P_NULL;
                if(EXB[i].requestedTime != currentTime)
                    EXB[i].requestedTime = currentTime;
            }
            
        }
        else
            *stateEXB = (*stateEXB == X01) ? X11 : X10;
    }
    return generateEventE;
}


int actionE(int portENB, int portEXB, BufferSwitch *bufferSwitch){
    int result = 0;
    /*
     *The variable result will contain several bit to represent:
     *(i)   generateEventE at bit 0
     *(ii)  generateEventF at bit 1
    */
    int firstEXB = bufferSwitch->firstLastEXBs[portEXB][0];
    int lastEXB = bufferSwitch->firstLastEXBs[portEXB][1];

    int firstENB = bufferSwitch->firstLastENBs[portENB][0];

    lastEXB = (lastEXB + 1) % BUFFER_SIZE;
    unsigned long temp = bufferSwitch->EXB[portEXB][lastEXB].id;
    unsigned long requestedTime = bufferSwitch->EXB[portEXB][lastEXB].requestedTime;
    int numOngoingE = getCount(temp);//number of ongoing event E
    int min = getMin(temp);
    int max = getMax(temp);

    bufferSwitch->EXB[portEXB][lastEXB].id = bufferSwitch->ENB[portENB][firstENB].id;
    bufferSwitch->ENB[portENB][firstENB].id = -1;
    bufferSwitch->EXB[portEXB][lastEXB].srcIP = 
                    bufferSwitch->ENB[portENB][firstENB].srcIP;
    bufferSwitch->ENB[portENB][firstENB].srcIP = -1;
    bufferSwitch->EXB[portEXB][lastEXB].dstIP = 
                    bufferSwitch->ENB[portENB][firstENB].dstIP;
    bufferSwitch->ENB[portENB][firstENB].dstIP = -1;
    bufferSwitch->EXB[portEXB][lastEXB].requestedTime 
                = bufferSwitch->ENB[portENB][firstENB].generatedTime;
    bufferSwitch->ENB[portENB][firstENB].generatedTime = -1;
    bufferSwitch->EXB[portEXB][lastEXB].state = P5;
    bufferSwitch->ENB[portENB][firstENB].state = P_NULL;

    if(numOngoingE > 1){
        if(countEmptySlots(firstEXB, lastEXB) > 0){
            result |= 1;
            int newLast = (lastEXB + 1) % BUFFER_SIZE;
        }
    }
    if(lastEXB == 0 && firstEXB == 0)
        result |= 2;//will generate event F
    changeForRemove(bufferSwitch->firstLastENBs[portENB]);
    changeForInsert(bufferSwitch->firstLastEXBs[portEXB]);
    return result;
}

void changeForRemove(int *firstLastBuffer){
    int first = firstLastBuffer[0];
    int last = firstLastBuffer[1];
    if(first == last){
        *firstLastBuffer = 0;
        firstLastBuffer[1] = -1;
    }
    else{
        first++;
        first %= BUFFER_SIZE;
        *firstLastBuffer = first;
    }
}

void changeForInsert(int *firstLastBuffer){
    firstLastBuffer[1] = (firstLastBuffer[1] + 1) % BUFFER_SIZE;
}

int countEmptySlots(int first, int last){
    if(first == 0 && last == -1)
        return BUFFER_SIZE;
    return (BUFFER_SIZE - (last - first + 1));
}

void signEXB_ID(int EXB_ID, int *registeredEXB){
    *registeredEXB = EXB_ID;
}

void signRequestedTime(unsigned long *requestedTime, 
                            unsigned long currentTime){
    *requestedTime = currentTime;
}

#endif