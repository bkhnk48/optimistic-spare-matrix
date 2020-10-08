/*
            An implementation of testing Splay 
*/

#include <stdio.h>
#include <stdlib.h>
#include "Splay.c"
#include "timing.c"


void main(int argc, char** argv) {
/* A sample use of these functions. Start with the empty tree,         */
/* insert some stuff into it, and then delete it                        */
    char *tID = argv[1];
    int testID = atoi(tID);
    /*
        1:  10, 20
        2:  10, 20, 30, 40, 50, 60, 70, 50, 70, 50, 60
    */
    
    Tree * root;

    
    root = NULL;              /* the empty tree */
    double wc1 = 0, wc2 = 0, cpuT = 0;
    
    if(testID == 1)
    {
        root = add(A, 0, 0, 0, 0, 10, root);
        root = add(A, 1, 1, 0, 0, 20, root);
    }

    if(testID == 2)
    {
        root = add(A, 0, 0, 0, 0, 10, root);
        root = add(A, 1, 0, 0, 0, 20, root);
        root = add(A, 0, 0, 0, 0, 30, root);
        root = add(A, 1, 0, 0, 0, 40, root);
        root = add(A, 0, 0, 0, 0, 50, root);
        root = add(A, 1, 0, 0, 0, 60, root);
        root = add(A, 0, 0, 0, 0, 70, root);
        root = add(A, 1, 1, 0, 0, 50, root);
        root = add(A, 1, 1, 0, 1, 70, root);
        root = add(A, 1, 1, 0, 2, 50, root);
        root = add(A, 1, 1, 0, 1, 60, root);
        
    }

    if(testID == 3)
    {
        
    }

    if(testID == 4)
    {
        
        int N = 0, i;
        timing(&wc1, &cpuT);
        for(N = 0; N < 1000; N++)
        {
            root = NULL;
            for(i = 0; i < 16; i++)
            {
                root = add(A, 0, 0, 0, 0, (i % 2), root);
            }
            free(root);
            if(i < 0){
                root += i + N;
            }
        }
        timing(&wc2, &cpuT);
        printf("Time: %f ms\n", (wc2 - wc1)*1000);
        printf("================================\n");
        return;
    }

    if(testID == 5)
    {
        int i = 0;
        timing(&wc1, &cpuT);
        int currentTime = 0;
        int endTime = 1000*1000*1000;
        root = NULL;
        for(i = 0; i < 16; i++)
        {
            root = add(A, 0, 0, 0, i, 0, root);
        }
        Tree * ev = (Tree *) malloc (sizeof (Tree));
        ev->endTime = -1;
        root = removeFirstEvent(ev, root);
        while(currentTime <= endTime && ev->endTime != -1)
        {
            if(ev->endTime == currentTime)
            {
                i = ev->idElementInGroup;//Lay id cua host trong danh sach cac hosts
                if(ev->type == A)
                {
                    root = add(A, //type A
                            0, //packetID 
                            i,
                            i, //location at this host
                            currentTime + 10000, //startTime = currentTime + HOST_DELAY
                            currentTime + 10000, //endTime = currentTime + HOST_DELAY
                            root);
                    root = add(B, //type B
                            0, //packetID 
                            i,
                            i, //location at this host
                            currentTime, //startTime = currentTime 
                            currentTime, //endTime = currentTime (right now)
                            root);
                }
                else if(ev->type == B)
                {
                    root = add(C, //type C
                            0, //packetID 
                            i,
                            i, //location at this host
                            currentTime, //startTime = currentTime 
                            currentTime, //endTime = currentTime (right now)
                            root);
                }
                else if(ev->type == C)
                {
                    root = add(D, //type D
                            0, //packetID 
                            i,
                            i, //location at this host
                            currentTime, //startTime = currentTime 
                            currentTime + 3000, //endTime = currentTime + 3000
                            root);
                }
                else if(ev->type == D)
                {
                    root = add(E, //type E
                            0, //packetID 
                            i,
                            i, //location at this host
                            currentTime, //startTime = currentTime 
                            currentTime + 100, //endTime = currentTime + 100
                            root);
                    root = add(H_HOST, //type H_HOST
                            0, //packetID 
                            i,
                            i, //location at this host
                            currentTime, //startTime = currentTime 
                            currentTime + 101, //endTime = currentTime + 101
                            root);
                }
                else if(ev->type == E)
                {
                    root = add(F, //type F
                            0, //packetID 
                            i,
                            i, //location at this host
                            currentTime, //startTime = currentTime 
                            currentTime + 100, //endTime = currentTime + 100
                            root);
                }
                else if(ev->type == F)
                {
                    if(i % 2 == 0)
                    {
                        root = add(G, //type G
                            0, //packetID 
                            i,
                            i, //location at this host
                            currentTime, //startTime = currentTime 
                            currentTime + 3000, //endTime = currentTime + 3000
                            root);
                    }
                    else{
                        root = add(D, //type D
                            0, //packetID 
                            i,
                            i, //location at this host
                            currentTime, //startTime = currentTime 
                            currentTime + 100, //endTime = currentTime + 100
                            root);
                    }
                }
                else if(ev->type == H_HOST)
                {

                }
            }
            ev->endTime = -1;
            root = removeFirstEvent(ev, 
                                        root);
            currentTime = ev->endTime;
            //int isNegativeEndTime = -(ev->endTime>>31);//0 hoac 1
            //currentTime = (1 - isNegativeEndTime)*(ev->endTime) + 
            //        isNegativeEndTime*currentTime;
        }
        timing(&wc2, &cpuT);
        printf("Time: %f ms\n", (wc2 - wc1)*1000);
        printf("================================\n");
    }
    //show(root);
}
