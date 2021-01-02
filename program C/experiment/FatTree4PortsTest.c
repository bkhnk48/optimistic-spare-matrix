#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FatTreeGraph.c"
#include "FatTreeTables.c"
#include "OptimizationSplay.c"
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
    int inport = 0;
    int generateEventB, generateEventC;
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
              nextIndex = allNodes[i].links[0].nextIndex;
              inport = allNodes[i].links[0].inport;
              generateEventC = actionB(&bufferHosts[i],
                                        allNodes[i].links[0].pkt,
                                        bufferSwitches[nextIndex].ENB[inport]
                                        );
              if(generateEventC)
                add(C, i, 0, currentTime +  defaultBias*33
                              , &root, arr
                            );
            }
            #pragma endregion
            
            else if(type == C){
              
              
            }
        }
        ongoingTime = -1;
        removeFirst(&first, &root, arr
               );
      
        currentTime = arr[first][3];
        ongoingTime = arr[first][3];
    }

    return 0;

}
