#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Node.c"
#define RIGHT_MASK 0x7fffffff
#define LEFT_MASK ((unsigned long)0x7fffffff << 32)
#define MASK_INT 65535

unsigned long *data; //[384];
unsigned int **arr; //[384][5];
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

void insert(int type, int idElementInGroup,
                int portID, 
                unsigned long endTime,
                unsigned int idNewNode);
int removeSoonestEvent();
unsigned long newwidth();
void resize(unsigned long newsize);
void localInit(unsigned long nbuck, unsigned long bwidth, 
                                    unsigned long startprio);
void initqueue();
void enqueue(int type, int idElementInGroup,
                int portID, 
                unsigned long endTime,
                unsigned int idNewNode);
int dequeue();
void printBuckets();

void resize(unsigned long newsize){
    unsigned long bwidth;
    
    unsigned long oldnbuckets;
    Node** oldbuckets;

    if(!resizeenable) return;

    bwidth = newwidth();
    //oldbuckets = buckets;
    oldnbuckets = nbuckets;

    localInit(newsize,bwidth,lastprio);

    unsigned long i;
    // them lai cac phan tu vao calendar moi
    for(i=0; i < oldnbuckets; i++){
        Node* foo = oldbuckets[i];
        while(foo!=NULL){ // tranh viec lap vo han
            Node* tmp = new_node(foo->type,foo->idElementInGroup,
                                            foo->portID,foo->endTime);
            //insert(tmp);
            foo = foo->next;
        }
    }

    return;
}


void localInit(unsigned long nbuck, unsigned long bwidth, unsigned long startprio){
    unsigned long i;
    double n;

    width = bwidth;
    nbuckets = nbuck;

    // khoi tao cac bucket
    qsize = 0;

    // khoi tao cac chi so ban dau cua bucket dau tien
    lastprio = startprio;
    n = startprio / width;
    lastbucket = ((unsigned long)n) % nbuckets;
    buckettop = (n+1)*width + 0.5*width;

    // khoi tao 2 linh canh dau vao cuoi
    bot_threshold = nbuckets/2 - 2;
    top_threshold = 2 * nbuckets;
}

void initqueue(){
    localInit(2,1,0.0);
    resizeenable = 1;
}