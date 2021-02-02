#include <stdio.h>
#include <stdlib.h>
#include "NetworkNode.c"
#include "Event.c"

#ifndef _LIB_OF_FATTREE_
#define _LIB_OF_FATTREE_

#define INPORT 0
#define OUTPORT 1

int getIPv4OfHost(int index, int k){
  //each pod has numOfPorts^2/4 servers and numOfPorts switches
  int numOfNodesInPod = k*k/4 + k;
  int p = index / (k*k/4);//pod of server
  int x = index % (k*k/4);
  int e = x / (k/2);//subnet
  int serverID = numOfNodesInPod*p + x;
  int h = x - e*(k/2) + 2;
  int ipv4 = (10 << 24) | (p << 16) | (e << 8) | h;
  return ipv4;
}

int getIndexOfHost(int ipv4, int k){
  int h = ipv4 & 255;
  int e = (ipv4 >> 8) & 255;
  int x = e*(k/2) + h - 2;
  int p = (ipv4 >> 16) & 255;
  int index = p*(k*k/4) + x;
  return index;
}

int getIPv4OfSwitch(int index, int k){
  //each pod has numOfPorts^2/4 servers and numOfPorts switches
  int numOfPodSwitches = k*k;
  int ipv4 = 0;
  if(index >= numOfPodSwitches){
    index -= numOfPodSwitches;
    int i = (index % (k/2)) + 1;
    int j = ((index - i + 1)/ (k/2)) + 1;
    //ip of core switch
    ipv4 = (10 << 24) | (k << 16) | (j << 8) | i;
  }
  else{
    //ip of pod switches
    int p = index / (k);//pod of switch
    int s = index % k;
    ipv4 = (10 << 24) | (p << 16) | (s << 8) | 1;
  }
  return ipv4;
}

/*if this function returns 0: the ipv4 address is of Host
 *                         1: the node is pod switch
 *                         2: the node is core switch
*/
int typeOfNode(int ipv4, int k){
  
  int B = (ipv4 << 8) >> 24;
  if(B == k)
    return CORE_SWITCH;//the core switch
  int lastBit = (ipv4 & 255);
  if(lastBit == 1){
    int s = (ipv4 >> 8) & 255;
    if(s >= 0 && s <= (k/2) - 1)
      return EDGE_SWITCH;//the edge switch
    else 
      return AGG_SWITCH;
  }
  return HOST; //the host
}

enum TypesOfNode typeOfIndex(int index, int k){
  if(index < k*k*k/4){
    return HOST;
  }
  int maxPodSwitch = k*k*k/4 + k*k;
  if(index >= k*k*k/4 && index < maxPodSwitch){
    index -= k*k*k/4;
    int isEdge = (index % k) < k/2;
    return (isEdge ? EDGE_SWITCH : AGG_SWITCH);
  }
  if(index < maxPodSwitch + k*k/4){
    return CORE_SWITCH;
  }
  return -1; //Error
}

int getIndexOfSwitch(int ipv4, int k){
  int node = typeOfNode(ipv4, k);
  if(node == HOST) return ERROR;
  if(node == EDGE_SWITCH || node == AGG_SWITCH){
    int p = (ipv4 << 8) >> 24;
    int s = (ipv4 << 16) >> 24;
    int index = p*k + s;
    return index;
  }
  else if(node == CORE_SWITCH){
    //index = a*(k/2) + b + numOfPodSwitches;
    int i = ipv4 & 255;
    int j = (ipv4 >> 8) & 255;
    int b = i - 1;
    int a = (j - 1) + (b*2/k);
    int index = ((a*k/2) + b) + k*k;
    return index;
  }
  return ERROR;
}

void printIPv4(int ipv4){
  //an IPv4 address has format: A.B.C.D
  int A = (ipv4 >> 24) & 255;
  int B = (ipv4 >> 16) & 255;
  int C = (ipv4 >>  8) & 255;
  int D = ipv4 & 255;
  printf("%d.%d.%d.%d", A, B, C, D);
}

void assignTypeOfSwitch(BufferSwitch *bufferSwitches, int k){
  int numOfSwitches = 5*k*k/4;
  int numOfHosts = k*k*k/4;
  int i;
  for(i = 0; i < numOfSwitches; i++){
    bufferSwitches[i].type = typeOfIndex(i + numOfHosts, k);
  }
}

