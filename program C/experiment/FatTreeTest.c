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

void testPath(int k, Tables *tablesOfSwitches){
  int i, j;
  int nextIP;
  RoutingTable table;
  int index;
  
  for(i = 0; i < k*k*k/4; i++){
    for(j = 0; j < k*k*k/4; j++){
      if(i != j){
        int count = 0;
        int srcIP = getIPv4OfHost(i, k);
        int destIP = getIPv4OfHost(j, k);
        int podOfSrc = (srcIP >> 16) & 255;
        int podOfDst = (destIP >> 16) & 255;
        int subnetOfSrc = (srcIP >> 8) & 255;
        int subnetOfDest = (destIP >> 8) & 255;
        #pragma region src and dst are in the same subnet
        if(subnetOfDest == subnetOfSrc && podOfDst == podOfSrc){
          count = 1;
          nextIP = next(srcIP, srcIP, destIP, k, NULL);
          while (nextIP != destIP)
          {
            index = getIndexOfSwitch(nextIP, k);
            count--;
            table = tablesOfSwitches->tables[index];
            nextIP = next(srcIP, nextIP, destIP, k, &table);
          }
          assert(count == 0);
        }
        #pragma endregion
        else if(podOfDst == podOfSrc){
          #pragma region src and dst are in the same pod but different subnets
          count = 3;
          
          nextIP = next(srcIP, srcIP, destIP, k, NULL);
          while (nextIP != destIP)
          {
            index = getIndexOfSwitch(nextIP, k);
            count--;
            table = tablesOfSwitches->tables[index];
            nextIP = next(srcIP, nextIP, destIP, k, &table);
          }
          assert(count == 0);
          #pragma endregion
        }
        else{
          #pragma region src and dst are in different pods
          count = 5; 
          nextIP = next(srcIP, srcIP, destIP, k, NULL);
          
          while (nextIP != destIP)
          {
            index = getIndexOfSwitch(nextIP, k);
            count--;
            table = tablesOfSwitches->tables[index];
            nextIP = next(srcIP, nextIP, destIP, k, &table);
          }
          assert(count == 0);
          #pragma endregion
        } 
        
      }
    }
  }
}

void testNeighbors(int k){
  int i = 0, p = 0;
  for(i = 0; i < k*k*k/4; i++){
    int ipOfHost = getIPv4OfHost(i, k);
    int edgeIP = getNeighborIP(ipOfHost, HOST, 0, k);
    int indexOfEdge = getIndexOfSwitch(edgeIP, k);
    int subnetOfHost = (i % (k*k/4))/ (k/2);
    int podOfHost = i / (k*k/4);
    int subnet = subnetOfHost;
    int pod = podOfHost;
    assert(indexOfEdge == (subnetOfHost + podOfHost*k));
    for(p = k/2; p < k; p++){
      int aggIP = getNeighborIP(edgeIP, EDGE_SWITCH, p, k);
      int indexOfAgg = getIndexOfSwitch(aggIP, k);
      //printf("indexOfAgg = %d\n", indexOfAgg);
      assert(indexOfAgg == pod*k + p);
    }
  }
}

