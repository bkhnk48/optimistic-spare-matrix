#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FatTreeGraph.c"
#include "FatTreeTables.c"
#include "MergeSplay.c"
#include "ControlFlow.c"
#include "timing.c"
#include "TestAPI.c"
#include <limits.h>
#include <locale.h>

int main(int argc, char **argv)
{
  unsigned int first = UINT_MAX;
  int defaultSec = 1;
  int defaultBias = 0;
  int k = 4;
  int PACKET_SIZE = 12 * 1000; //12KB
  int SWITCH_CYCLE = 100;
  //int packets_per_sec = 1000;//each sec generated 1000 packets
  int T = 1000;
  int BANDWIDTH_HOST = 12 * 1000 * 1000; //12MByte/s
  int loadingTime = BANDWIDTH_HOST / PACKET_SIZE + 13;
  unsigned long **receivedPkts = NULL;
  int STEP = 100;

  if (argc >= 2)
  {
    k = atoi(argv[1]);
    if (argc > 2)
      defaultSec = atoi(argv[2]);
    if (argc > 3)
      defaultBias = atoi(argv[3]);
  }

  unsigned long currentTime = 0;
  int numOfHosts = k * k * k / 4;
  unsigned long endTime = defaultSec * ((unsigned long)(1000 * 1000));
  unsigned long STEP_TIME = endTime / STEP;

  unsigned long i, j;
  unsigned int N, root = UINT_MAX;

  arr = malloc(sizeof *arr * (6 * k * k * k));
  data = malloc((6 * k * k * k) * sizeof(unsigned long));
  for (i = 0; i < 6 * k * k * k; i++)
  {
    arr[i] = malloc(sizeof *arr[i] * BUFFER_SIZE);
    for (j = 0; j < BUFFER_SIZE; j++)
    {
      arr[i][j] = UINT_MAX;
    }
    data[i] = 0;
  }

  int *pairs = NULL;
  pairs = malloc((k * k * k / 4) * sizeof(int));
  for (i = 0; i < numOfHosts; i++)
  {
    //if ((i + 1) % (k / 2) != 0)
    //  pairs[i] = (i / (k / 2) + 1) * (k / 2) - 1;
    //else
    //  pairs[i] = 0;
    int pod = i / (k*k/4);
    pairs[i] = ((i % (k*k/4)) + 1)%(k*k/4) + pod*(k*k/4);
    //printf("host %ld sends to %d\n", i, pairs[i]);
  }

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
  //int delta = 0;

  root = UINT_MAX;

  for (i = k/2 - 1; i < k / 2; i++) //Only test first k/2 hosts
  {
    idNodeInTree = hash(i, HOST, 0, A, k);
    add(A, i, 0, 0, &root //, arr
        ,
        idNodeInTree);
  }

  int cPkt = 0;

  removeFirst(&first, &root //, arr
  );

  unsigned long ongoingTime = (unsigned long)arr[first][0] << 32 + arr[first][1];
  while (currentTime <= endTime && ongoingTime != -1)
  {
    if (ongoingTime == currentTime)
    {
      count++;
      #pragma region get value from data array
      int type = data[first] & 65535; //type of event
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
        if (generateEventB)
        {
          add(B, i, 0, currentTime + defaultBias * 13, &root, first + 1);
        }
      }
      #pragma endregion

      #pragma region action of Event type B
      else if (type == B)
      {

        generateEventC = actionB(&bufferHosts[i],
                                 allNodes[i].links[0].pkt);
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
                                 getIPv4OfHost(pairs[i], k), T);
        if (generateEventB)
          add(B, i, 0, currentTime + defaultBias * 13, &root, first - 1);
        if (generateEventD)
        {
          idNodeInTree = hash(nextIndex, EDGE_SWITCH, nextPort, D, k);
          add(D, nextIndex, nextPort, currentTime + loadingTime, &root, idNodeInTree);
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
        idPrev += (allNodes[i + numOfHosts].type == EDGE_SWITCH && idPrevPort < k / 2
                       ? 0
                       : numOfHosts);

        Packet *pkt = allNodes[idPrev].links[idPrevPort].pkt;

        assert(pkt->srcIP != -1 && pkt->dstIP != -1 && pkt->generatedTime != -1);

        int preLast = bufferSwitches[i].firstLastENBs[portID][1];

        int posInENB = receivePacket(portID,
                                     &bufferSwitches[i],
                                     currentTime,
                                     pkt);

        checkENB_EXB(&bufferSwitches[i], k);
        int last = bufferSwitches[i].firstLastENBs[portID][1];
        assert(last == (preLast + 1) % BUFFER_SIZE);

        if (posInENB == bufferSwitches[i].firstLastENBs[portID][0])
        {
          //Packet is ahead of all other ones on ENB
          assert(bufferSwitches[i].requestedTimeOfENB[portID] == currentTime);
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

          generateEventE = actionD(portID,
                                   nextEXB,
                                   &bufferSwitches[i],
                                   currentTime);

          checkENB_EXB(&bufferSwitches[i], k);
          if (generateEventE)
          {
            idNodeInTree = hash(i, allNodes[i + numOfHosts].type, nextEXB, E, k);
            add(E, i, nextEXB, currentTime + SWITCH_CYCLE, &root, idNodeInTree);
          }
        }
        #pragma endregion
        assert(allNodes[idPrev].links[0].pkt->id == -1);
      }
      else if (type == E)
      {
        #pragma region action of Event type E
        int portID = (data[first] >> 16) & MASK_INT;
        
        int pickUpENB = chooseENB_ID(portID,
                                     &bufferSwitches[i], k);
        assert(pickUpENB >= 0 && pickUpENB < k && pickUpENB != portID);

        generateEventE = 0;
        generateEventF = 0;
        generateEventH = 0;
        generateEventH_HOST = 0;
        int H_IS_HOST =
            ((allNodes[i + numOfHosts].type == EDGE_SWITCH &&
              pickUpENB < (k / 2 - 1))
                 ? 1
                 : 0);
        //number of empty slots in ENB b4 moving
        int preEmptyInENB = countEmptySlots(bufferSwitches[i].firstLastENBs[pickUpENB][0], bufferSwitches[i].firstLastENBs[pickUpENB][1]);
        //number of empty slots in EXB b4 moving
        int preEmptyInEXB = 0;
        preEmptyInEXB = countEmptySlots(bufferSwitches[i].firstLastEXBs[portID][0], bufferSwitches[i].firstLastEXBs[portID][1]);
        
        move(pickUpENB, portID, &bufferSwitches[i]);

        checkENB_EXB(&bufferSwitches[i], k);
        //number of empty slots in ENB after moving
        int postEmptyInENB = countEmptySlots(bufferSwitches[i].firstLastENBs[pickUpENB][0], bufferSwitches[i].firstLastENBs[pickUpENB][1]);
        //number of empty slots in EXB after moving
        int postEmptyInEXB = countEmptySlots(bufferSwitches[i].firstLastEXBs[portID][0], bufferSwitches[i].firstLastEXBs[portID][1]);
        assert(preEmptyInENB == postEmptyInENB - 1);
        assert(preEmptyInEXB == postEmptyInEXB + 1);

        #pragma region Shift packet in ENB
        Packet *ENB = bufferSwitches[i].ENB[pickUpENB];
        int posInENB = bufferSwitches[i].firstLastENBs[pickUpENB][0];

        if (ENB[posInENB].srcIP != -1 && ENB[posInENB].dstIP != -1 && ENB[posInENB].id != -1 && ENB[posInENB].generatedTime != -1)
        {
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
          signEXB_ID(nextEXB, &bufferSwitches[i].registeredEXBs[pickUpENB]);
        }
        #pragma endregion
        
        int generatedEF = actionE(pickUpENB, portID,
                                  &bufferSwitches[i],
                                  &allNodes[i + numOfHosts].links[portID]);

        checkENB_EXB(&bufferSwitches[i], k);
        generateEventE = generatedEF & 1;
        generateEventF = (generatedEF & 2) >> 1;

        if (generateEventE)
        {
          idNodeInTree = hash(i, allNodes[i + numOfHosts].type,
                              portID, E, k);
          add(E, i, portID, currentTime + SWITCH_CYCLE, &root, idNodeInTree);
        }

        if (generateEventF)
        {
          idNodeInTree = hash(i, allNodes[i + numOfHosts].type,
                              portID, F, k);
          add(F, i, portID, currentTime + SWITCH_CYCLE, &root, idNodeInTree);
        }

        int idPrev = allNodes[i + numOfHosts].links[pickUpENB].nextIndex;
        idNodeInTree = 0;
        if (H_IS_HOST)
        {
          idNodeInTree = hash(idPrev, HOST, 0, H_HOST, k);
          add(H_HOST, idPrev, 0, currentTime + 1, &root, idNodeInTree);
        }
        else
        {
          //generate event H
        }

        #pragma endregion
      }
      else if (type == H_HOST)
      {
        //generateEventB = 0;
        int tempCount = bufferHosts[i].countNextENB;
        generateEventC = actionH_HOST(&bufferHosts[i],
                                      allNodes[i].links[0].pkt
                                      //, &generateEventB
        );
        assert(bufferHosts[i].countNextENB == tempCount + 1);
        int nextNode = allNodes[i].links[0].nextIndex;
        int nextPort = allNodes[i].links[0].nextPort;
        int firstENB = bufferSwitches[nextNode].firstLastENBs[nextPort][0];
        int lastENB = bufferSwitches[nextNode].firstLastENBs[nextPort][1];
        assert(bufferHosts[i].countNextENB == countEmptySlots(firstENB, lastENB));

        if (generateEventC)
          add(C, i, 0, currentTime + defaultBias * 33, &root, first - 1);
      }
      else if (type == F)
      {
        #pragma region action of Event type F
        int portID = (data[first] >> 16) & MASK_INT;
        nextIndex = allNodes[i + numOfHosts].links[portID].nextIndex;

        nextPort = allNodes[i + numOfHosts].links[portID].nextPort;
        generateEventE = 0;
        nextIP = getNeighborIP(allNodes[i + numOfHosts].ipv4, allNodes[i + numOfHosts].type, portID, k);
        int generateEventD_OR_G = actionF(&bufferSwitches[i],
                                          portID,
                                          &allNodes[i + numOfHosts].links[portID],
                                          &generateEventE //,
                                                          //getIPv4OfHost(pairs[i], k), T
        );
        //if(generateEventE)
        //  add(E, i, 0, currentTime +  defaultBias*13
        //                , &root, first - 1
        //          );
        if (generateEventD_OR_G)
        {
          enum TypesOfNode tempNode = typeOfNode(nextIP, k);
          enum TypesOfEvent tempEvent = (tempNode == HOST) ? G : D;
          Packet *pkt = allNodes[i + numOfHosts].links[portID].pkt;
                                          
          assert(pkt->srcIP != -1 && pkt->dstIP != -1 && pkt->generatedTime != -1);

          //nextIndex += ((tempNode == HOST) ? 0 : numOfHosts);
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
        //int srcIP = allNodes[nextNode + numOfHosts].links[nextPort].pkt->srcIP;
        actionG(&bufferHosts[i], &receivedPkts[i][j],
                allNodes[nextNode + numOfHosts].links[nextPort].pkt);
        assert(allNodes[nextNode + numOfHosts].links[nextPort].pkt->srcIP == -1);
        #pragma endregion
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
  printf("\n\nFINISH !!!!!!!!!!!! ^_^....\n");

  assertPackets(allNodes, bufferHosts,
                        bufferSwitches, numOfHosts, 5 * k * k / 4, k);

  return 0;
}