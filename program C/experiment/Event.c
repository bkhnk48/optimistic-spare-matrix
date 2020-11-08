#include <stdio.h>

#ifndef _TYPES_OF_EVENT_
#define _TYPES_OF_EVENT_
enum TypesOfEvent
{
    A = 0, //GenerationEvent
    B = 1, //LeavingSourceQueueEvent
    C = 2, //LeavingEXBEvent
    H_HOST = 3, //NotificationEvent at hosts
    D = 5, //ReachingENBEvent
    E = 6, //MovingInSwitchEvent
    F = 7, //LeavingSwitchEvent
    G = 4, //ReachingDestinationEvent
    H = 8, //NotificationEvent
    X = 9  //RandomEvent
};

enum Side{LEFT, RIGHT};

void loadArray(int a[1000]){
   int num;
   FILE *fptr;

   if ((fptr = fopen("random1000.txt","r")) == NULL){
       printf("Error! opening file");
       return;
   }
   int i = 0;
   while(!feof(fptr)){
    fscanf(fptr,"%d", &num);
    a[i] = num;
    i++;
   }

   fclose(fptr);
}
#endif