void testHash(int k){
  int size = 6*k*k*k;
  int **arr = malloc(sizeof * arr * size);
  int i, j, ev, port;
  for(i = 0; i < size; i++){
    arr[i] = malloc(sizeof * arr[i] * 4);
    arr[i][0] = 0; //id in group
    arr[i][1] = 0; //type of Node
    arr[i][2] = 0; //port ID
    arr[i][3] = 0; //type of Event
  }
  int numOfHosts = k*k*k/4;
  int numOfSwitches = 5*k*k/4;
  int index = 0;
  int eventsOfHost[5] = {A, B, C, H_HOST, G};
  int eventsDEF[3] = {D, E, F};
  int eventsDEFH[4] = {D, E, F, H};
  for(i = 0; i < numOfHosts + numOfSwitches; i++){
    if(i < numOfHosts){
      for(j = 0; j < 5; j++){
        index = hash(i, HOST, 0, eventsOfHost[j], k);
        arr[index][0] = i;
        arr[index][1] = HOST; //type of Node
        arr[index][2] = 0; //port ID
        arr[index][3] = eventsOfHost[j]; //type of Event
      }
    }
  
    else if(i < numOfHosts + k*k/2){
      for(j = 0; j < k/2; j++){
        for(ev = 0; ev < 3; ev ++){
          index = hash(i - numOfHosts, EDGE_SWITCH, j, 
                        eventsDEF[ev],
                        k);
          arr[index][0] = i - numOfHosts;
          arr[index][1] = EDGE_SWITCH; //type of Node
          arr[index][2] = j; //port ID
          arr[index][3] = eventsDEF[ev]; //type of Event
        }
      }
      for(j = k/2; j < k; j++){
        for(ev = 0; ev < 4; ev ++){
          index = hash(i - numOfHosts, EDGE_SWITCH, j, 
                        eventsDEFH[ev],
                        k);
          arr[index][0] = i - numOfHosts;
          arr[index][1] = EDGE_SWITCH; //type of Node
          arr[index][2] = j; //port ID
          arr[index][3] = eventsDEFH[ev]; //type of Event
        }
      }
    }
    else if(i < numOfHosts + k*k){
      for(j = 0; j < k; j++){
        index = hash(i - numOfHosts, AGG_SWITCH, j, D, k);
        index = hash(i - numOfHosts, AGG_SWITCH, j, E, k);
        index = hash(i - numOfHosts, AGG_SWITCH, j, F, k);
        index = hash(i - numOfHosts, AGG_SWITCH, j, H, k);
      }
    }
    else{
      for(j = 0; j < k; j++){
        index = hash(i - numOfHosts, CORE_SWITCH, j, D, k);
        index = hash(i - numOfHosts, CORE_SWITCH, j, E, k);
        index = hash(i - numOfHosts, CORE_SWITCH, j, F, k);
        index = hash(i - numOfHosts, CORE_SWITCH, j, H, k);
      }
    }
  }

  for(i = 1; i < numOfHosts; i++){
    for(j = 5*(i - 1) + 1; j < 5*i; j++){
      assert(arr[j][0] == arr[j - 1][0]);//trung id 
      assert(arr[j][3] == arr[j - 1][3] + 1);
      //cac event khac nhau: A, B, C, H_HOST, G
    }
    assert(arr[i*5][1] == arr[5*(i-1)][1]);//trung typeOfNode
  }
  
  for(i = numOfHosts; i < numOfHosts + k*k/2; i++){
    index = (i - numOfHosts)*7*k/2 + 5*numOfHosts;
    if(i - numOfHosts >= 1){
      assert(arr[index - 1][0] == i - numOfHosts - 1);
    }
    for(j = 0; j < k/2; j++){
      assert(arr[index + j*3 + (D - D)][0] == i - numOfHosts);//trung id 
      assert(arr[index + j*3 + (E - D)][0] == i - numOfHosts);//trung id 
      assert(arr[index + j*3 + (F - D)][0] == i - numOfHosts);//trung id 

      assert(arr[index + j*3 + (D - D)][1] == EDGE_SWITCH);//edge switch
      assert(arr[index + j*3 + (E - D)][1] == EDGE_SWITCH);//edge switch 
      assert(arr[index + j*3 + (F - D)][1] == EDGE_SWITCH);//edge switch

      assert(arr[index + j*3 + (D - D)][2] == j);//port
      assert(arr[index + j*3 + (E - D)][2] == j);//port
      assert(arr[index + j*3 + (F - D)][2] == j);//port

      assert(arr[index + j*3 + (D - D)][3] == D);//event
      assert(arr[index + j*3 + (E - D)][3] == E);//event
      assert(arr[index + j*3 + (F - D)][3] == F);//event
    }
    //assert(arr[i*5][1] == arr[5*(i-1)][1]);//trung typeOfNode
    index += 3*k/2;
    for(j = k/2; j < k; j++){
      port = j - k/2;
      assert(arr[index + port*4 + (D - D)][0] == i - numOfHosts);//trung id 
      assert(arr[index + port*4 + (E - D)][0] == i - numOfHosts);//trung id 
      assert(arr[index + port*4 + (F - D)][0] == i - numOfHosts);//trung id 
      assert(arr[index + port*4 + (H - D)][0] == i - numOfHosts);//trung id 

      assert(arr[index + port*4 + (D - D)][1] == EDGE_SWITCH);//edge switch
      assert(arr[index + port*4 + (E - D)][1] == EDGE_SWITCH);//edge switch 
      assert(arr[index + port*4 + (F - D)][1] == EDGE_SWITCH);//edge switch
      assert(arr[index + port*4 + (H - D)][1] == EDGE_SWITCH);//edge switch

      assert(arr[index + port*4 + (D - D)][2] == j);//port
      assert(arr[index + port*4 + (E - D)][2] == j);//port
      assert(arr[index + port*4 + (F - D)][2] == j);//port
      assert(arr[index + port*4 + (H - D)][2] == j);//port

      assert(arr[index + port*4 + (D - D)][3] == D);//event
      assert(arr[index + port*4 + (E - D)][3] == E);//event
      assert(arr[index + port*4 + (F - D)][3] == F);//event
      assert(arr[index + port*4 + (H - D)][3] == H);//event
    }
  }
}

int main(){
  int k = 4;
  int serverId = 0;
  int delta = 0;
  int i, p, e, h;
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
        printf("(original = %d \n", addNodes[node]);
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
      printf("(original = %d \n", addNodes[node]);
      assert(addNodes[node] == getIPv4OfSwitch(index, k));
      assert(index == getIndexOfSwitch(addNodes[node], k));
      assert(CORE_SWITCH == typeOfNode(getIPv4OfSwitch(index, k), k));
    }
  }
  
  testNeighbors(k);
  testBuildingTables(k, addServers, addNodes);

  Tables *tablesOfSwitches = malloc(sizeof(Tables));
  buildTables(tablesOfSwitches, k);
  testPath(k, tablesOfSwitches);

  BufferSwitch *bufferSwitches = initBufferSwitches(k*k*5/4, k);

  NetworkNode *allNodes = initNetworkNodes(k*k*k/4, 5*k*k/4, k);
  printf("assert network type\n");
  #pragma region Test type of network nodes
  for(i = 0; i < (k*k*k/4) + (5*k*k/4); i++){
    if(i < (k*k*k/4) + (k*k)){
      int pod = i / (k + k*k/4);
      int subIndex = (i - (k*k*k/4)) % (k);
      if(i < (k*k*k/4)){
        assert(allNodes[i].type == HOST);
        assert(typeOfNode(allNodes[i].ipv4, k) == HOST);
      }
      else if(subIndex < k/2){
        assert(allNodes[i].type == EDGE_SWITCH);
        assert(typeOfNode(allNodes[i].ipv4, k) == EDGE_SWITCH);
      }
      else{
        assert(allNodes[i].type == AGG_SWITCH);
        assert(typeOfNode(allNodes[i].ipv4, k) == AGG_SWITCH);
      }
    }
    else{
      assert(allNodes[i].type == CORE_SWITCH);
      assert(typeOfNode(allNodes[i].ipv4, k) == CORE_SWITCH);
    }
  }
  #pragma endregion
  
  testHash(k);
  return 0;
}
