#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FatTreeGraph.c"
#include "FatTreeTables.c"
#include "MergeSplay.c"
#include "ControlFlow.c"
#include "timing.c"
#include "TestAPI.c"
#include "Throughput.c"
#include "PairStrategies.c"
#include <limits.h>
#include <locale.h>
#include <math.h>

int main(int argc, char **argv)
{
  double wc1 = 0, wc2 = 0, cpuT = 0;
  unsigned int first = UINT_MAX;
  int defaultSec = 1;
  int defaultBias = 0;
  ProcStatm proc_statm;
  long page_size = sysconf(_SC_PAGESIZE);
  int k = 4;
  int PACKET_SIZE = 96 * 100; //9.6KB
  int SWITCH_CYCLE = 10;
  //int packets_per_sec = 1000;//each sec generated 1000 packets
  int T = 100 * 1000;
  unsigned long BANDWIDTH_HOST = (unsigned long)96 * 1000 * 1000; //96MByte/s
  unsigned long BANDWIDTH_CORE = (unsigned long)10667 * 10 * 1000; //106.67MByte/s
  unsigned long loadingTime1 = ((unsigned long)PACKET_SIZE * 1000 * 1000 * 1000) / BANDWIDTH_HOST; //BANDWIDTH_HOST / PACKET_SIZE ;//+ 13;
  unsigned long loadingTime2 = ((unsigned long)PACKET_SIZE * 1000 * 1000 * 1000) / BANDWIDTH_CORE; //BANDWIDTH_HOST / PACKET_SIZE ;//+ 13;
  
  unsigned long **receivedPkts = NULL;
  int STEP = 100;
  enum PairStrategy stragegy = FORCE_TO_PAIR;
  
  unsigned long currentTime = 0;
  int numOfHosts = k * k * k / 4;
  int defaultNumOfFlows = numOfHosts;
  unsigned long endTime = defaultSec * ((unsigned long)(1000 * 1000 * 1000));
  unsigned long STEP_TIME = endTime / STEP;

  if (argc >= 2)
  {
    k = atoi(argv[1]);
    if (argc > 2)
      defaultSec = atoi(argv[2]);
    if (argc > 3)
      defaultBias = atoi(argv[3]);
  }

  

  unsigned long i, j;
  unsigned int N, root = UINT_MAX;

  arr = malloc(sizeof *arr * (25 * k * k * k/4));
  data = malloc((25 * k * k * k/4) * sizeof(unsigned long));
  for (i = 0; i < 25 * k * k * k/4; i++)
  {
    arr[i] = malloc(sizeof *arr[i] * BUFFER_SIZE);
    for (j = 0; j < BUFFER_SIZE; j++)
    {
      arr[i][j] = UINT_MAX;
    }
    data[i] = 0;
  }

  PairPattern *pairs = NULL;
  pairs = malloc(numOfHosts * sizeof(PairPattern)); 
  forceToPair(pairs, numOfHosts, 2);
  printfPairs(pairs, numOfHosts);

  receivedPkts = malloc(sizeof *receivedPkts * numOfHosts);
  for (i = 0; i < numOfHosts; i++)
  {
    receivedPkts[i] = malloc(sizeof *receivedPkts[i] * (STEP + 1));
    for (j = 0; j < STEP + 1; j++)
    {
      receivedPkts[i][j] = 0;
    }
  }

  Tables *tablesOfSwitches = malloc(sizeof(Tables));
  buildTables(tablesOfSwitches, k);
  BufferHost *bufferHosts = initBufferHosts(k * k * k / 4);
  BufferSwitch *bufferSwitches = initBufferSwitches(k * k * 5 / 4, k);
  assignTypeOfSwitch(bufferSwitches, k);
  NetworkNode *allNodes = initNetworkNodes(k * k * k / 4, 5 * k * k / 4, k);

  setlocale(LC_NUMERIC, "");

  printf("Start Simulating ......\n");
  unsigned long count = 0;
  int nextIP = 0;
  int nextIndex;
  int nextPort = 0;
  int generateEventB, generateEventC, generateEventD;
  int generateEventE, generateEventF, generateEventH;
  int generateEventH_HOST;
  int generateEventG;
  int idNodeInTree = 0;
  int numOfFlows = 0;
  int type;
  int edited = 0, flag = 0;
  
  root = UINT_MAX;

  timing(&wc1, &cpuT);

  for (i = 0; i < numOfHosts; i++) //Only test first hosts in pod
  {
    idNodeInTree = hash(i, HOST, 0, A, k);
    add(A, i, 0, 0, &root, idNodeInTree);
    numOfFlows++;
  }

  removeFirst(&first, &root);

  unsigned long ongoingTime = (unsigned long)arr[first][0] << 32 + arr[first][1];
  while (currentTime <= endTime && ongoingTime != -1)
  {
    if (ongoingTime == currentTime)
    {
      count++;
      #pragma region get value from data array
      type = data[first] & 65535; //type of event
      i = data[first] >> 32;          //idElementInGroup
      arr[first][0] = UINT_MAX;
      arr[first][1] = UINT_MAX;
      arr[first][2] = UINT_MAX;
      arr[first][3] = UINT_MAX;
      arr[first][4] = UINT_MAX;
      #pragma endregion

      #pragma region action of Event type A
      if (type == A)
      {
        allNodes[i].generatedPackets++;
        add(A, i, 0, currentTime + T, &root, first);
        generateEventB = actionA(T, currentTime, &bufferHosts[i]);
        if (generateEventB){
          add(B, i, 0, currentTime + defaultBias * 13, &root, first + 1);
        }
      }
      #pragma endregion

      #pragma region action of Event type B
      else if (type == B)
      {
        generateEventC = actionB(&bufferHosts[i], allNodes[i].links[0].pkt);
        if (generateEventC)
          add(C, i, 0, currentTime + defaultBias * 33, &root, first + 1);
      }
      #pragma endregion

      else if (type == C)
      {
        #pragma region action of Event type C
        nextIndex = allNodes[i].links[0].nextIndex;
        nextPort = allNodes[i].links[0].nextPort;
        generateEventB = 0;
        generateEventD = actionC(&bufferHosts[i],
                                 allNodes[i].links, &generateEventB,
                                 getIPv4OfHost(pairs[i].dst, k), T);
        if (generateEventB)
          add(B, i, 0, currentTime + defaultBias * 13, &root, first - 1);
        if (generateEventD){
          idNodeInTree = hash(nextIndex, EDGE_SWITCH, nextPort, D, k);
          add(D, nextIndex, nextPort, currentTime + loadingTime1, &root, idNodeInTree);
        }
        #pragma endregion
      }
      else if (type == D)
      {
        #pragma region action of Event type D
        int portID = (data[first] >> 16) & MASK_INT;
        Packet *ENB = bufferSwitches[i].ENB[portID];
        int idPrev = allNodes[i + numOfHosts].links[portID].nextIndex;

        int idPrevPort = allNodes[i + numOfHosts].links[portID].nextPort;
        idPrev += (allNodes[i + numOfHosts].type == EDGE_SWITCH && portID < k / 2 ? 0 : numOfHosts);

        Packet *pkt = allNodes[idPrev].links[idPrevPort].pkt;
        //int preLast = bufferSwitches[i].firstLastENBs[portID][1];

        int posInENB = receivePacket(portID, &bufferSwitches[i], currentTime, pkt);

        if (posInENB == bufferSwitches[i].firstLastENBs[portID][0])
        {
          //Packet is ahead of all other ones on ENB
          int nextIP = next(ENB[posInENB].srcIP,
                            allNodes[i + numOfHosts].ipv4,
                            ENB[posInENB].dstIP,
                            k, &(tablesOfSwitches->tables[i]));

          int nextEXB = getEXB_ID(nextIP,
                                  allNodes[i + numOfHosts].type, k);
          //this func has two params:
          // + nextEXB: the port ID of the next EXB
          // + registeredEXB[portID]: the array's element to store the nextEXB
          //additional info: portID - ID of ENB in which outgoing packet
          signEXB_ID(nextEXB, &bufferSwitches[i].registeredEXBs[portID]);

          generateEventE = actionD(portID, nextEXB, &bufferSwitches[i], currentTime);

          if (generateEventE){
            idNodeInTree = hash(i, allNodes[i + numOfHosts].type, nextEXB, E, k);
            add(E, i, nextEXB, currentTime + SWITCH_CYCLE, &root, idNodeInTree);
          }


        }
        
      }
      #pragma endregion
      else if (type == E)
      {
        #pragma region action of Event type E
        int portID = (data[first] >> 16) & MASK_INT;
        int pickUpENB = chooseENB_ID(portID, &bufferSwitches[i], k);
        
        generateEventE = 0;
        generateEventF = 0;
        generateEventH = 0;
        generateEventH_HOST = 0;
        int H_IS_HOST = ((allNodes[i + numOfHosts].type == EDGE_SWITCH && pickUpENB <= (k / 2 - 1)) ? 1 : 0);
        
        int shallFindNewPkt = 0;
        
        shallFindNewPkt = move(pickUpENB, portID, &bufferSwitches[i]);
        
        #pragma region Shift packet in ENB
        Packet *ENB = bufferSwitches[i].ENB[pickUpENB];
        int posInENB = bufferSwitches[i].firstLastENBs[pickUpENB][0];

        if (ENB[posInENB].srcIP != -1 && ENB[posInENB].dstIP != -1 && ENB[posInENB].id != -1 && ENB[posInENB].generatedTime != -1)
        {
          int nextIP = next(ENB[posInENB].srcIP,
                            allNodes[i + numOfHosts].ipv4,
                            ENB[posInENB].dstIP,
                            k, &(tablesOfSwitches->tables[i]));

          int nextEXB = getEXB_ID(nextIP, allNodes[i + numOfHosts].type, k);
          //this func has two params:
          // + nextEXB: the port ID of the next EXB
          // + registeredEXB[portID]: the array's element to store the nextEXB
          //additional info: portID - ID of ENB in which outgoing packet
          signEXB_ID(nextEXB, &bufferSwitches[i].registeredEXBs[pickUpENB]);
        }
        #pragma endregion

        if(shallFindNewPkt){
          findENB_ID(portID, &bufferSwitches[i], currentTime, k);
        }
        int generatedEF = actionE(pickUpENB, portID, &bufferSwitches[i], &allNodes[i + numOfHosts].links[portID]);

        generateEventE = generatedEF & 1;
        generateEventF = (generatedEF & 2) >> 1;
        
        if (generateEventE){
          idNodeInTree = hash(i, allNodes[i + numOfHosts].type, portID, E, k);
          add(E, i, portID, currentTime + SWITCH_CYCLE, &root, idNodeInTree);
        }

        if (generateEventF){
          idNodeInTree = hash(i, allNodes[i + numOfHosts].type, portID, F, k);
          add(F, i, portID, currentTime + SWITCH_CYCLE, &root, idNodeInTree);
        }

        int idPrev = allNodes[i + numOfHosts].links[pickUpENB].nextIndex;
        idNodeInTree = 0;
        if (H_IS_HOST){
          idNodeInTree = hash(idPrev, HOST, 0, H_HOST, k);
          add(H_HOST, idPrev, 0, currentTime + 1, &root, idNodeInTree);
        }
        else
        {
          //generate event H
          int idPrePort = allNodes[i + numOfHosts].links[pickUpENB].nextPort;
          idNodeInTree = hash(idPrev, allNodes[idPrev + numOfHosts].type, idPrePort, H, k);
          add(H, idPrev, idPrePort, currentTime + 1, &root, idNodeInTree);
        }

        #pragma endregion
      }
      else if (type == H_HOST)
      {
        //generateEventB = 0;
        int nextNode = allNodes[i].links[0].nextIndex;
        int nextPort = allNodes[i].links[0].nextPort;
        
        generateEventC = actionH_HOST(&bufferHosts[i], allNodes[i].links[0].pkt);
        
        if (generateEventC){
          add(C, i, 0, currentTime + defaultBias * 33, &root, first - 1);
        }
      }
      else if (type == F)
      {
        #pragma region action of Event type F
        int portID = (data[first] >> 16) & MASK_INT;
        nextIndex = allNodes[i + numOfHosts].links[portID].nextIndex;

        nextPort = allNodes[i + numOfHosts].links[portID].nextPort;
        
        generateEventE = k;//pass k as parameter in the variable generateEventE
        nextIP = getNeighborIP(allNodes[i + numOfHosts].ipv4, allNodes[i + numOfHosts].type, portID, k);
        int generateEventD_OR_G = actionF(&bufferSwitches[i],
                                          portID,
                                          &allNodes[i + numOfHosts].links[portID],
                                          &generateEventE //,
                                                          //getIPv4OfHost(pairs[i], k), T
        );
        if(generateEventE){
          int pickUpENB = findENB_ID(portID, &bufferSwitches[i], currentTime, k);
          if(pickUpENB >= 0 && pickUpENB < k && pickUpENB != portID){
            //if(i == 19 && currentTime == 605926 - SWITCH_CYCLE)
            //  printf("DEBUG\n");
            idNodeInTree = first - 1;
              //hash(i, allNodes[i + numOfHosts].type, portID, E, k);
            //I believe the return value of hash in this case is (first - 1)
            add(E, i, portID, currentTime + SWITCH_CYCLE, &root, idNodeInTree);
          }
        }
        
        if (generateEventD_OR_G)
        {
          enum TypesOfNode tempNode = typeOfNode(nextIP, k);
          enum TypesOfEvent tempEvent = (tempNode == HOST) ? G : D;
          
          unsigned long loadingTime = loadingTime1;
          if(tempEvent == D){
            loadingTime = (bufferSwitches[i].type == CORE_SWITCH || bufferSwitches[nextIndex].type == CORE_SWITCH) ? loadingTime2 : loadingTime1;
          }
          idNodeInTree = hash(nextIndex, tempNode, nextPort, tempEvent, k);
          add(tempEvent, nextIndex, nextPort, currentTime + loadingTime, &root, idNodeInTree);
        }
        #pragma endregion
      }
      else if (type == G)
      {
        #pragma region action of Event type G
        j = currentTime / STEP_TIME;
        int nextNode = allNodes[i].links[0].nextIndex;
        int nextPort = allNodes[i].links[0].nextPort;
        if(allNodes[nextNode + numOfHosts].links[nextPort].pkt->id == 0 && 
            getIndexOfHost(allNodes[nextNode + numOfHosts].links[nextPort].pkt->srcIP, k) == 0){
          printf("Latency %ld\n", currentTime);
        }
        //int srcIP = allNodes[nextNode + numOfHosts].links[nextPort].pkt->srcIP;
        actionG(&bufferHosts[i], &receivedPkts[i][j],
                allNodes[nextNode + numOfHosts].links[nextPort].pkt);
        
        idNodeInTree = hash(nextNode, EDGE_SWITCH, nextPort, H, k);
        add(H, nextNode, nextPort,currentTime + 1, &root, idNodeInTree);
        
        #pragma endregion
      }
      else if (type == H)
      {
        int portID = (data[first] >> 16) & MASK_INT;
        generateEventF = actionH(&bufferSwitches[i], allNodes[i + numOfHosts].type, portID, 
                                    allNodes[i + numOfHosts].links[portID].pkt, k);
        if(generateEventF){
          add(F, i, portID, currentTime, &root, first - 1);
        }
      }
    }
    
    ongoingTime = -1;
    removeFirst(&first, &root //, arr
    );
    if (first != UINT_MAX)
    {
      currentTime = ((unsigned long)arr[first][0] << 32) + arr[first][1];
      ongoingTime = currentTime;

    }
    else
    {
      ongoingTime = -1;
    }
  }
  printf("\n\nFINISH!!!!!!!!!!!! ^_^....\n");

  unsigned long total = calculateThroughput(receivedPkts, PACKET_SIZE, STEP, numOfHosts, (double)numOfFlows*BANDWIDTH_HOST*STEP_TIME/1000000000);

  timing(&wc2, &cpuT);
  printf("Time: %'f ms with count = %'ld\n", (wc2 - wc1)*1000, count);
  printf("================================\n");
  badness(wc2 - wc1, page_size, proc_statm);

  //assertPackets(total, allNodes, bufferHosts,
  //                      bufferSwitches, numOfHosts, 5 * k * k / 4, k);

  
  return 0;
}