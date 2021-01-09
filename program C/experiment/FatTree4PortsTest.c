#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FatTreeGraph.c"
#include "FatTreeTables.c"
#include "SplitSplay.c"
#include "timing.c"
#include <locale.h>
#include <math.h>

int main(int argc, char** argv) {
    double wc1 = 0, wc2 = 0, cpuT = 0;
    int first = -1;
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

    unsigned long arr[384][7];//384 = 6*(k*k*k) as k = 4
    //= 16*5 + (4*4/2)*((4/2)*4 + 3*(4/2)) + idElementInGroup*4*4 + portID*4 + (type - D);
    //Cach tinh khac:
    //So su kien xay ra tren HOST: (k*k*k/4)*5 = 80
    //So su kien xay ra tren Edge: (4*(k/2) + 3*(k/2))*k*k/2 = 14*8 = 112
    //So su kien xay ra tren Switch: (4*k)*3*k*k/4 = 3*k*k*k = 192
    //Tong: 80 + 112 + 192 = 80 + 304 = 384 (6*k*k*k)
    unsigned long i;
    int j, N, root = -1;
    for(i = 0; i < 384; i++)
    {
      for(j = 0; j < 7; j++)
      {
         arr[i][j] = -1L;
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
    //char* p = malloc(1 * 1024 * 1024 * 1024);
    timing(&wc1, &cpuT);

    root = -1;

    for(i = 0; i < 16; i++)
    {
      add(0, i, 0, 0, &root, arr);
    }

    removeFirst(&first, &root, arr);

    unsigned long ongoingTime = arr[first][3];
    while(currentTime <= endTime && ongoingTime != -1)
    {
        if(ongoingTime == currentTime)
        {
            count++;
         
            int type = arr[first][0];
            i = arr[first][1];
            arr[first][3] = -1L;
            arr[first][4] = -1L;
            arr[first][5] = -1L;
            arr[first][6] = -1L;
         
            #pragma region action of Event type A
            if(type == A)
            {
                add(A, i, 0, currentTime + T, &root, arr
                        );
                generateEventB = actionA(T, currentTime, &bufferHosts[i]);
                if(generateEventB){
                  add(B, i, 0, currentTime +  defaultBias*13
                              , &root, arr
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
                              , &root, arr
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
                              , &root, arr
                        ); 
              if(generateEventD)
                add(-D, nextIndex, nextPort, currentTime + loadingTime
                              , &root, arr
                        );
            }
            #pragma endregion
        }
        ongoingTime = -1;
        removeFirst(&first, &root, arr
               );
      
        currentTime = arr[first][3];
        ongoingTime = arr[first][3];
    }

    return 0;

}
