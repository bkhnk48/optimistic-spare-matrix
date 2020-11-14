#include <stdio.h>
#include <stdlib.h>
#include "CalendarQueue.c"
#include "timing.c"
#include <locale.h>


int main()
{
    initqueue();
    //int arbitrary[1000];
    //loadArray(arbitrary);
    double wc1 = 0, wc2 = 0, cpuT = 0;
    unsigned long i = 0;
    unsigned long count = 0;
    unsigned long index = 0;
    
    unsigned long currentTime = 0;
    unsigned long endTime = 60*((unsigned long)(1000*1000));
    printf("Simulation time is %ld (s)\n", endTime / (1000*1000));

    timing(&wc1, &cpuT);
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

    timing(&wc2, &cpuT);
    setlocale(LC_NUMERIC, "");
    printf("Time: %'f ms with count = %'ld\n", (wc2 - wc1)*1000, count);
    printf("================================\n");

    return 0;
}