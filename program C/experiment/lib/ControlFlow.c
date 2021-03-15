#include <stdio.h>
#include <stdlib.h>
#include "NetworkNode.c"
#include "ShiftBits.c"
#include "Event.c"

#ifndef _CONTROL_FLOW_
#define _CONTROL_FLOW_
int chooseENB_ID(int EXB_ID, BufferSwitch *bufferSwitch, int k){
    //StoredPacket *EXB = bufferSwitch->EXB[EXB_ID];
    int found = 0;
    int j = bufferSwitch->firstLastEXBs[EXB_ID][1];
    j = (j == -1) ? 0 : ((j + 1) % BUFFER_SIZE);
    if(bufferSwitch->EXB[EXB_ID][j].dstIP == -1 
        && bufferSwitch->EXB[EXB_ID][j].srcIP == -1 
        && bufferSwitch->EXB[EXB_ID][j].id != -1){
        found = 1;
    }
    if(found == 0)
        return -1;
    int count = getCount(bufferSwitch->EXB[EXB_ID][j].id);
    int min = getMin(bufferSwitch->EXB[EXB_ID][j].id);
    int max = getMax(bufferSwitch->EXB[EXB_ID][j].id);

    int m = min;
    unsigned long soonestPkt = 
                    bufferSwitch->ENB[min][0].generatedTime;
    int pickUp = min;
    if(count > 2){
        for(m = min; m <= max; m++){
            if(bufferSwitch->requestedTimeOfENB[m] 
                    == bufferSwitch->requestedTimeToEXB[EXB_ID]
                && 
                    bufferSwitch->registeredEXBs[m]
                    == EXB_ID
                    )
            {
                if(soonestPkt > 
                        bufferSwitch->ENB[m][0].generatedTime){
                    soonestPkt = 
                        bufferSwitch->ENB[m][0].generatedTime;
                    pickUp = m;
                } 
                //countRequestedTime++;
            }
        }
    }
    else if(count == 2){
        pickUp =
            (bufferSwitch->ENB[max][0].generatedTime
                < bufferSwitch->ENB[min][0].generatedTime)
                ? max : min;
    }
    
    if(pickUp < 0 || pickUp >= k
        || pickUp == EXB_ID
        )
        return -1;
    return pickUp;          
}

int findENB_ID(int EXB_ID, BufferSwitch *bufferSwitch, unsigned long currentTime, int k){
    int found = 0;
    int j = bufferSwitch->firstLastEXBs[EXB_ID][1];
    j = (j == -1) ? 0 : ((j + 1) % BUFFER_SIZE);
    
    int m;
    int firstENB ;
    int lastENB;
    unsigned long soonestPkt = ULONG_MAX;
    int pickUp = -1;
    unsigned short count = 0;
    for(m = 0; m <= k - 1; m++){
        if(bufferSwitch->requestedTimeOfENB[m] 
                <= currentTime
            && 
                bufferSwitch->registeredEXBs[m]
                == EXB_ID
            && m != EXB_ID
                )
        {
            firstENB = bufferSwitch->firstLastENBs[m][0];
            lastENB = bufferSwitch->firstLastENBs[m][1];
            if(countEmptySlots(firstENB, lastENB) < BUFFER_SIZE){
                if(bufferSwitch->ENB[m][firstENB].id != -1 //This express prevents from resending a choosen pkt
                    &&
                    soonestPkt > 
                        bufferSwitch->ENB[m][firstENB].generatedTime){
                    soonestPkt = 
                        bufferSwitch->ENB[m][firstENB].generatedTime;
                    pickUp = m;
                } 
            }
            //countRequestedTime++;
        }
    }
    int min = pickUp;
    int max = pickUp;
    if(pickUp < k && pickUp >= 0 && pickUp != EXB_ID){
        if(bufferSwitch->requestedTimeToEXB[EXB_ID] != currentTime)
            bufferSwitch->requestedTimeToEXB[EXB_ID] = currentTime;
        for(m = 0; m <= k - 1; m++){
            if(bufferSwitch->requestedTimeOfENB[m] 
                <= currentTime
            && 
                bufferSwitch->registeredEXBs[m]
                == EXB_ID
            && m != EXB_ID //&& m != pickUp
                )
            {
                firstENB = bufferSwitch->firstLastENBs[m][0];
                lastENB = bufferSwitch->firstLastENBs[m][1];
                if(countEmptySlots(firstENB, lastENB) < BUFFER_SIZE){
                    if(soonestPkt == bufferSwitch->ENB[m][firstENB].generatedTime
                        && bufferSwitch->ENB[m][firstENB].id != -1
                        ){
                        count++;
                        min = (min > m) ? m : min;
                        max = (max < m) ? m : max;
                    }
                }
            }
        }
        if(count == 1){
            max = 0;
        }
        unsigned long result = 0;
        result |= count;
        result |= (min << 16);
        result |= ((unsigned long)max << 32);
        bufferSwitch->EXB[EXB_ID][j].id = result;
    }
    
    return pickUp;          
}

#endif
