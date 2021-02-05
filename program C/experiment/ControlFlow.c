#include <stdio.h>
#include <stdlib.h>
#include "NetworkNode.c"
#include "ShiftBits.c"

#ifndef _CONTROL_FLOW_
#define _CONTROL_FLOW_
int chooseENB_ID(int EXB_ID, BufferSwitch *bufferSwitch, int k){
    //StoredPacket *EXB = bufferSwitch->EXB[EXB_ID];
    int found = 0;
    int j = bufferSwitch->firstLastEXBs[EXB_ID][1];
    j = (j == -1) ? 0 : ((j + 1) % BUFFER_SIZE);
    if(EXB[j].dstIP == -1 
        && EXB[j].srcIP == -1 
        && EXB[j].id != -1){
        found = 1;
    }
    if(found == 0)
        return -1;
    int count = getCount(EXB[j].id);
    int min = getMin(EXB[j].id);
    int max = getMax(EXB[j].id);

    int m = min;
    unsigned long soonestPkt = 
                    bufferSwitch->ENB[min][0].generatedTime;
    int pickUp = min;
    if(count > 2){
        for(m = min; m <= max; m++){
            if(bufferSwitch->requestedTimeOfENB[m] 
                    == EXB[j].requestedTime
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
#endif
