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
   int endTime = 1000*1000;

   int arr[20250][7];
   int i, j, N, root = -1;
   for(i = 0; i < 20250; i++)
   {
      for(j = 0; j < 7; j++)
      {
         arr[i][j] = -1;
      }
   }

   int count = 0;
   timing(&wc1, &cpuT);

   root = -1;


   for(i = 0; i < 6750; i++)
   {
      add(0, i, 0, 0, &root, arr);
   }
   removeFirst(&first, &root, arr);
      
   int ongoingTime = arr[first][3];
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
         else if(type == C){
            //add(-D, i / 2, i % 2, currentTime + 3000,        &root, arr);
         }
         else if(type == -D)
         {
            int port = arr[first][2];
            
            add(-E, i, ((port + 1) % 4), currentTime + 100, &root, arr);
            add(H_HOST, i*2 + port, 0,   currentTime + 101, &root, arr);
         }
         else if(type == -E)
         {
            int port = arr[first][2];
            add(-F, i, port,             currentTime + 100, &root, arr);
         }
         else if(type == -F)
         {
            int port = arr[first][2];
            if(port < 2)
            {
               add(G, i*2 + port, port,             currentTime + 3000, &root, arr);
            }
            else{
               int DinAgg = D<<1;
               add(DinAgg, i, port % 2,                  currentTime + 100, &root, arr);
            }
         }
         else if(type == H_HOST)
         {
            
         }
      }
      ongoingTime = -1;
      removeFirst(&first, &root, arr);
      
      currentTime = arr[first][3];
      ongoingTime = arr[first][3];
   }
   

   timing(&wc2, &cpuT);
   printf("Time: %f ms with count = %d\n", (wc2 - wc1)*1000, count);
   printf("================================\n");
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