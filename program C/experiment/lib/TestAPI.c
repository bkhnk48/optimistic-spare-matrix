#include <stdlib.h>
#include <stdio.h>
#include "NetworkNode.c"
#include <assert.h>
#include <limits.h>

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
    
    //printf("EXB has %ld, SQ has %ld ", countEXB, countSQ);
    count = countEXB + countSQ;
    return count;
}

unsigned long getPacketsInSwitch(int ipOfHost, BufferSwitch *buffSwitch, int k){
    unsigned long count = 0;
    int i = 0, j = 0;
    for(i = 0; i < k; i++){
        for(j = 0; j < BUFFER_SIZE; j++){
            if(buffSwitch->ENB[i][j].srcIP == ipOfHost){
                if(ipOfHost == 167838211)
                    printf("At ENB %d, slot %d, a pkt id = %ld available\n", i, j, buffSwitch->ENB[i][j].id);
                count++;
            }
        }
    }

    for(i = 0; i < k; i++){
        for(j = 0; j < BUFFER_SIZE; j++){
            if(buffSwitch->EXB[i][j].srcIP == ipOfHost){
                if(ipOfHost == 167838211)
                    printf("At EXB %d, slot %d, a pkt id = %ld available\n", i, j, buffSwitch->EXB[i][j].id);
                count++;
            }
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

void assertPackets(unsigned long total, 
                        NetworkNode *allNodes, 
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
    double allGeneratedPackets = 0;
    double pktsInHost = 0;
    double pktsInSwitches = 0;
    double pktsInLinks = 0;
    double pktsInDest = 0;
    int pktsInEachSwitch = 0;
    for(i = 0; i < numOfHosts; i++){
        allGeneratedPackets = allNodes[i].generatedPackets;
        pktsInHost = getPacketsInSource(&buffHosts[i]);
        pktsInSwitches = 0;
        pktsInDest = 0;
        pktsInLinks = (allNodes[i].links[0].pkt->dstIP != -1) ? 1 : 0; 
        if(i == 25)
            printf("Host %d has IP %d\n", i, allNodes[i].ipv4);
        for(j = 0; j < numOfSwitches; j++){
            if(i == 25){
                //printf("%d\n", allNodes[i].ipv4);
                //exit(1);
            }
            pktsInEachSwitch = getPacketsInSwitch(allNodes[i].ipv4
                    , &buffSwitches[j], numOfPorts);
            pktsInSwitches += pktsInEachSwitch;
            if(pktsInEachSwitch > 0 && i == 25){
                printf("At switch %d, we have %d pkts\n", j, pktsInEachSwitch);
            }
            for(l = 0; l < numOfPorts; l++){
                pktsInLinks += 
                    (allNodes[j + numOfHosts].links[l].pkt->dstIP != -1
                    && allNodes[j + numOfHosts].links[l].pkt->srcIP == allNodes[i].ipv4
                        ) ? 1 : 0;
            }
        }
        
        for(j = 0; j < numOfHosts; j++){
            if(j != i){
                Sender *temp = buffHosts[j].receivedPkts;
                while(temp != NULL){
                    if(temp->srcIP == allNodes[i].ipv4){
                        pktsInDest += temp->offset + temp->segment*ULONG_MAX;
                    }
                    temp = temp->next;
                }
            }
        }
        total -= pktsInDest;
        if(allGeneratedPackets != 0 && i == 25){
            printf("validated %d: %lf(all) = %lf(Src) + %lf(L) + %lf(Sw) + %lf(Dst)\n",
                 i, allGeneratedPackets, pktsInHost, pktsInLinks, pktsInSwitches, pktsInDest);
            assert(allGeneratedPackets == pktsInHost + pktsInLinks + pktsInSwitches + pktsInDest);
        }
        
    } 
    assert(total == 0);                           
}
#endif