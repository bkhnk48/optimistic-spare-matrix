#include <stdio.h>
#include <stdlib.h>
#include "CalendarQueue.c"
#include "timing.c"

int main()
{
    initqueue();
    int arbitrary[1000];
    loadArray(arbitrary);
    double wc1 = 0, wc2 = 0, cpuT = 0;
    int i = 0;
    long count = 0;
    int index = 0;
    timing(&wc1, &cpuT);
    unsigned long currentTime = 0;
    unsigned long endTime = 5*((unsigned long)(1000*1000));

    for(i = 0; i < 6750; i++)
    {
        enqueue(new_node(A, i, 0, 0));
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
                enqueue(new_node(A, i, 0, currentTime + 10000));
                enqueue(new_node(B, i, 0, currentTime));
            }
            else if(ev->type == B)
            {
                enqueue(new_node(C, i, 0, currentTime));
            }
            int temp = arbitrary[index % 1000];
            if(temp != -1)
            {
                enqueue(new_node(C, i, 0, currentTime + temp));
                //printf("%ld ", currentTime + temp);
            }
            index++;
            ev->endTime = -1;
            ev = dequeue();

            currentTime = ev->endTime;
        }
    }

    timing(&wc2, &cpuT);
    printf("\nTime: %f ms with count = %ld\n", (wc2 - wc1)*1000, count);
    printf("================================\n");

    return 0;
}