#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FatTreeGraph.c"

int main(){
  int k = 30;
  int serverId = 0;
  int delta = 0;
  int p, e, h;
  int numEachPod = k*k/4 + k;
  int *addServer = malloc((k*k*k/4)*sizeof(int));

  #pragma region addresses for servers
  // address for servers
  for(p = 0; p < k; p++) {
      int offset = numEachPod * p;
      for (e = 0; e < k / 2; e++) {
          for (h = 2; h <= k / 2 + 1; h++) {
              serverId = offset + e * k / 2 + h - 2;
              delta = k*p*(1 + ((serverId - k) >> 31));
              
              delta = serverId - delta;
              addServer[delta] = (10 << 24);
              addServer[delta] |= (p << 16);
              addServer[delta] |= (e << 8);
              addServer[delta] |= h;
          }
      }
  }
  #pragma endregion

  for(p = 0; p < k*k*k/4; p++){
    printf("address of server %d = %d, its IPv4 = %d\n", 
              p, addServer[p], getIPv4OfHost(p, k));
    assert(addServer[p] == getIPv4OfHost(p, k));
    assert(p == getIndexOfHost(addServer[p], k));
  }
  return 0;
}