#include <stdio.h>
#include <stdlib.h>
#include "NetworkNode.c"
#include "ShiftBits.c"
#include "Throughput.c"

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

unsigned long Count[11] = {0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0};

unsigned long Events[49983] = {0};

enum Side{LEFT, RIGHT};

int actionD(int portENB, //int *generateEventE,
                int portEXB,
                //StoredPacket *EXB,
                //enum StatesOfEXB *stateEXB,
                BufferSwitch *bufferSwitch,
                //int last,
                unsigned long currentTime
                );

void changeForRemove(int *firstLastBuffer);

void changeForInsert(int *firstLastBuffer);

int countEmptySlots(int first, int last);

void update(int portENB, int portEXB, 
                    unsigned long temp, 
                    BufferSwitch *bufferSwitch);

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
            packetID = bufferHost->firstSQ;
            if(bufferHost->lastSQ == bufferHost->firstSQ + 1){
                bufferHost->lastSQ = -1;
                bufferHost->firstSQ++; 
            }
            else{
                bufferHost->firstSQ++;
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
    
    generateEventC = (pktInLink->id == -1) &&
        //Check buffer of next switch
                        (bufferHost->countNextENB > 0);
    
    return generateEventC;
}

int actionC(BufferHost *bufferHost, Link *link, 
                int *generateEventB,
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
            isFull = ((bufferHost->lastEXB - bufferHost->firstEXB + 1) == BUFFER_SIZE);
            packetID = bufferHost->firstEXB;
            if(bufferHost->lastEXB == bufferHost->firstEXB + 1){
                bufferHost->firstEXB += 1;
                bufferHost->lastEXB = -1;
            }
            else{
                bufferHost->firstEXB++;
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
                    //enum TypesOfNode typeOfNode,
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
        bufferSwitch->ENB[portID][i].generatedTime = pkt->generatedTime;
        //pkt->srcIP = -1;
        pkt->dstIP = -1;
        pkt->currIP = -1;
        pkt->state = P_NULL;
        pkt->generatedTime = -1;
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
                int portEXB,
                //StoredPacket *EXB,
                //enum StatesOfEXB *stateEXB,
                BufferSwitch *bufferSwitch,
                //int last,
                unsigned long currentTime
                ){
    
    int generateEventE = 0;
    int i = 0;
    int couldSendPacket = 0;
    //int first = firstLastEXB[0];
    int last = bufferSwitch->firstLastEXBs[portEXB][1];
    int first = bufferSwitch->firstLastEXBs[portEXB][0];
    //if(*stateEXB == X01 || *stateEXB == X00){
    if(//bufferSwitch->stsEXBs[portEXB] == X01 || 
       //     bufferSwitch->stsEXBs[portEXB] == X00)
        countEmptySlots(first, last) > 0
        )
       {
        //EXB is not full
        if(last == -1)//empty EXB
            i = 0;
        else{
            i = (last + 1) % BUFFER_SIZE;
        }
        if(
            //EXB[i].srcIP == -1 && 
            //    EXB[i].dstIP == -1
            bufferSwitch->EXB[portEXB][i].srcIP == -1 &&
                bufferSwitch->EXB[portEXB][i].dstIP == -1
                ){
            couldSendPacket = 1;
            //found empty slot in EXB
        }
        /*}*/
        if(couldSendPacket){
            //if(EXB[i].id == -1)
            if(bufferSwitch->EXB[portEXB][i].id == -1){
                //slot thu (i) cua EXB thuc su trong
                generateEventE = 1;//tao event E
            }
            else {//neu o thu (i) do dang danh cho 1 packet nao do
                
                if((bufferSwitch->requestedTimeToEXB[portEXB] < currentTime 
                    && bufferSwitch->requestedTimeToEXB[portEXB] == 0//Gia tri 0 nay de reset, bao rang EXB nay chua co
                    //nhan duoc yeu cau gui goi tin tu ENB sang
                    ) || bufferSwitch->requestedTimeToEXB[portEXB] == currentTime 
                    ){
                    generateEventE = (bufferSwitch->requestedTimeToEXB[portEXB] != currentTime);
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
                bufferSwitch->EXB[portEXB][i].id = increase(bufferSwitch->EXB[portEXB][i].id, portENB);
                if(bufferSwitch->EXB[portEXB][i].srcIP != -1){
                    bufferSwitch->EXB[portEXB][i].srcIP = -1;
                    bufferSwitch->EXB[portEXB][i].dstIP = -1;
                }
                if(bufferSwitch->EXB[portEXB][i].state != P_NULL)
                    bufferSwitch->EXB[portEXB][i].state = P_NULL;
                if(bufferSwitch->requestedTimeToEXB[portEXB] != currentTime)
                    bufferSwitch->requestedTimeToEXB[portEXB] = currentTime;
            }
            
        }
        else
            bufferSwitch->stsEXBs[portEXB] = (bufferSwitch->stsEXBs[portEXB] == X01) ? X11 : X10;
    }
    return generateEventE;
}


int move(int portENB, int portEXB, BufferSwitch *bufferSwitch){
    int needToFindNextPkt = 0;
    int firstEXB = bufferSwitch->firstLastEXBs[portEXB][0];
    int lastEXB = bufferSwitch->firstLastEXBs[portEXB][1];

    int firstENB = bufferSwitch->firstLastENBs[portENB][0];

    lastEXB = (lastEXB + 1) % BUFFER_SIZE;
    unsigned long temp = bufferSwitch->EXB[portEXB][lastEXB].id;
    unsigned long requestedTime = bufferSwitch->requestedTimeToEXB[portEXB];
    
    bufferSwitch->EXB[portEXB][lastEXB].id = bufferSwitch->ENB[portENB][firstENB].id;
    bufferSwitch->ENB[portENB][firstENB].id = -1;
    bufferSwitch->EXB[portEXB][lastEXB].srcIP = 
                    bufferSwitch->ENB[portENB][firstENB].srcIP;
    bufferSwitch->ENB[portENB][firstENB].srcIP = -1;
    bufferSwitch->EXB[portEXB][lastEXB].dstIP = 
                    bufferSwitch->ENB[portENB][firstENB].dstIP;
    bufferSwitch->ENB[portENB][firstENB].dstIP = -1;
    bufferSwitch->EXB[portEXB][lastEXB].generatedTime 
                = bufferSwitch->ENB[portENB][firstENB].generatedTime;
    bufferSwitch->ENB[portENB][firstENB].generatedTime = -1;
    bufferSwitch->EXB[portEXB][lastEXB].state = P5;
    bufferSwitch->ENB[portENB][firstENB].state = P_NULL;

    changeForRemove(bufferSwitch->firstLastENBs[portENB]);
    changeForInsert(bufferSwitch->firstLastEXBs[portEXB]);
    if(countEmptySlots(firstEXB, lastEXB) > 0){
    //It means EXB[portEXB] has at least one empty slot
        int count = getCount(temp) - 1;
        if(count > 0){
            update(portENB, portEXB, temp, bufferSwitch);
        }
        else if(count == 0){
            needToFindNextPkt = 1;
        }
    }
    return needToFindNextPkt;
}

void update(int portENB, int portEXB, 
                    unsigned long temp, BufferSwitch *bufferSwitch){
    int count = getCount(temp) - 1;
    int last = bufferSwitch->firstLastEXBs[portEXB][1];
    int newLast = 
            (bufferSwitch->firstLastEXBs[portEXB][1] + 1)
                             % BUFFER_SIZE;
    int min = getMin(temp);
    int max = getMax(temp);
    if(count == 1){
        if(min == portENB){
            min = max;
            max = 0;
        }
        else if(max == portENB){
            max = 0;
        }
    }
    else{
        //it means count > 1
        int i = 0;
        if(min == portENB){
            min = portENB + 1;
            for(i = min; i <= max; i++){
                if(bufferSwitch->requestedTimeOfENB[i] 
                    == bufferSwitch->requestedTimeToEXB[portEXB]
                && 
                    bufferSwitch->registeredEXBs[i]
                    == portEXB
                    )
                { 
                    min = i;
                    break;
                }
            }
        }
        else if(max == portENB){
            max = portENB - 1;
            for(i = max; i >= min; i--){
                if(bufferSwitch->requestedTimeOfENB[i] 
                    == bufferSwitch->requestedTimeToEXB[portEXB]
                && 
                    bufferSwitch->registeredEXBs[i]
                    == portEXB
                    )
                { 
                    max = i;
                    break;
                }
            }
        }
    }

    unsigned long result = 0;
    result |= (unsigned short)count;
    result |= (min << 16);
    result |= ((unsigned long)max << 32);
    bufferSwitch->EXB[portEXB][newLast].id = result;
}

int actionE(int portENB, int portEXB, 
                BufferSwitch *bufferSwitch, Link *link){
    int result = 0;
    /*
    *The variable result will contain several bit to represent:
    *(i)   generateEventE at bit 0
    *(ii)  generateEventF at bit 1
    */
    int firstEXB = bufferSwitch->firstLastEXBs[portEXB][0];
    int lastEXB = bufferSwitch->firstLastEXBs[portEXB][1];
    int nextLastEXB = (lastEXB + 1) % BUFFER_SIZE;

    int firstENB = bufferSwitch->firstLastENBs[portENB][0];

    unsigned long id = bufferSwitch->EXB[portEXB][nextLastEXB].id;
    if(id != -1){
        int count = getCount(id);
        int emptySlots = countEmptySlots(firstEXB, lastEXB);
        if(count >= 1 && emptySlots > 0){
            result |= 1;//will create event E
            //bufferSwitch->registeredEXBs[nextLastEXB] = 0;
            bufferSwitch->requestedTimeToEXB[portEXB] = 0;
        }
    }

    if((result & 1) == 0){
    //if there is no packet which has the same requested time with moved one
        //if(bufferSwitch->ENB[portENB][firstENB].srcIP != -1)
        {
        //if the previous ENB still has a ongoing packet 
           //bufferSwitch->registeredEXBs[nextLastEXB] = 0;
           bufferSwitch->requestedTimeToEXB[portEXB] = 0;
        }
    }

    if(bufferSwitch->countNextENB[portEXB] > 0
        && link->pkt->id == -1
        && bufferSwitch->EXB[portEXB][firstEXB].state == P5
        ){
        bufferSwitch->EXB[portEXB][firstEXB].state = P5bis;
        result |= 2;//will create event F
    }
    return result;
}

int actionF(BufferSwitch *bufferSwitch, 
                int portID, 
                Link *link, 
                int *generateEventE){
    int pktID = -1;
    int generateEventD = 0;
    int k = *generateEventE;
    *generateEventE = 0;
    int first = bufferSwitch->firstLastEXBs[portID][0];
    int last = bufferSwitch->firstLastEXBs[portID][1];
    int emptySlots = countEmptySlots(first, last);
    if(emptySlots < BUFFER_SIZE && link->pkt->id == -1){
        //StoredPacket pkt = bufferSwitch->EXB[portID][first];
        link->pkt->id = bufferSwitch->EXB[portID][first].id;
        link->pkt->srcIP = bufferSwitch->EXB[portID][first].srcIP;
        link->pkt->dstIP = bufferSwitch->EXB[portID][first].dstIP;
        link->pkt->generatedTime = bufferSwitch->EXB[portID][first].generatedTime;
        link->pkt->state = P3;//the packet is moved in a unidirectional way.

        bufferSwitch->EXB[portID][first].id = -1;
        bufferSwitch->EXB[portID][first].srcIP = -1;
        bufferSwitch->EXB[portID][first].dstIP = -1;
        bufferSwitch->EXB[portID][first].generatedTime = -1;
        bufferSwitch->EXB[portID][first].state = P_NULL;
        generateEventD = 1;
        if(bufferSwitch->type != EDGE_SWITCH || portID >= k/2)
            bufferSwitch->countNextENB[portID]--;

        changeForRemove(bufferSwitch->firstLastEXBs[portID]);
        int afterLast = bufferSwitch->firstLastEXBs[portID][1];
        if(bufferSwitch->EXB[portID][(last + 1) % BUFFER_SIZE].id != -1
            && bufferSwitch->EXB[portID][(last + 1) % BUFFER_SIZE].srcIP == -1
            && ((afterLast + 1) % BUFFER_SIZE) != ((last + 1) % BUFFER_SIZE)
            ){
            //printf("At switch %d\n", bufferSwitch->indexInGroup);
            //if()
            
            afterLast = (afterLast + 1) % BUFFER_SIZE;
            bufferSwitch->EXB[portID][afterLast].id = bufferSwitch->EXB[portID][(last + 1) % BUFFER_SIZE].id;
            bufferSwitch->EXB[portID][(last + 1) % BUFFER_SIZE].id = -1;
            bufferSwitch->EXB[portID][afterLast].srcIP = -1;
            bufferSwitch->EXB[portID][afterLast].dstIP = -1;
            bufferSwitch->EXB[portID][afterLast].generatedTime = 0;
            bufferSwitch->EXB[portID][afterLast].state = P_NULL;
        }
        if(bufferSwitch->firstLastEXBs[portID][0] == 0
            && bufferSwitch->firstLastEXBs[portID][1] == -1){
            //It means EXB[portID] is empty - has no packet
            //Therefore, we should move the id of next slot to
            //the first (index = 0)
            first = bufferSwitch->firstLastEXBs[portID][0];
            if(bufferSwitch->EXB[portID][first + 1].id != -1){
                bufferSwitch->EXB[portID][first].id = 
                    bufferSwitch->EXB[portID][first + 1].id;
                bufferSwitch->EXB[portID][first + 1].id = -1;
            }
        }
        int wasFull = emptySlots == 0;
        if(wasFull){
            if(bufferSwitch->stsEXBs[portID] == X11){
                //printf("Gio ko full nua nhung van giu X11\n");
                bufferSwitch->stsEXBs[portID] == X00;
                //exit(1);
            }
            *generateEventE = 1;
        }
    }
    return generateEventD;
}

int actionH_HOST(BufferHost *bufferHost, Packet *pktInLink
                    //, int *generateEventB
                ){
    int generateEventC = 0;
    //*generateEventB = 0;
    int wasFull = bufferHost->countNextENB == 0;
    bufferHost->countNextENB++;
    if(wasFull){
        if(bufferHost->firstEXB != -1
            && pktInLink->dstIP == -1
                ){
            generateEventC = 1;
        }
        else{
            //might be re-written in the future
        }
    }
    else{
        generateEventC = (pktInLink->id == -1) && (bufferHost->firstEXB != -1);
    }

    return generateEventC;
}

void actionG(BufferHost *buffHost, unsigned long *count, Packet *packet){
    *count += 1;
    if(buffHost->receivedPkts == NULL){
        buffHost->receivedPkts = malloc(sizeof(Sender));
        buffHost->receivedPkts->srcIP = packet->srcIP;
        buffHost->receivedPkts->segment = 0;
        buffHost->receivedPkts->offset = 1;
        buffHost->receivedPkts->next = NULL;
    }
    else{
        Sender *temp = buffHost->receivedPkts;
        //Sender *prev = NULL;
        int found = 0;
        while(temp != NULL && !found){
            if(temp->srcIP == packet->srcIP){
                if(temp->offset == ULONG_MAX){
                    temp->segment++;
                    temp->offset = 0;
                }
                temp->offset += 1;
                found = 1;
            }
            else{
                temp = temp->next;
                //prev = temp;
            }
        }

        if(!found){
            temp = malloc(sizeof(Sender));
            temp->srcIP = packet->srcIP;
            temp->segment = 0;
            temp->offset = 1;
            temp->next = buffHost->receivedPkts;
            buffHost->receivedPkts = temp;
        }
    }
    
    packet->id = -1;
    packet->srcIP = -1;
    packet->dstIP = -1;
    packet->generatedTime = -1;
    packet->state = P_NULL;
}

int actionH(BufferSwitch *buffSwitch, int type, int portID, Packet *pktInLink, int k){
    //return value is the value of variable: generateEventF
    if(type != EDGE_SWITCH || portID >= k/2){
        buffSwitch->countNextENB[portID]++;
    }
    if(countEmptySlots(buffSwitch->firstLastEXBs[portID][0],
                        buffSwitch->firstLastEXBs[portID][1])
            < BUFFER_SIZE
                ){
        int firstEXB = buffSwitch->firstLastEXBs[portID][0];
        if(buffSwitch->EXB[portID][firstEXB].state == P5){
            if(pktInLink->id == -1){
                buffSwitch->EXB[portID][firstEXB].state = P5bis;
                return 1;
            }
        }
        else
            return 0;
    }
    else
        return 0;
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
    if(last < first){
        last += BUFFER_SIZE;
    }
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