int getNeighborIP(int currentIP, enum TypesOfNode typeOfNode,
                      int port, int k){
  int neighborIP;
  if(typeOfNode == HOST){
    int pod = (currentIP >> 16) & 255;
    int _switch = (currentIP >> 8) & 255;
    neighborIP = (10 << 24) | (pod << 16) | (_switch << 8) | 1;
    return neighborIP;
  }

  #pragma region neighbor of edge switch
  if(typeOfNode == EDGE_SWITCH){
    int pod = (currentIP >> 16) & 255;
    int _switch = (currentIP >> 8) & 255;
    if(port >= k/2){//neighbor is agg switch
      int _switchOfAgg = port;
      neighborIP = (10 << 24) | (pod << 16) | (_switchOfAgg << 8) | 1;
      return neighborIP;
    }
    else{//neighbor is host
      int ID = 2 + port;
      neighborIP = (10 << 24) | (pod << 16) | (_switch << 8) | ID;
      return neighborIP;
    }
  }
  #pragma endregion

  #pragma region neighbor of agg switch
  if(typeOfNode == AGG_SWITCH){
    int pod = (currentIP >> 16) & 255;
    int _switch = (currentIP >> 8) & 255;

    if(port >= 0 && port < k/2){//neighbor is edge switch
      //int ID = 2 + port;
      _switch = port;
      neighborIP = (10 << 24) | (pod << 16) | (_switch << 8) | 1;
      return neighborIP;
    }
    else{//neighbor is core switch
      int indexOfAggInPod = _switch % (k/2);
      int core = indexOfAggInPod*(k/2) + (port - (k/2)) + (k*k);  
      neighborIP = getIPv4OfSwitch(core, k);
      return neighborIP;
    }  
  }
  #pragma endregion

  #pragma region neighbor of core switch
  if(typeOfNode == CORE_SWITCH){
    //address of Core: 10.k.j.i
    int j = (currentIP >> 8) & 255;
    j -= 1;
    int _switch = j + (k/2);
    neighborIP = (10 << 24) | (port << 16) | (_switch << 8) | 1;
    return neighborIP;
  }
  #pragma endregion
  return neighborIP;
}

