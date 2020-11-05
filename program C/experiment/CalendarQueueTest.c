#include "CalendarQueue.c"
#include "timing.c"

int main()
{
    List POOL[QSPACE];
    CalendarQueue *q = initqueue(POOL);
    double wc1 = 0, wc2 = 0, cpuT = 0;
    int i = 0;
    long count = 0;
    timing(&wc1, &cpuT);
    unsigned long currentTime = 0;
    unsigned long endTime = 10*((unsigned long)(1000*1000));

    for(i = 0; i < 6750; i++)
    {
        insert(q, new_node(A, i, 0, 0), POOL);
    }
    
    Node* ev = dequeue(q, POOL);
    while(currentTime <= endTime && ev->endTime != -1)
    {
        if(ev->endTime == currentTime)
        {
            count++;
            //printf("%d)Event type = %d at %d with endTime = %d\n", count, ev->type, ev->idElementInGroup, ev->endTime);
            i = ev->idElementInGroup;//Lay id cua host trong danh sach cac hosts
            if(ev->type == A)
            {
                insert(q, new_node(A, i, 0, currentTime + 10000), POOL);
                insert(q, new_node(B, i, 0, currentTime), POOL);
            }
            else if(ev->type == B)
            {
                insert(q, new_node(C, i, 0, currentTime), POOL);
            }
            ev->endTime = -1;
            ev = dequeue(q, POOL);

            currentTime = ev->endTime;
        }
    }
    
    timing(&wc2, &cpuT);
	
    printf("Time: %f ms with count = %ld\n", (wc2 - wc1)*1000, count);
    printf("================================\n");

    return 0;
}