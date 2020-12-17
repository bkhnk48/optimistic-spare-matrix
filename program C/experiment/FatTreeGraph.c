#include <stdio.h>
#include <stdlib.h>


#ifndef _LIB_OF_FATTREE_
#define _LIB_OF_FATTREE_

enum TypesOfNode
{
    HOST = 0, //host
    POD_SWITCH = 1, //pod switch
    CORE_SWITCH = 2, //core switch
    ERROR = -1
};

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
  if(lastBit == 1)
    return POD_SWITCH;//the pod switch
  return HOST; //the host
}

int getIndexOfSwitch(int ipv4, int k){
  int node = typeOfNode(ipv4, k);
  if(node == HOST) return ERROR;
  if(node == POD_SWITCH){
    //p = 
  }
  return ERROR;
}
#endif

