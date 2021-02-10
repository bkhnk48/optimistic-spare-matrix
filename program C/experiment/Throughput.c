#include <stdio.h>
#include <stdlib.h>

unsigned long calculateThroughput(unsigned long **receivedPackets, int PACKET_SIZE, int STEP, int numOfHosts, double INTERVAL_BANDWIDTH){
   int i, j;
   unsigned long total = 0;
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
      printf("|%d)%ld", i, counter[i]);
      total += counter[i];
   }
   printf("\n");

   for(i = 0; i < STEP; i++){
      thpt[i] = counter[i]*PACKET_SIZE*100/INTERVAL_BANDWIDTH;
      printf("|%.0lf%%", thpt[i]);
   }
   printf("\n");
   free(thpt);
   free(counter);
   return total;
}