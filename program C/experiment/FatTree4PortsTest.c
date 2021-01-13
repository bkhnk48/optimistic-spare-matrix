#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FatTreeGraph.c"
#include "FatTreeTables.c"
#include "MergeSplay.c"
#include "timing.c"
#include <limits.h>
#include <locale.h>
#include <math.h>

int main(int argc, char** argv) {
    double wc1 = 0, wc2 = 0, cpuT = 0;
    unsigned int first = UINT_MAX;
    int defaultSec = 1;
    int defaultBias = 0;
    ProcStatm proc_statm;
    long page_size = sysconf(_SC_PAGESIZE);
    int k = 4;
    int PACKET_SIZE = 12*1000; //12KB
    int packets_per_sec = 1000;//each sec generated 1000 packets
    int T = 1000000;
    int BANDWIDTH_HOST = 12*1000*1000;//12MByte/s
    int loadingTime = BANDWIDTH_HOST / PACKET_SIZE + 13;

    if(argc >= 2)
    {
        k = atoi(argv[1]);
        defaultSec = atoi(argv[2]);
        if(argc >= 3)
            defaultBias = atoi(argv[3]);
    }
    unsigned long currentTime = 0;
    unsigned long endTime = defaultSec*((unsigned long)(1000*1000));

    //unsigned int arr[384][5];//384 = 6*(k*k*k) as k = 4
    //= 16*5 + (4*4/2)*((4/2)*4 + 3*(4/2)) + idElementInGroup*4*4 + portID*4 + (type - D);
    //Cach tinh khac:
    //So su kien xay ra tren HOST: (k*k*k/4)*5 = 80
    //So su kien xay ra tren Edge: (4*(k/2) + 3*(k/2))*k*k/2 = 14*8 = 112
    //So su kien xay ra tren Switch: (4*k)*3*k*k/4 = 3*k*k*k = 192
    //Tong: 80 + 112 + 192 = 80 + 304 = 384 (6*k*k*k)
    unsigned long i, j;
    unsigned int N, root = UINT_MAX;
    for(i = 0; i < 384; i++)
    {
      for(j = 0; j < 5; j++)
      {
         arr[i][j] = UINT_MAX;
      }
    }

    int *pairs = NULL;
    pairs = Stride(8, k);
    Tables *tablesOfSwitches = malloc(sizeof(Tables));
    buildTables(tablesOfSwitches, k);
    BufferHost *bufferHosts = initBufferHosts(k*k*k/4);
    BufferSwitch *bufferSwitches = initBufferSwitches(k*k*5/4, k);
    NetworkNode *allNodes = initNetworkNodes(k*k*k/4, 5*k*k/4, k);

    setlocale(LC_NUMERIC, "");
   
    printf("Simulation time is %ld (s)\n", endTime / (1000*1000));
    unsigned long mem = mem_avail();
    printf("Free memory Available = %'ld\n", mem / (1024*1024));

    printf("Start Simulating ......\n");
    unsigned long count = 0;
    int nextIP = 0;
    int nextIndex;
    int nextPort = 0;
    int generateEventB, generateEventC, generateEventD;
    int idNode = 0;
    //char* p = malloc(1 * 1024 * 1024 * 1024);
    timing(&wc1, &cpuT);

    root = UINT_MAX;

    for(i = 0; i < 16; i++)
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
         
            int type = data[first] & 65535;
            i = data[first] >> 32;
            arr[first][0] = UINT_MAX;
            arr[first][1] = UINT_MAX;
            arr[first][2] = UINT_MAX;
            arr[first][3] = UINT_MAX;
            arr[first][4] = UINT_MAX;
         
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
                              getIPv4OfHost(pairs[i], k)
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
            }
            #pragma endregion
        }
        ongoingTime = -1;
        removeFirst(&first, &root//, arr
               );
      
        currentTime = ((unsigned long)arr[first][0] << 32)
                                  + arr[first][1];
        ongoingTime = currentTime;
    }

    timing(&wc2, &cpuT);
    printf("Time: %'f ms with count = %'ld\n", (wc2 - wc1)*1000, count);
    printf("================================\n");
    badness(wc2 - wc1, page_size, proc_statm);
    return 0;

}
