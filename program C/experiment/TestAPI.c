#include <stdlib.h>
#include <stdio.h>
#include "NetworkNode.c"
#include <assert.h>

#ifndef _TEST_API_
#define _TEST_API_

unsigned long getCountFirstLast(unsigned long first, unsigned long last){
    int result = 0;
    if(first == -1){
        result = 0;
    }
    else{
        if(last == -1){
            result = 1;
        }
        else{
            result = last - first + 1;
        }
    }
    return result;
}

unsigned long getPacketsInSource(BufferHost *buffHost){
    unsigned long count = 0;
    unsigned long countSQ = 
            getCountFirstLast(buffHost->firstSQ, buffHost->lastSQ);
    
    unsigned long countEXB = 
            getCountFirstLast(buffHost->firstEXB, buffHost->lastEXB);
    
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

void checkENB_EXB(BufferSwitch *buffSwitch, int k){
    int i = 0, j = 0;
    for(i = 0; i < k; i++){
        for(j = 0; j < BUFFER_SIZE; j++){
            if(buffSwitch->ENB[i][j].dstIP != -1){
                assert(buffSwitch->ENB[i][j].srcIP != -1);
                assert(buffSwitch->ENB[i][j].id != -1);
            }
        }
    }

    for(i = 0; i < k; i++){
        for(j = 0; j < BUFFER_SIZE; j++){
            if(buffSwitch->EXB[i][j].dstIP != -1){
                assert(buffSwitch->EXB[i][j].srcIP != -1);
                assert(buffSwitch->EXB[i][j].id != -1);
            }
        }
    }
}

void assertPackets(NetworkNode *allNodes, 
                            BufferHost *buffHosts,
                            BufferSwitch *buffSwitches,
                            int numOfHosts,
                            int numOfSwitches,
                            int numOfPorts
                            ){
    /*
    * This func checks all generated Packets from a source
    * to validate whether the number is sum of 
    * packets in hosts, packets in switches (which belong to source) 
    * and packets in links (which belong to source)
    * this func has not yet counted received packets in destination(s)
    */                        
    int i = 0, j = 0, l = 0;
    unsigned long allGeneratedPackets = 0;
    unsigned long pktsInHost = 0;
    unsigned long pktsInSwitches = 0;
    unsigned long pktsInLinks = 0;
    for(i = 0; i < numOfHosts; i++){
        allGeneratedPackets = allNodes[i].generatedPackets;
        pktsInHost = getPacketsInSource(&buffHosts[i]);
        pktsInSwitches = 0;
        pktsInLinks = (allNodes[i].links[0].pkt->dstIP != -1) ? 1 : 0; 
        for(j = 0; j < numOfSwitches; j++){
            pktsInSwitches += getPacketsInSwitch(allNodes[i].ipv4
                    , &buffSwitches[j], numOfPorts);
            for(l = 0; l < numOfPorts; l++){
                pktsInLinks += 
                    (allNodes[j + numOfHosts].links[l].pkt->dstIP != -1
                    && allNodes[j + numOfHosts].links[l].pkt->srcIP == allNodes[i].ipv4
                        ) ? 1 : 0;
            }
        }
        //printf("validated %d: %ld = %ld + %ld + %ld\n",
        //         i, allGeneratedPackets, pktsInHost, pktsInLinks, pktsInSwitches);
        assert(allGeneratedPackets == pktsInHost + pktsInLinks + pktsInSwitches);
        
    }                            
}
#endif