#include <stdio.h>
#include <stdlib.h>
#include "Event.c"

enum Side{LEFT, RIGHT};

void add(int type, int packetID, int idNode,
                int idElementInGroup,
                int startTime, 
                int endTime,
                int * root,
                int index,
                int arr[320][9]
                );
void show(int arr[320][9], int root);
void leaf(int arr[320][9], int root, enum Side side);

void show(int arr[320][9], int root)
{
   if(arr[root][0] != -1)
   {
      printf("\n===========> for event type = %d at end = %d in %d\n", 
               arr[root][0], arr[root][1], arr[root][2]);
      leaf(arr, arr[root][7], LEFT);
      leaf(arr, arr[root][8], RIGHT);
   }
   else{
      printf("===========> left NULL\n");
   }
}

void leaf(int arr[320][9], int root, enum Side side)
{
   printf("===========> ");
   if(side == LEFT)
   {
       printf("left ");
   }
   else{
       printf("right ");
   }
   if(arr[root][0] != -1)
   {
      printf("for event type = %d at end = %d in %d\n", 
               arr[root][0], arr[root][1], arr[root][2]);
      leaf(arr, arr[root][7], LEFT);
      leaf(arr, arr[root][8], RIGHT);
   }
   else{
      printf("NULL\n");
   }
}