#include <stdio.h>
#include <stdlib.h>
#include "CalendarQueue.c"
#include "timing.c"
#include <locale.h>

int main(int argc, char** argv) 
{
    initqueue();
    //int arbitrary[1000];
    //loadArray(arbitrary);
    double wc1 = 0, wc2 = 0, cpuT = 0;
    long majorPft1 = 0, majorPft2 = 0;
    unsigned long i = 0;
    unsigned long count = 0;
    unsigned long index = 0;
    
    int defaultSec = 70;

    if(argc >= 2)
    {
      defaultSec = atoi(argv[1]);
    }
    unsigned long currentTime = 0;
    unsigned long endTime = defaultSec*((unsigned long)(1000*1000));
    setlocale(LC_NUMERIC, "");
    
    printf("Simulation time is %ld (s)\n", endTime / (1000*1000));
    unsigned long mem = mem_avail();
    printf("Free memory Available = %'ld\n", mem / (1024*1024));

    printf("Start Simulating ......\n");

    timing(&wc1, &cpuT, &majorPft1);
    for(i = 0; i < 6750L; i++)
    {
        enqueue(new_node(A, i, 0L, 0L));
    }

    
    Node * ev = dequeue();
    while(currentTime <= endTime && ev->endTime != -1)
    {   //printf("hello world");
        if(ev->endTime == currentTime)
        {
            count++;
            /*printf("%d)Event type = %d at %d with endTime = %d\n"
                , count, ev->type, ev->idElementInGroup, ev->endTime
                    );*/
            i = ev->idElementInGroup;//Lay id cua host trong danh sach cac hosts
            if(ev->type == A)
            {
                enqueue(new_node(A, i, 0L, currentTime + 10000L));
                enqueue(new_node(B, i, 0L, currentTime +  3333L));
            }
            else if(ev->type == B)
            {
                enqueue(new_node(C, i, 0L, currentTime + 3333L));
            }
            //int temp = arbitrary[index % 1000];
            //if(temp != -1)
            {
                //enqueue(new_node(C, i, 0, currentTime + temp));
                //printf("%ld ", currentTime + temp);
            }
            //index++;
            ev->endTime = -1;
            ev = dequeue();

            currentTime = ev->endTime;
        }
    }

    timing(&wc2, &cpuT, &majorPft2);
    printf("Time: %'f ms with count = %'ld as well as page fault = %ld\n", 
                    (wc2 - wc1)*1000, count, majorPft2 - majorPft1);
    printf("================================\n");

    return 0;
}