#include <stdio.h>
#include <stdlib.h>
#include "OptimizationSplay.c"
#include "timing.c"

void testGeneratingEventA();

int main(int argc, char** argv) 
{
   double wc1 = 0, wc2 = 0, cpuT = 0;
   int first = -1;
   unsigned long currentTime = 0;
   unsigned long endTime = 1*((unsigned long)(1000*1000));

   unsigned long arr[20250][7];//20250 = 3*(k*k*k/4) as k = 30
   int i, j, N, root = -1;
   for(i = 0; i < 20250; i++)
   {
      for(j = 0; j < 7; j++)
      {
         arr[i][j] = -1;
      }
   }

   unsigned long count = 0;
   timing(&wc1, &cpuT);

   root = -1;


   for(i = 0; i < 6750; i++)
   {
      add(0, i, 0, 0, &root, arr);
   }
   removeFirst(&first, &root, arr);
      
   unsigned long ongoingTime = arr[first][3];
   while(currentTime <= endTime && ongoingTime != -1)
   {
      if(ongoingTime == currentTime)
      {
         count++;
         
         int type = arr[first][0];
         i = arr[first][1];
         /*printf("%d)Event type = %d at %d with endTime = %d. The index is %d\n"
               , count, type, i, arr[first][3], first
               );*/
         arr[first][3] = -1;
         arr[first][4] = -1;
         arr[first][5] = -1;
         arr[first][6] = -1;
         

         if(type == A)
         {
            add(A, i, 0, currentTime + 10000, &root, arr);
            add(B, i, 0, currentTime        , &root, arr);
         }
         else if(type == B){
            add(C, i, 0, currentTime        , &root, arr);
         }
      }
      ongoingTime = -1;
      removeFirst(&first, &root, arr);
      
      currentTime = arr[first][3];
      ongoingTime = arr[first][3];
   }
   

   timing(&wc2, &cpuT);
   printf("Time: %f ms with count = %ld\n", (wc2 - wc1)*1000, count);
   printf("================================\n");


   return 0;
}

