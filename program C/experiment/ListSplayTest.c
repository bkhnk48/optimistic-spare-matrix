#include <stdio.h>
#include <stdlib.h>
#include "ListSplay.c"
#include "timing.c"

int main()
{
    splay_tree *t = new_splay_tree();              /* the empty tree */
    double wc1 = 0, wc2 = 0, cpuT = 0;
    int i = 0;
    long count = 0;
    
    
    timing(&wc1, &cpuT);
    int currentTime = 0;
    int endTime = 1000*1000;
    
    for(i = 0; i < 6750; i++)
    {
        insert(t, A, i, 0, 0);
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
                insert(t, A, i, 0, currentTime + 10000);
                insert(t, B, i, 0, currentTime);
            }
            else if(ev->type == B)
            {
                insert(t, C, i, 0, currentTime);
            }
            free(ev);
            ev = removeFirst(t);
            
            currentTime = ev->endTime;
        }
    }
    
    timing(&wc2, &cpuT);
    printf("Time: %f ms with count = %ld\n", (wc2 - wc1)*1000, count);
    printf("================================\n");

    return 0;
}