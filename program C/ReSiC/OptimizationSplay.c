#include <stdio.h>
#include <stdlib.h>
#include "Event.c"

enum Side{LEFT, RIGHT};

void add(int type, int portID, 
                int idElementInGroup,
                int endTime,
                int root,
                int arr[320][7]
                );
/* From now on, an event has 7 fields about:
   + it's type
   + idElementInGroup is the index of element in each group Hosts, Ways, Switches Edge, Agg, Core
   + the portID where the event happens (= -1 in case event - A, B, C, G, H occurs on host)
   + endTime: the time of event to be finished.
   + index of father.
   + index of left
   + index of right
*/

void show(int arr[320][7], int root);
void leaf(int arr[320][7], int root, enum Side side);

void show(int arr[320][7], int root)
{
   if(arr[root][0] != -1)
   {
      printf("\n===========> for event type = %d at end = %d in %d\n", 
               arr[root][0], arr[root][3], arr[root][1]);
      leaf(arr, arr[root][5], LEFT);
      leaf(arr, arr[root][6], RIGHT);
   }
   else{
      printf("===========> left NULL\n");
   }
}

void leaf(int arr[320][7], int root, enum Side side)
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
               arr[root][0], arr[root][3], arr[root][1]);
      leaf(arr, arr[root][5], LEFT);
      leaf(arr, arr[root][6], RIGHT);
   }
   else{
      printf("NULL\n");
   }
}