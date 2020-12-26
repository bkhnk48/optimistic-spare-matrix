#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FatTreeGraph.c"
#include "FatTreeTables.c"

void testBuildingTables(int k, int *addServer, int *addNodes){

}


int main(){
  int k = 4;
  int serverId = 0;
  int delta = 0;
  int p, e, h;
  int numEachPod = k*k/4 + k;
  int numServers = k * k * k / 4;
  int numPodSwitches = k * k;
  int *addServers = malloc((k*k*k/4)*sizeof(int));
  int *addNodes = malloc((k*k*5/4 + k*k*k/4)*sizeof(int));

  #pragma region addresses for servers
  // address for servers
  for(p = 0; p < k; p++) {
      int offset = numEachPod * p;
      for (e = 0; e < k / 2; e++) {
          for (h = 2; h <= k / 2 + 1; h++) {
              serverId = offset + e * k / 2 + h - 2;
              delta = k*p*(1 + ((serverId - k) >> 31));
              
              delta = serverId - delta;
              addServers[delta] = (10 << 24);
              addServers[delta] |= (p << 16);
              addServers[delta] |= (e << 8);
              addServers[delta] |= h;
          }
      }
  }
  #pragma endregion

  #pragma region addresses for switches
  // address for switches
  // address for pod's switches
  for (int p = 0; p < k; p++) {
      int offset = numEachPod * p;
      for (int s = 0; s < k; s++) {
          int switchId = offset + k * k / 4 + s;
          addNodes[switchId] = (10 << 24) | (p << 16) | (s << 8) | 1;
      }
  }
  // address for core switches
  for (int j = 1; j <= k / 2; j++) {
      for (int i = 1; i <= k / 2; i++) {
          int offset = numPodSwitches + numServers;
          int switchId = offset + (j - 1) * k / 2 + i - 1;
          //printf("Switch iD of Core = %d\n", switchId);
          addNodes[switchId] = (10 << 24) | (k << 16) | (j << 8) | i;
      }
  }

  
  #pragma endregion


  for(p = 0; p < k*k*k/4; p++){
    printf("address of server %d = %d, its IPv4 = ", 
              p, addServers[p]);
    printIPv4(getIPv4OfHost(p, k)); printf("\n");
    assert(addServers[p] == getIPv4OfHost(p, k));
    assert(p == getIndexOfHost(addServers[p], k));
    assert(HOST == typeOfNode(getIPv4OfHost(p, k), k));
  }

  int node; int index;
  for(node = 0; node < k*k*k/4 + 5*k*k/4; node++){
    if(node < k*k*k/4 + k*k)
    {
      int pod = node / (k*k/4 + k);
      if(node >= pod*(k*k/4 + k) + (k*k/4)){
        index = node - pod*(k*k/4) - (k*k/4);
        
         
        //        node, addNodes[node], index, );
        assert(addNodes[node] == getIPv4OfSwitch(index, k));
        assert(index == getIndexOfSwitch(addNodes[node], k));
        int indexInPod = node % (k*k/4 + k);
        if(indexInPod >= k*k/4 && indexInPod < ((k*k/4) + k/2)){
          assert(EDGE_SWITCH == typeOfNode(getIPv4OfSwitch(index, k), k));
          printf("address of EDGE switch %d = ", node);
        }
        else{
          assert(AGG_SWITCH == typeOfNode(getIPv4OfSwitch(index, k), k));
          printf("address of AGG switch %d = ", node);
        }
        printIPv4(addNodes[node]);
        printf(" its IPv4 (index = %d) = ", index);
        printIPv4(getIPv4OfSwitch(index, k));
        printf("\n");
      }
    }
    else{
      index = node - k*k*k/4;
      //printf("address of core switch %d = %d, its IPv4 (index = %d) = %d\n", 
      //        node, addNodes[node], index, getIPv4OfSwitch(index, k));
      printf("address of CORE switch %d = ", node);
      printIPv4(addNodes[node]);
      printf(" its IPv4 (index = %d) = ", index);
      printIPv4(getIPv4OfSwitch(index, k));
      printf("\n"), 
      assert(addNodes[node] == getIPv4OfSwitch(index, k));
      assert(index == getIndexOfSwitch(addNodes[node], k));
      assert(CORE_SWITCH == typeOfNode(getIPv4OfSwitch(index, k), k));
    }
  }
  return 0;
}