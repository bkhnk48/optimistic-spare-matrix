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

unsigned long getPacketsInHost(unsigned long idHost, BufferHost *buffHost){
    unsigned long count = 0;
    unsigned long countSQ = 
            getCount(buffHost->firstSQ, buffHost->lastSQ);
    
    unsigned long countEXB = 
            getCount(buffHost->firstEXB, buffHost->lastEXB);
    
    count = countEXB + countSQ;
    return count;
}

#endif