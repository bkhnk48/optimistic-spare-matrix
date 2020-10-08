#include <stdio.h>
#include <stdlib.h>
#include "OptimizationSplay.c"
#include "timing.c"

void testGeneratingEventA();

int main(int argc, char** argv) 
{
   double wc1 = 0, wc2 = 0, cpuT = 0;
   int first = -1;
   int currentTime = 0;
   int endTime = 1000*1000*1000;

   int arr[384][7];
   int i, j, N, root = -1;
   for(i = 0; i < 384; i++)
   {
      for(j = 0; j < 7; j++)
      {
         arr[i][j] = -1;
      }
   }

   timing(&wc1, &cpuT);

   root = -1;


   for(i = 0; i < 16; i++)
   {
      add(0, i, 0, 0, &root, arr);
   }
   removeFirst(&first, &root, arr);
      
   int ongoingTime = arr[first][3];
   while(currentTime <= endTime && ongoingTime != -1)
   {
      if(ongoingTime == currentTime)
      {
         int type = arr[first][0];
         i = arr[first][1];
         arr[first][3] = -1;
         if(type == A)
         {
            add(A, i, 0, currentTime + 10000, &root, arr);
            add(B, i, 0, currentTime        , &root, arr);
         }
         else if(type == B){
            add(C, i, 0, currentTime        , &root, arr);
         }
         else if(type == C){
            add(-D, i / 2, i % 2, currentTime + 300,        &root, arr);
         }
         
      }
      ongoingTime = -1;
      removeFirst(&first, &root, arr);
      currentTime = arr[first][3];
      ongoingTime = arr[first][3];
      //printf("Current time = %d as first event %d\n", currentTime, first);
      //printf("while event 42 has endTime = %d\n", arr[42][3]);
   }
   /*if(i < 0){
      arr[21][7] += i + N;
   }*/
   

   timing(&wc2, &cpuT);
   printf("Time: %f ms\n", (wc2 - wc1)*1000);
   printf("================================\n");
   //show(arr, root); 
   return 0;
}

void testGeneratingEventA()
{
   double wc1 = 0, wc2 = 0, cpuT = 0;


   int arr[384][7];
   int i, j, N, root = -1;
   for(i = 0; i < 384; i++)
   {
      for(j = 0; j < 7; j++)
      {
         arr[i][j] = -1;
      }
   }

   timing(&wc1, &cpuT);

   for(N = 0; N < 1000; N++)
   {
      
      root = -1;

      for(i = 0; i < 16; i++)
      {
         add(0, i, 0, ((i + N) % 2), &root, arr);
         
      }
      if(i < 0){
         arr[21][7] += i + N;
      }
   }

   timing(&wc2, &cpuT);
   printf("Time: %f ms\n", (wc2 - wc1)*1000);
   printf("================================\n");
}