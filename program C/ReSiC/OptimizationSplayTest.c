#include <stdio.h>
#include <stdlib.h>
#include "OptimizationSplay.c"

int main(int argc, char** argv) 
{
   int arr[384][7];
   int i, j;
   for(i = 0; i < 384; i++)
   {
      for(j = 0; j < 7; j++)
      {
         arr[i][j] = -1;
      }
   }
   int root = -1;

   add(0, 0, 0, 0, &root, arr);
   show(arr, root); 
   return 0;
}