#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FatTreeGraph.c"
#include "FatTreeTables.c"
#include "CalendarQueue.c"
#include "ControlFlow.c"
#include "timing.c"
#include "TestAPI.c"
#include "Throughput.c"
#include "PairStrategies.c"
#include <limits.h>
#include <locale.h>
#include <math.h>



int main(int argc, char** argv) 
{
    //double* pBigArray = (double*)malloc(sizeof(double) * 536870912); 
    initqueue();
    //int arbitrary[1000];
    //loadArray(arbitrary);
    double wc1 = 0, wc2 = 0, cpuT = 0;
    unsigned long i = 0;
    unsigned long count = 0;
    unsigned long index = 0;
    
    int defaultSec = 70;
    int defaultBias = 1;
    ProcStatm proc_statm;
    long page_size = sysconf(_SC_PAGESIZE);

    if(argc >= 2)
    {
      defaultSec = atoi(argv[1]);
      if(argc >= 3)
        defaultBias = atoi(argv[2]);
    }
    unsigned long currentTime = 0;
    unsigned long endTime = defaultSec*((unsigned long)(1000*1000));
    
    
    printf("Simulation time is %ld (s)\n", endTime / (1000*1000));
    unsigned long mem = mem_avail();
    printf("Free memory Available = %'ld\n", mem / (1024*1024));

    printf("Start Simulating ......\n");
    //char* p = malloc(1 * 1024 * 1024 * 1024);
    
    timing(&wc1, &cpuT);
    for(i = 0; i < 6750L; i++)
    {
        enqueue(new_node(A, i, 0L, i));
    }

    
    Node * ev = dequeue();
    while(currentTime <= endTime && ev->endTime != -1)
    {   
        if(ev->endTime == currentTime)
        {
            count++;
            
            i = ev->idElementInGroup;//Lay id cua host trong danh sach cac hosts
            if(ev->type == A)
            {
                enqueue(new_node(A, i, 0L, currentTime + 10000));
                enqueue(new_node(B, i, 0L, currentTime + defaultBias*13
                                        ));
            }
            else if(ev->type == B)
            {
                enqueue(new_node(C, i, 0L, currentTime + defaultBias*33
                                        ));
            }
            ev->endTime = -1;
            ev = dequeue();

            currentTime = ev->endTime;
        }
    }

    //printf("Stop Simulating...\n");
    timing(&wc2, &cpuT);
    printf("Time: %'f ms with count = %'ld\n", (wc2 - wc1)*1000, count);
    printf("================================\n");
    badness(wc2 - wc1, page_size, proc_statm);

    return 0;
}