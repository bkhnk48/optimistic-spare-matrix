#include <stdio.h>
#include <stdlib.h>


#ifndef _LIB_OF_FATTREE_
#define _LIB_OF_FATTREE_

int getIPv4(int index, int k){
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

#endif

