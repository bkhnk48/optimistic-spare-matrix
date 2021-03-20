#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "Node.c"
#include "ACalendarQueue.c"
#include "HCQStruct.c"

HCQStruct *allCQs;
void putIntoQueueH(unsigned long endTime, unsigned int idNewNode);
void insertH(unsigned long endTime, unsigned int idNewNode);
unsigned int removeSoonestEventH();
unsigned long newwidthH();
void resizeH(unsigned long newsize);
void localInitH(unsigned long nbuck, unsigned long bwidth, unsigned long startprio);
void initqueueH();
void enqueueH(unsigned long endTime, unsigned int idNewNode);
unsigned int dequeueH();
void printBucketsH();


void initqueueH(int numOfHosts, int numOfSwitches){
    localInitA(2,1,0.0);
    resizeenable = 1;
    allCQs = malloc((numOfHosts + numOfSwitches)*sizeof(HCQStruct));
    int i = 0; 
    for(i = 0; i < numOfHosts + numOfSwitches; i++){
        initqueue(&allCQs[i]);
    } 
}
