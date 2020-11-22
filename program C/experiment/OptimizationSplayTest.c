#include <stdio.h>
#include <stdlib.h>
#include "OptimizationSplay.c"
#include "timing.c"
#include <locale.h>


int main(int argc, char** argv) 
{
   double wc1 = 0, wc2 = 0, cpuT = 0;
   int first = -1;
   int defaultSec = 70;
   int defaultBias = 1;

   if(argc >= 2)
   {
      defaultSec = atoi(argv[1]);
      if(argc >= 3)
        defaultBias = atoi(argv[2]);
   }
   unsigned long currentTime = 0;
   unsigned long endTime = defaultSec*((unsigned long)(1000*1000));

   unsigned long arr[20250][7];//20250 = 3*(k*k*k/4) as k = 30
   unsigned long i;
   int j, N, root = -1;
   for(i = 0; i < 20250L; i++)
   {
      for(j = 0; j < 7; j++)
      {
         arr[i][j] = -1L;
      }
   }
   setlocale(LC_NUMERIC, "");
   
   printf("Simulation time is %ld (s)\n", endTime / (1000*1000));
   unsigned long mem = mem_avail();
   printf("Free memory Available = %'ld\n", mem / (1024*1024));

   printf("Start Simulating ......\n");
   unsigned long count = 0;
   //char* p = malloc(1 * 1024 * 1024 * 1024);
   timing(&wc1, &cpuT);

   root = -1;


   for(i = 0; i < 6750L; i++)
   {
      add(0, i, 0, i, &root, arr
                              );
   }
   removeFirst(&first, &root, arr
                              );
      
   unsigned long ongoingTime = arr[first][3];
   while(currentTime <= endTime && ongoingTime != -1)
   {
      
      //for (int i = 0; i < 1024; ++i) {
      //   p[i * 1024 * 1024] = currentTime;    
      //}
      if(ongoingTime == currentTime)
      {
         count++;
         
         int type = arr[first][0];
         i = arr[first][1];
         /*printf("%d)Event type = %d at %d with endTime = %d. The index is %d\n"
               , count, type, i, arr[first][3], first
               );*/
         arr[first][3] = -1L;
         arr[first][4] = -1L;
         arr[first][5] = -1L;
         arr[first][6] = -1L;
         

         if(type == A)
         {
            add(A, i, 0, currentTime + 10000, &root, arr
                        );
            add(B, i, 0, currentTime +  defaultBias*13
                              , &root, arr
                        );
         }
         else if(type == B){
            add(C, i, 0, currentTime +  defaultBias*33
                              , &root, arr
                        );
         }
      }
      ongoingTime = -1;
      removeFirst(&first, &root, arr
               );
      
      currentTime = arr[first][3];
      ongoingTime = arr[first][3];
   }
   
   //printf("Stop Simulating...\n");
   timing(&wc2, &cpuT);
   printf("Time: %'f ms with count = %'ld\n", (wc2 - wc1)*1000, count);
   printf("================================\n");


   return 0;
}

