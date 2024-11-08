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
            assert(typeOfNode(nextIP, k) == AGG_SWITCH);
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
  unsigned long portAndNextIP;
  
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
          portAndNextIP = next(srcIP, srcIP, destIP, k, NULL);
          nextIP = (int)portAndNextIP;
          while (nextIP != destIP)
          {
            index = getIndexOfSwitch(nextIP, k);
            count--;
            table = tablesOfSwitches->tables[index];
            portAndNextIP = next(srcIP, nextIP, destIP, k, &table);
            nextIP = (int)portAndNextIP;
          }
          assert(count == 0);
        }
        #pragma endregion
        else if(podOfDst == podOfSrc){
          #pragma region src and dst are in the same pod but different subnets
          count = 3;
          
          portAndNextIP = next(srcIP, srcIP, destIP, k, NULL);
          nextIP = (int)portAndNextIP;
          while (nextIP != destIP)
          {
            index = getIndexOfSwitch(nextIP, k);
            count--;
            table = tablesOfSwitches->tables[index];
            portAndNextIP = next(srcIP, nextIP, destIP, k, &table);
            nextIP = (int)portAndNextIP;
          }
          assert(count == 0);
          #pragma endregion
        }
        else{
          #pragma region src and dst are in different pods
          count = 5; 
          portAndNextIP = next(srcIP, srcIP, destIP, k, NULL);
          nextIP = (int)portAndNextIP;
          if(i == 2 && j == 9)
          {
            //printf("DEBUG %d\n", __LINE__);
          }
          while (nextIP != destIP)
          {
            index = getIndexOfSwitch(nextIP, k);
            count--;
            #pragma region test the case of paper
            if(i == 2 && j == 9)
            {
              if(count == 4)
                assert(index == 1);
              if(count == 3)
                assert(index == 2);
              if(count == 2)
                assert(index == 17);
              if(count == 1)
                assert(index == 10);
              if(count == 0)
                assert(index == 8);
            }
            #pragma endregion 
            table = tablesOfSwitches->tables[index];
            portAndNextIP = next(srcIP, nextIP, destIP, k, &table);
            nextIP = (int)portAndNextIP;
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
  int size = 25*k*k*k/4;
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
  //int eventsDEF[3] = {D, E, F, H};
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
  
    /*else if(i < numOfHosts + k*k/2){
      
    }*/
    else if(i < numOfHosts + k*k){
      int type = (i - numOfHosts) % k;
      if(type < k/2){
        for(j = 0; j < k/2; j++){
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
      else{
        for(j = 0; j < k; j++){
          for(ev = 0; ev < 4; ev ++){
            index = hash(i - numOfHosts, AGG_SWITCH, j, 
                          eventsDEFH[ev],
                          k);
            arr[index][0] = i - numOfHosts;
            arr[index][1] = AGG_SWITCH; //type of Node
            arr[index][2] = j; //port ID
            arr[index][3] = eventsDEFH[ev]; //type of Event
          }
        }
      }
    }
    else{
      for(j = 0; j < k; j++){
        for(ev = 0; ev < 4; ev ++){
            index = hash(i - numOfHosts, CORE_SWITCH, j, 
                          eventsDEFH[ev],
                          k);
            arr[index][0] = i - numOfHosts;
            arr[index][1] = CORE_SWITCH; //type of Node
            arr[index][2] = j; //port ID
            arr[index][3] = eventsDEFH[ev]; //type of Event
        }
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
  int countEdge = 0, currEdge = -1, countAgg = 0, currAgg = -1;
  int countPortOfEdge = 0, currPortOfEdge = -1;
  int countEventOfEdge = 0, currEventOfEdge = -1;

  int countPort = 0, currPort = -1;
  int countEvent = 0, currEvent = -1;
  
  int minIndex = hash(0, EDGE_SWITCH, 0, D, k);
  int maxIndex = hash(k*k - 1, AGG_SWITCH, k - 1, H, k);
  for(i = minIndex; i <= maxIndex; i++){
    if(arr[i][1] == EDGE_SWITCH){
      if(currEdge != arr[i][0]){
        countEdge++;
        currEdge = arr[i][0];
        if(countEdge > 1){
          assert(countEventOfEdge == 8*k/2);
          assert(countPortOfEdge == k);
        }
        countEventOfEdge = 1;
        countPortOfEdge = 1;
        currEventOfEdge = arr[i][3];
        currPortOfEdge = arr[i][2];
      }
      else{
        if(currEventOfEdge != arr[i][3]){
          countEventOfEdge++;
          currEventOfEdge = arr[i][3];
        }
        if(currPortOfEdge != arr[i][2]){
          countPortOfEdge++;
          currPortOfEdge = arr[i][2];
        }
      }
    }
  
    if(arr[i][1] == AGG_SWITCH){
      if(currAgg != arr[i][0]){
        countAgg++;
        currAgg = arr[i][0];
        if(countAgg > 1){
          assert(countEvent == 4*k);
          assert(countPort == k);
        }
        countEvent = 1;
        countPort = 1;
        currEvent = arr[i][3];
        currPort = arr[i][2];
      }
      else{
        if(currEvent != arr[i][3]){
          countEvent++;
          currEvent = arr[i][3];
        }
        if(currPort != arr[i][2]){
          countPort++;
          currPort = arr[i][2];
        }
      }
    }
  }
  assert(countEdge == k*k/2);
  assert(countAgg == k*k/2);

  assert(countEdge * 8*k/2 + countAgg * 4 * k 
            == maxIndex - minIndex + 1);

  minIndex = hash(k*k, CORE_SWITCH, 0, D, k);
  maxIndex = hash(5*k*k/4 - 1, CORE_SWITCH, k - 1, H, k);

  int currCore = -1, countCore = 0;
  countEvent = 0; countPort = 0;
  for(i = minIndex; i <= maxIndex; i++){
    if(arr[i][1] == CORE_SWITCH){
      if(currCore != arr[i][0]){
        countCore++;
        currCore = arr[i][0];
        if(countCore > 1){
          assert(countEvent == 4*k);
          assert(countPort == k);
        }
        countEvent = 1;
        countPort = 1;
        currEvent = arr[i][3];
        currPort = arr[i][2];
      }
      else{
        if(currEvent != arr[i][3]){
          countEvent++;
          currEvent = arr[i][3];
        }
        if(currPort != arr[i][2]){
          countPort++;
          currPort = arr[i][2];
        }
      }
    }
  }
  assert(countCore == k*k/4);
}

void testCoreInPath(Tables *tablesOfSwitches){
  //this func has implicit variable k = 4
  int i, j;
  int nextIP;
  RoutingTable table;
  int index;
  unsigned long portAndNextIP;
  int **cores = NULL;
  cores = malloc(sizeof * cores * 4);
  for(i = 0; i < 4; i++){
    cores[i] = malloc(sizeof * cores[i] * 7);
    cores[i][0] = 0;
    for(j = 1; j < 7; j++)
      cores[i][j] = -1;
  }
  
  for(i = 0; i < 16; i++){
    int srcIP = getIPv4OfHost(i, 4);
    int podOfSrc = (srcIP >> 16) & 255;
    
    for(j = 0; j < 16; j++){
      if(i != j){
        int count = 0;
        int destIP = getIPv4OfHost(j, 4);
        int podOfDst = (destIP >> 16) & 255;
        if(podOfSrc != podOfDst){
          int subnetOfSrc = (srcIP >> 8) & 255;
          int subnetOfDest = (destIP >> 8) & 255;
          portAndNextIP = next(srcIP, srcIP, destIP, 4, NULL);
          nextIP = (int)portAndNextIP;
          while (nextIP != destIP)
          {
            index = getIndexOfSwitch(nextIP, 4);
            if(index >= 16){
              cores[index-16][cores[index-16][0] + 1] = j;
              cores[index-16][0]++;
            }
            table = tablesOfSwitches->tables[index];
            portAndNextIP = next(srcIP, nextIP, destIP, 4, &table);
            nextIP = (int)portAndNextIP;
          }
        }
      }
    }
    
    assert(cores[0][0] + cores[1][0] + cores[2][0] + cores[3][0] == 12);
    assert(cores[0][0]*cores[1][0] == 0);
    assert(cores[2][0]*cores[3][0] == 0);
    assert(cores[0][0] + cores[1][0] == cores[2][0] + cores[3][0]);
    assert(cores[0][0] == cores[2][0]);
    assert(cores[1][0] == cores[3][0]);
    
    int l, k;

    printf("Host %d ", i);
    for(l = 0; l < 4; l++){
      if(cores[l][0] > 0){
        printf("pass core %d to reach: ", l + 16);
        cores[l][0] = 0;
        for(k = 1; k < 7; k++){
          if(cores[l][k] != -1){
            printf(", %d", cores[l][k]);
            cores[l][k] = -1;
          }
        }
      }
      printf("\n");
    }
  }
}


int main(){
  int k = 4;
  int serverId = 0;
  int delta = 0;
  int i, j, p, e, h;
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
  testCoreInPath(tablesOfSwitches);

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
        assert(allNodes[i].indexInGroup == i);
        assert(typeOfNode(allNodes[i].ipv4, k) == HOST);
      }
      else if(subIndex < k/2){
        assert(allNodes[i].type == EDGE_SWITCH);
        assert(typeOfNode(allNodes[i].ipv4, k) == EDGE_SWITCH);
        for(j = 1; j < k/2; j++){
          int host1 = allNodes[i].links[j - 1].nextIndex;
          int host2 = allNodes[i].links[j].nextIndex;
          assert(host1 == host2 - 1);
          assert(host1 / (k/2) == host2 / (k/2));
          assert(host2 / (k*k/4) == 
                        allNodes[i].indexInGroup / k);
          assert(host1 / (k*k/4) == 
                        allNodes[i].indexInGroup / k); 
          assert(allNodes[host1].links[0].nextIndex == 
                      i - numServers);
        }
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
  
  for(i = 0; i < k*k*k/4 - 1; i++){
    if(i / (k/2) == (i + 1)/(k/2)){
      int switch1 = allNodes[i].links[0].nextIndex;
      int switch2 = allNodes[i + 1].links[0].nextIndex;
      assert(switch1 == switch2);
    }
  }
  testHash(k);

  int h_index = hash(0, EDGE_SWITCH, 1, H, 4);
  printf("F index at sw 0, port 1 %d\n", hash(0, EDGE_SWITCH, 1, F, 4));
  int d_index = hash(0, EDGE_SWITCH, 2, D, 4);
  printf("E index at sw 0, port 2 %d\n", hash(0, EDGE_SWITCH, 2, E, 4));
  printf("h_index = %d d_index = %d\n", h_index, d_index);
  assert(h_index != d_index);
  return 0;
}
