#include <stdio.h>
#include <stdlib.h>
#include "VoidSplay.c"
#include "timing.c"
#include <locale.h>

int main(int argc, char** argv) 
{
    splay_tree *t = new_splay_tree();              /* the empty tree */
    double wc1 = 0, wc2 = 0, cpuT = 0;
    long majorPft1 = 0, majorPft2 = 0;
    int i = 0;
    long count = 0;
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

    for(i = 0; i < 6750; i++)
    {
        insert(t, new_node(A, i, 0, 0));
    }

    node * ev = removeFirst(t);
    while(currentTime <= endTime && ev->endTime != -1)
    {
        if(ev->endTime == currentTime)
        {
            count++;
            /*printf("%d)Event type = %d at %d with endTime = %d\n"
                , count, ev->type, ev->idElementInGroup, ev->endTime
                    );*/
            i = ev->idElementInGroup;//Lay id cua host trong danh sach cac hosts
            if(ev->type == A)
            {
                insert(t, new_node(A, i, 0, currentTime + 10000));
                insert(t, new_node(B, i, 0, currentTime +  3333));
            }
            else if(ev->type == B)
            {
                insert(t, new_node(C, i, 0, currentTime +  3333));
            }
            free(ev);
            ev = removeFirst(t);
            
            currentTime = ev->endTime;
        }
    }

    
    timing(&wc2, &cpuT, &majorPft2);
    printf("Time: %'f ms with count = %'ld as well as page fault = %ld\n", 
                    (wc2 - wc1)*1000, count, majorPft2 - majorPft1);
    printf("================================\n");

    return 0;
}