#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FatTreeGraph.c"
#include "FatTreeTables.c"

void testBuildingTables(int k, int *addServer, int *addNodes){
  printf("Test the routing tables of all switches\n");
  printf("=======================================\n");
  Tables *tablesOfSwitches = malloc(sizeof(Tables));
  buildTables(tablesOfSwitches, k);
  //Ta se kiem tra viec dinh tuyen co thanh cong hay khong?
  //(a) Neu nut edge nhan duoc goi tin yeu cau di den host cung subnet => chuyen sang cac cong [0..k/2-1]
  //      va host dich se la lang gieng cua nut edge
  //        Con neu nut edge nhan duoc goi tin yeu cau di den host khong cung subnet => chuyen
  //        sang cac cong [k/2..k/2 - 1] de den cac agg
  //(b) Neu nut agg nhan duoc goi tin yeu cau di den host cung pod (voi agg)
  //      => chuyen sang cac cong [0..k/2- 1] de den cac edge
  //        Con neu nut agg nhan duoc goi tin yeu cau di den host khong cung pod => chuyen sang
  //        cac cong [k/2..k/2 - 1] de den cac core
  //(c) Neu nut core nhan duoc cac goi tin di den cac host => chuyen den agg co cung
  //      pod voi host do
  int i = 0, j = 0;
  for(i = 0; i < k*k*5/4; i++){
    if(i < k*k){
      #pragma region Routing at Edge switch
      if((i % k) >= 0 && (i %k) <= (k/2 - 1)){
        for(j = 0; j < k*k*k/4; j++){
          int destIP = getIPv4OfHost(j, k);
          int edgeIP = getIPv4OfSwitch(i, k);
          int podOfDest = (destIP << 8) >> 24;
          int subnetOfDest = (destIP << 16) >> 24;
          int podOfEdge = (edgeIP << 8) >> 24;
          int subnetOfEdge = (edgeIP << 16) >> 24;

          if(subnetOfDest != subnetOfEdge 
                || podOfDest != podOfEdge){
            int suffix = destIP & 255;
            suffix -= 2;
            int nextIP = tablesOfSwitches->tables[i].suffixTable[suffix];
            //printf("Through "); 
            //printIPv4(edgeIP);
            //printf(", packet to "); printIPv4(destIP); 
            assert(typeOfNode(nextIP, k) == AGG_SWITCH);
            //printf(" will be next to AGG SWITCH\n");
          }
        }
      }
      #pragma endregion
      else{
        #pragma region Routing at Agg switch
        for(j = 0; j < k*k*k/4; j++){
          int destIP = getIPv4OfHost(j, k);
          int aggIP = getIPv4OfSwitch(i, k);
          int podOfDest = (destIP << 8) >> 24;
          int subnetOfDest = (destIP << 16) >> 24;
          int podOfAgg = (aggIP << 8) >> 24;
          int suffix = destIP & 255; 
          suffix -= 2;
          
          if(podOfDest == podOfAgg){
          //agg nhan duoc goi tin yeu cau di den host cung pod (voi agg)
            int nextIP = tablesOfSwitches->tables[i].prefixTable[subnetOfDest];
            //printf(" agg ip = "); printIPv4(aggIP); 
            //printf(" dest IP = "); printIPv4(destIP); printf(" ");
            //printIPv4(nextIP); printf("\n");
            assert(typeOfNode(nextIP, k) == EDGE_SWITCH);
          }
          else{
            int nextIP = tablesOfSwitches->tables[i].suffixTable[suffix];
            assert(typeOfNode(nextIP, k) == CORE_SWITCH);
          }
        }
        #pragma endregion
      }
    }
    else{
      #pragma region routing at core switch
      for(j = 0; j < k*k*k/4; j++){
        int destIP = getIPv4OfHost(j, k);
        int podOfDest = (destIP << 8) >> 24;
        int nextIP = tablesOfSwitches->tables[i].prefixTable[podOfDest];
        int podOfAgg = (nextIP << 8) >> 24;
        //printf("pod of dest = %d, pod of agg = %d\n", podOfDest, podOfAgg);
        assert(podOfDest == podOfAgg);
        assert(typeOfNode(nextIP, k) == AGG_SWITCH);
      }
      #pragma endregion
    }
    
  }
}

void testPath(int k, int ipOfHost, int ipOfDest, Tables *tablesOfSwitches){

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
  
  testBuildingTables(k, addServers, addNodes);
  return 0;
}
