#include <stdlib.h>
#include <stdio.h>
#include "NetworkNode.c"

#ifndef _TEST_API_
#define _TEST_API_

unsigned long getCount(unsigned long first, unsigned long last){
    int result = 0;
    if(first == -1){
        result = 0;
    }
    else{
        if(last == -1){
            result = 1;
        }
        else{
            result = first - last + 1;
        }
    }
    return result;
}

unsigned long getPacketsInSource(BufferHost *buffHost){
    unsigned long count = 0;
    unsigned long countSQ = 
            getCount(buffHost->firstSQ, buffHost->lastSQ);
    
    unsigned long countEXB = 
            getCount(buffHost->firstEXB, buffHost->lastEXB);
    
    count = countEXB + countSQ;
    return count;
}

unsigned long getPacketsInSwitch(int ipOfHost, BufferSwitch *buffSwitch, int k){
    unsigned long count = 0;
    int i = 0, j = 0;
    for(i = 0; i < k; i++){
        for(j = 0; j < BUFFER_SIZE; j++){
            if(buffSwitch->ENB[i][j].srcIP == ipOfHost)
                count++;
        }
    }

    for(i = 0; i < k; i++){
        for(j = 0; j < BUFFER_SIZE; j++){
            if(buffSwitch->EXB[i][j].srcIP == ipOfHost)
                count++;
        }
    }

    return count;
}

#endif