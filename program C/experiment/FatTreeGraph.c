#include <stdio.h>
#include <stdlib.h>
#include "NetworkNode.c"

#ifndef _LIB_OF_FATTREE_
#define _LIB_OF_FATTREE_



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

NetworkNode *initNetworkNodes(int numOfHosts, int numOfSwitches, int k){
    NetworkNode *networkNodes = malloc((numOfSwitches + numOfHosts) 
                                        *sizeof(NetworkNode)
                                        );
    
    int i, j; int pod, index;
    for(i = 0; i < numOfHosts; i++){
      networkNodes[i].indexInGroup = i;
      pod = i / (k*k/4);
      networkNodes[i].indexInNodes = i % (k*k/4) + pod*((k*k/4) + k);
      networkNodes[i].ipv4 = getIPv4OfHost(i, k);
      networkNodes[i].type = HOST;
      networkNodes[i].links = malloc(1*sizeof(int));
    }
    for(i = numOfHosts; i < numOfHosts + numOfSwitches - (k*k/4); i++){
      pod = i / (k*k/4 + k);
      index = i - pod*(k*k/4) - (k*k/4);
      networkNodes[i].indexInGroup = index;
      //networkNodes[i].indexInNodes = i % (k*k/4) + pod*((k*k/4) + k);
      networkNodes[i].ipv4 = getIPv4OfSwitch(index, k);
      networkNodes[i].type = typeOfNode(networkNodes[i].ipv4, k);
      networkNodes[i].links = malloc(k*sizeof(int));
      for(j = 0; j < k; j++)
        networkNodes[i].links[j] = 0;
    }
    for(i = numOfHosts + numOfSwitches - (k*k/4); 
              i < numOfHosts + numOfSwitches; i++){
      index = i - (k*k*k/4);
      networkNodes[i].indexInGroup = index;
      networkNodes[i].indexInNodes = i ;
      networkNodes[i].ipv4 = getIPv4OfSwitch(index, k);
      networkNodes[i].type = CORE_SWITCH;
      networkNodes[i].links = malloc(k*sizeof(int));
      for(j = 0; j < k; j++)
        networkNodes[i].links[j] = 0;
    }
}
#endif

