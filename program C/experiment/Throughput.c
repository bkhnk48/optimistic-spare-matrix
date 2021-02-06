#include <stdio.h>
#include <stdlib.h>

void calculateThroughput(unsigned long **receivedPackets, int PACKET_SIZE, int STEP, int numOfHosts, int BANDWIDTH_HOST){
   int i, j;
   double *thpt = malloc(STEP * sizeof(double));
   unsigned long *counter = malloc(STEP * sizeof(unsigned long));
   for(i = 0; i < STEP; i++){
      thpt[i] = 0.0;
      counter[i] = 0;
   }
   for(i = 0; i < numOfHosts; i++){
      for(j = 0; j < STEP; j++){
         counter[j] += receivedPackets[i][j];
      }
   }
   for(i = 0; i < STEP; i++){
      thpt[i] = (double)counter[i]*PACKET_SIZE*100/BANDWIDTH_HOST;
      printf("|%lf", thpt[i]);
   }
   printf("\n");
   free(thpt);
   free(counter);
}