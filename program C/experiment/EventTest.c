#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FatTreeGraph.c"
#include "FatTreeTables.c"
#include "MergeSplay.c"
#include "timing.c"
#include <limits.h>
#include <locale.h>

int main(int argc, char** argv) {
    unsigned int first = UINT_MAX;
    int defaultSec = 1;
    int defaultBias = 0;
    int k = 8;
    int PACKET_SIZE = 12*1000; //12KB
    int SWITCH_CYCLE = 100;
    int packets_per_sec = 1000;//each sec generated 1000 packets
    int T = 1000000;
    int BANDWIDTH_HOST = 12*1000*1000;//12MByte/s
    int loadingTime = BANDWIDTH_HOST / PACKET_SIZE + 13;

    if(argc >= 2)
    {
        k = atoi(argv[1]);
        if(argc > 2)
          defaultSec = atoi(argv[2]);
        if(argc > 3)
            defaultBias = atoi(argv[3]);
    }

    unsigned long currentTime = 0;
    int numOfHosts = k*k*k/4;
    unsigned long endTime = defaultSec*((unsigned long)(1000*1000));

    unsigned long i, j;
    unsigned int N, root = UINT_MAX;
    
    arr = malloc(sizeof * arr * (6*k*k*k));
    data = malloc((6*k*k*k)*sizeof(unsigned long));
    for(i = 0; i < 6*k*k*k; i++){
        arr[i] = malloc(sizeof * arr[i] * 5);
        for(j = 0; j < 5; j++){
            arr[i][j] = UINT_MAX;
        }
        data[i] = 0;
    }

    
    int *pairs = NULL;
    pairs = malloc((k*k*k/4) * sizeof(int));
    for(i = 0; i < numOfHosts; i++){
        if((i + 1) % (k/2) != 0)
            pairs[i] = (i/(k/2) + 1)*(k/2) - 1;
        else
            pairs[i] = 0;        
    }

    Tables *tablesOfSwitches = malloc(sizeof(Tables));
    buildTables(tablesOfSwitches, k);
    BufferHost *bufferHosts = initBufferHosts(k*k*k/4);
    BufferSwitch *bufferSwitches = initBufferSwitches(k*k*5/4, k);
    assignTypeOfSwitch(bufferSwitches, k);
    NetworkNode *allNodes = initNetworkNodes(k*k*k/4, 5*k*k/4, k);

    setlocale(LC_NUMERIC, "");
   

    printf("Start Simulating ......\n");
    unsigned long count = 0;
    int nextIP = 0;
    int nextIndex;
    int nextPort = 0;
    int generateEventB, generateEventC, generateEventD;
    int generateEventE;
    int idNode = 0;

    root = UINT_MAX;

    for(i = 0; i < k/2; i++)//Only test first k/2 hosts
    {
      idNode = hash(i, HOST, 0, A, k);
      add(0, i, 0, 0, &root//, arr
                    , idNode
                  );
    }

    removeFirst(&first, &root//, arr
                  );

    unsigned long ongoingTime = (unsigned long)arr[first][0] << 32 
                                  + arr[first][1];
    while(currentTime <= endTime && ongoingTime != -1)
    {
        if(ongoingTime == currentTime)
        {
            count++;
         
            #pragma region get value from data array
            int type = data[first] & 65535;//type of event
            i = data[first] >> 32;//idElementInGroup
            arr[first][0] = UINT_MAX;
            arr[first][1] = UINT_MAX;
            arr[first][2] = UINT_MAX;
            arr[first][3] = UINT_MAX;
            arr[first][4] = UINT_MAX;
            #pragma endregion
         
            #pragma region action of Event type A
            if(type == A)
            {
                add(A, i, 0, currentTime + T, &root, first
                        );
                generateEventB = actionA(T, currentTime, &bufferHosts[i]);
                if(generateEventB){
                  add(B, i, 0, currentTime +  defaultBias*13
                              , &root, first + 1
                        );  
                }
            }
            #pragma endregion 
            
            #pragma region action of Event type B
            else if(type == B){
              
              generateEventC = actionB(&bufferHosts[i],
                                        allNodes[i].links[0].pkt
                                        );
              if(generateEventC)
                add(C, i, 0, currentTime +  defaultBias*33
                              , &root, first + 1
                            );
            }
            #pragma endregion
            
            else if(type == C){
              #pragma region action of Event type C
              nextIndex = allNodes[i].links[0].nextIndex;
              nextPort = allNodes[i].links[0].nextPort;
              generateEventB = 0;
              generateEventD = actionC(&bufferHosts[i], 
                              allNodes[i].links, &generateEventB,
                              getIPv4OfHost(pairs[i], k), T
                              );
              if(generateEventB)
                add(B, i, 0, currentTime +  defaultBias*13
                              , &root, first - 1 
                        ); 
              if(generateEventD){
                idNode = hash(nextIndex, EDGE_SWITCH, nextPort, D, k);
                add(D, nextIndex, nextPort, currentTime + loadingTime
                              , &root, idNode
                        );
              }
            #pragma endregion
            }
            else if(type == D){
              #pragma region action of Event type D
              int portID = (data[first] >> 16) & MASK_INT;
              Packet *ENB = bufferSwitches[i].ENB[portID];
              
              int idPrevHost = allNodes[i + numOfHosts].links[portID].nextIndex;
              Packet *pkt = allNodes[idPrevHost].links[0].pkt;
              
              int posInENB = receivePacket(&bufferSwitches[i].stsENBs[portID], 
                                              ENB, 
                                              &bufferSwitches[i].requestedTimeOfENB[portID],
                                              currentTime,
                                              pkt);
              
              //printf("AFTER that state of ENB %d\n", bufferSwitches[i].stsENBs[portID]);
              if(posInENB == 0){
              //Packet is ahead of all other ones on ENB
                assert(bufferSwitches[i].requestedTimeOfENB[portID] == currentTime);
                
                int nextIP = next(ENB[0].srcIP, 
                                  allNodes[i + numOfHosts].ipv4,
                                    ENB[0].dstIP,
                                    k, &(tablesOfSwitches->tables[i])
                                  );
                  
                int nextEXB = getEXB_ID(nextIP, 
                                allNodes[i + numOfHosts].type, k);
                //this func has two params: 
                // + nextEXB: the port ID of the next EXB
                // + registeredEXB[portID]: the array's element to store the nextEXB
                //additional info: portID - ID of ENB in which outgoing packet 
                signEXB_ID(nextEXB, &bufferSwitches[i].registeredEXBs[portID]);

                generateEventE = actionD(portID, 
                                          bufferSwitches[i].EXB[nextEXB],
                                          &bufferSwitches[i].stsEXBs[nextEXB],
                                          currentTime
                                          );
                if(generateEventE){
                  
                  idNode = hash(i, EDGE_SWITCH, nextEXB, E, k);
                  add(E, i, nextEXB, currentTime + SWITCH_CYCLE
                              , &root, idNode
                        );
                }
              }
              #pragma endregion
            }
            else if(type == E){
              #pragma region action of Event type E
              int portID = (data[first] >> 16) & MASK_INT;
              assert(portID == k/2 - 1 || (portID == 0));
              StoredPacket *EXB = bufferSwitches[i].EXB[portID];
              int found = 0;
              if(portID == k/2 - 1){
                for(j = 0; j < BUFFER_SIZE; j++){
                  if(EXB[j].dstIP == -1 
                        && EXB[j].srcIP == -1 
                        && EXB[j].id != -1){
                    found = 1;
                    break;
                  }
                }
                assert(found == 1);
                int count = getCount(EXB[j].id);
                int min = getMin(EXB[j].id);
                int max = getMax(EXB[j].id);
                int countRequestedTime = 0;
                int m = min;
                unsigned long soonestPkt = 
                              bufferSwitches[i].ENB[min][0].generatedTime;
                int pickUp = min;
                for(m = min; m <= max; m++){
                  if(bufferSwitches[i].requestedTimeOfENB[m] 
                          == EXB[j].requestedTime
                      && 
                        bufferSwitches[i].registeredEXBs[m]
                          == portID
                          )
                  {
                    if(soonestPkt > bufferSwitches[i].ENB[m][0].generatedTime)
                    {
                      soonestPkt = bufferSwitches[i].ENB[m][0].generatedTime;
                      pickUp = m;
                    } 
                    countRequestedTime++;
                  }
                }
                assert(pickUp >= min && pickUp <= max);
                assert(count == countRequestedTime);
              }
              
              #pragma endregion
            }
        }
        ongoingTime = -1;
        removeFirst(&first, &root//, arr
               );
      
        currentTime = ((unsigned long)arr[first][0] << 32)
                                  + arr[first][1];
        ongoingTime = currentTime;
    }
    printf("\n\nFINISH !!!!!!!!!!!! ^_^....\n");
    return 0;
}