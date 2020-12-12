#include <stdio.h>
#include <stdlib.h>
#include "VoidSplay.c"
#include "timing.c"
#include <math.h>


int main(int argc, char** argv) 
{
    splay_tree *t = new_splay_tree();              /* the empty tree */
    double wc1 = 0, wc2 = 0, cpuT = 0;
    unsigned long i = 0;
    long count = 0;
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
        insert(t, new_node(A, i, 0L, i));
    }

    node * ev = removeFirst(t);
    while(currentTime <= endTime && ev->endTime != -1)
    {
        if(ev->endTime == currentTime)
        {
            count++;

            i = ev->idElementInGroup;//Lay id cua host trong danh sach cac hosts
            if(ev->type == A)
            {
                insert(t, new_node(A, i, 0L, currentTime + 10000));
                insert(t, new_node(B, i, 0L, currentTime +  defaultBias*13
                                    ));
            }
            else if(ev->type == B)
            {
                insert(t, new_node(C, i, 0L, currentTime +  defaultBias*33
                                    ));
            }
            free(ev);
            ev = removeFirst(t);
            
            currentTime = ev->endTime;
        }
    }

    //printf("Max elements of Tree = %d\n", maxSize);
    //printf("Stop Simulating...\n");
    timing(&wc2, &cpuT);
    printf("Time: %'f ms with count = %'ld\n", (wc2 - wc1)*1000, count);
    printf("================================\n");
    badness(wc2 - wc1, page_size, proc_statm);

    return 0;
}