#include <stdio.h>
#include <stdlib.h>
#include "OptimizationSplay.c"
#include "timing.c"

int main(int argc, char** argv) 
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
         //add(0, 1, 0, 0, &root, arr);
      }
      if(i < 0){
         arr[21][7] += i + N;
      }
   }

   timing(&wc2, &cpuT);
   printf("Time: %f ms\n", (wc2 - wc1)*1000);
   printf("================================\n");
   //show(arr, root); 
   return 0;
}