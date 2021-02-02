#include <stdlib.h>
#include <stdio.h>
#include "NetworkNode.c"

#ifndef _TEST_API_
#define _TEST_API_

unsigned long getPacketOfHost(unsigned long idHost, BufferHost *buffHost){
    unsigned long count = 0;
    unsigned long countSQ = 0;
    if(buffHost->firstSQ == -1){
        countSQ = 0;
    }
    else{
        if(buffHost->lastSQ == -1){
            countSQ = 1;
        }
        else{
            countSQ = buffHost->firstSQ - buffHost->lastSQ + 1;
        }
    }

    
    return count;
}

#endif