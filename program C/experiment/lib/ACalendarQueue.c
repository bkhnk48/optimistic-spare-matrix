#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Node.c"
#define RIGHT_MASK 0x7fffffff
#define LEFT_MASK ((unsigned long)0x7fffffff << 32)
#define MASK_INT 65535

unsigned long *data; //[384];
unsigned int **arr; //[384][5];

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