NetworkNode *initNetworkNodes(int numOfHosts, int numOfSwitches, int k){
  NetworkNode *networkNodes = malloc((numOfSwitches + numOfHosts) 
                                      *sizeof(NetworkNode)
                                      );
  
  int i, j; int pod, index;
  int nextIP, nextIndex, currIP;
  for(i = 0; i < numOfHosts; i++){
    networkNodes[i].indexInGroup = i;
    pod = i / (k*k/4);
    networkNodes[i].indexInNodes = i % (k*k/4) + pod*((k*k/4) + k);
    currIP = getIPv4OfHost(i, k);
    networkNodes[i].ipv4 = currIP;
    networkNodes[i].type = HOST;
    networkNodes[i].links = malloc(1*sizeof(Link));
    networkNodes[i].links[0].pkt = malloc(1*sizeof(Packet));
    networkNodes[i].links[0].pkt->id = -1;
    networkNodes[i].links[0].pkt->srcIP = networkNodes[i].ipv4;
    networkNodes[i].links[0].pkt->currIP = networkNodes[i].ipv4;
    networkNodes[i].links[0].pkt->dstIP = -1;
    networkNodes[i].links[0].nextPort = (currIP & 255) - 2;
    //networkNodes[i].links[0].currPort = 0;
    networkNodes[i].links[0].nextIndex = 
                  getIndexOfSwitch(
                      getNeighborIP(currIP, HOST, 0, k), k
                    );
    networkNodes[i].generatedPackets = 0;
  }
  
  for(i = numOfHosts; i < numOfHosts + numOfSwitches - (k*k/4); i++){
    //pod = (i - numOfHosts)/ (k);
    index = (i - numOfHosts) % k;
    networkNodes[i].indexInGroup = i - numOfHosts;
    networkNodes[i].indexInNodes = i;
    currIP = getIPv4OfSwitch(i - numOfHosts, k);
    networkNodes[i].ipv4 = currIP;
    networkNodes[i].type = (index % k < k/2 ? EDGE_SWITCH : AGG_SWITCH);
    networkNodes[i].links = malloc(k*sizeof(Link));
    for(j = 0; j < k; j++){
      networkNodes[i].links[j].pkt = malloc(1*sizeof(Packet));
      networkNodes[i].links[j].pkt->id = -1;
      networkNodes[i].links[j].pkt->srcIP = -1;
      networkNodes[i].links[j].pkt->currIP = networkNodes[i].ipv4;
      networkNodes[i].links[j].pkt->dstIP = -1;
      networkNodes[i].links[j].nextPort = 0;
      //networkNodes[i].links[j].currPort = j;
      networkNodes[i].links[j].nextIndex = 0;
      //networkNodes[i].links[j].currIndex = i;
    }

    if(networkNodes[i].type == EDGE_SWITCH){
      for(j = 0; j < k/2; j++){
        networkNodes[i].links[j].nextIndex = 
              getIndexOfHost(
                getNeighborIP(currIP, EDGE_SWITCH, j, k), k
              );
      }
      for(j = k/2; j < k; j++){
        networkNodes[i].links[j].nextPort = (currIP >> 8) & 255;
        networkNodes[i].links[j].nextIndex = 
              getIndexOfSwitch(
                getNeighborIP(currIP, EDGE_SWITCH, j, k), k
              );
      }
    }
    else if(networkNodes[i].type == AGG_SWITCH){
      for(j = 0; j < k; j++){
        networkNodes[i].links[j].nextIndex = 
              getIndexOfSwitch(
                getNeighborIP(currIP, AGG_SWITCH, j, k), k
              );
      }
      for(j = 0; j < k/2; j++){
        networkNodes[i].links[j].nextPort = (currIP >> 8) & 255;
      }
      for(j = k/2; j < k; j++){
        networkNodes[i].links[j].nextPort = ((currIP >> 8) & 255) - k/2;
      }
    }

    networkNodes[i].generatedPackets = 0;
  }
  
  for(i = numOfHosts + numOfSwitches - (k*k/4); 
            i < numOfHosts + numOfSwitches; i++){
    index = i - (k*k*k/4);
    networkNodes[i].indexInGroup = index;
    networkNodes[i].indexInNodes = i ;
    currIP = getIPv4OfSwitch(index, k);
    networkNodes[i].ipv4 = currIP;
    networkNodes[i].type = CORE_SWITCH;
    networkNodes[i].links = malloc(k*sizeof(Link));
    for(j = 0; j < k; j++){
      networkNodes[i].links[j].pkt = malloc(1*sizeof(Packet));
      networkNodes[i].links[j].pkt->id = -1;
      networkNodes[i].links[j].pkt->srcIP = -1;
      networkNodes[i].links[j].pkt->currIP = networkNodes[i].ipv4;
      networkNodes[i].links[j].pkt->dstIP = -1;
      networkNodes[i].links[j].nextPort = (currIP & 255) - 1 + k/2;
      //networkNodes[i].links[j].currPort = j;
      networkNodes[i].links[j].nextIndex = 
              getIndexOfSwitch(
                getNeighborIP(currIP, CORE_SWITCH, j, k), k
              );
      //networkNodes[i].links[j].currIndex = i;
    }

    networkNodes[i].generatedPackets = 0;
  }
  
  return networkNodes;
}

int *Stride(int stride, int k){
    int *pairs = malloc((k*k*k/4) * sizeof(int));
    int i;
    int numOfHosts = k*k*k/4;
    for(i = 0; i < numOfHosts; i++){
        pairs[i] = (i + stride) % numOfHosts;
    }
    return pairs;
}

int hash(int idInGroup,enum TypesOfNode typeOfNode, 
            int port, int typeOfEvent,
            int k){
  int newIndex = 0;
  if(typeOfNode == HOST){
    newIndex = idInGroup*5 + typeOfEvent;
  }
  else if(typeOfNode == EDGE_SWITCH){
    int segment = (idInGroup/(k/2))/2;
    int offset = (idInGroup %(k/2));
    newIndex = segment*k/2 + offset;
    newIndex *= 7*k/2;
    newIndex += 5*k*k*k/4;
    if(port < k/2)
    {
      newIndex += port*3 + (typeOfEvent - D);
    }
    else{
      newIndex += (3*k/2) + (port - k/2)*4 + (typeOfEvent - D);
    }
  }
  else{
    if(typeOfNode == AGG_SWITCH){
      int segment = idInGroup/k;
      int offset = (idInGroup %k) - (k/2);
      newIndex = segment*(k/2) + offset;
      newIndex *= 4*k;
      newIndex += (5*k*k*k/4) + (7*k*k*k/4);
    }
    else{
      newIndex = idInGroup - (k*k);
      newIndex *= 4*k;
      newIndex += (5*k*k*k/4) + (7*k*k*k/4) + (4*k*k*k/2);
    }
    newIndex += port*4 + (typeOfEvent - D);
  }
  return newIndex;
}
#endif

