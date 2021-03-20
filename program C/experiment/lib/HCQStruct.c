#include "Node.c"

#ifndef _TYPES_OF_HCQSTRUCT_
#define _TYPES_OF_HCQSTRUCT_

typedef struct PureCalendarQueue{
    Node** buckets;
    int width;
    unsigned long nbuckets;
    //int firstsub;
    int resizeenable;
    unsigned long qsize;
    unsigned long lastprio;
    unsigned long lastbucket;
    double buckettop;
    unsigned long bot_threshold;
    unsigned long top_threshold; 
} HCQStruct;

void insert(Node* entry, HCQStruct *CQ);
Node* removeSoonestEvent(HCQStruct *CQ);
unsigned long newwidth(HCQStruct *CQ);
void resize(unsigned long newsize, HCQStruct *CQ);
void localInit(unsigned long nbuck, unsigned long bwidth, unsigned long startprio, HCQStruct *CQ);
void initqueue(HCQStruct *CQ);
void enqueue(Node* entry, HCQStruct *CQ);
Node* dequeue(HCQStruct *CQ);
void printBucket(Node* n, HCQStruct *CQ);
void printBuckets(HCQStruct *CQ);

void localInit(unsigned long nbuck, unsigned long bwidth, unsigned long startprio, HCQStruct *CQ){
    unsigned long i;
    double n;

    // khoi tao cac tham so
    CQ->buckets = (Node**) calloc(nbuck,sizeof(Node));
    CQ->width = bwidth;
    CQ->nbuckets = nbuck;

    // khoi tao cac bucket
    CQ->qsize = 0;
    for(i = 0; i < CQ->nbuckets; i++){
        CQ->buckets[i] = NULL;
    }

    // khoi tao cac chi so ban dau cua bucket dau tien
    CQ->lastprio = startprio;
    n = startprio / (CQ->width);
    CQ->lastbucket = ((unsigned long)n) % (CQ->nbuckets);
    CQ->buckettop = (n+1)*(CQ->width) + 0.5*(CQ->width);

    // khoi tao 2 linh canh dau vao cuoi
    CQ->bot_threshold = (CQ->nbuckets)/2 - 2;
    CQ->top_threshold = 2*(CQ->nbuckets);
}

void initqueue(HCQStruct *CQ){
    localInit(2,1,0.0, CQ);
    CQ->resizeenable = 1;
}

#endif