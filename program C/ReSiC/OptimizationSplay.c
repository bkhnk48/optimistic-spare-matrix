#include <stdio.h>
#include <stdlib.h>
#include "Event.c"

enum Side{LEFT, RIGHT};

void add(int type, int idElementInGroup,
                int portID, 
                int endTime,
                int *root,
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
void add(int type, int idElementInGroup,
                int portID, 
                int endTime,
                int *root,
                int arr[384][7]
                )
{
   /* Quy ước các event từ A đến G (tức type = 0..4) sẽ là 
      các event xảy ra trên hosts
      Các biến type < 0 sẽ là event xảy ra trên các edge switch
      Các biến type có bit cuối cùng là 1 sẽ là các event xảy ra trên Core switch
      Các biến type có bit cuối cùng là 0 sẽ là các event xảy ra trên Agg switch
   */
   int idNewNode = 0;
   if(type == A || type == B || type == C || type == H_HOST || type == G)
   {
      idNewNode = idElementInGroup*3 + type;
   }
   else if(type < 0)//Is event of edge switch
   {
      type *= -1;
      idNewNode = 16*3 + idElementInGroup*((4/2)*4 + 3*(4/2)) + type;
   }
   else{
      int isCoreSwitch = type & 1;
      type = type >> 1;
      idNewNode = 16*3 + (4*4/2)*((4/2)*4 + 3*(4/2)) + idElementInGroup*4 + type;
   }
   arr[idNewNode][0] = type;
   arr[idNewNode][1] = idElementInGroup;
   arr[idNewNode][2] = portID;
   arr[idNewNode][3] = endTime;
   arr[idNewNode][4] = -1;
   arr[idNewNode][5] = -1;
   arr[idNewNode][6] = -1;

   if(*root == -1)
   {
      arr[0][0] = type;
      arr[0][1] = idElementInGroup;
      arr[0][2] = portID;
      arr[0][3] = endTime;
      arr[0][4] = -1;
      arr[0][5] = -1;
      arr[0][6] = -1;

      *root = 0;
      return;
   }
   int left = idNewNode, right = idNewNode, temp = -1;
   int end_splay = 0;
   while (end_splay == 0) {
      if(endTime > arr[*root][3])
      {
         temp = arr[*root][6];
         if(temp == -1)
         {
              //cas "zig"
              left->right = t;
              t->father = left;
              right->left = NULL;
              end_splay = 1;
         }
      }
   }

}
void show(int arr[384][7], int root);
void leaf(int arr[384][7], int root, enum Side side);

void show(int arr[384][7], int root)
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

void leaf(int arr[384][7], int root, enum Side side)
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