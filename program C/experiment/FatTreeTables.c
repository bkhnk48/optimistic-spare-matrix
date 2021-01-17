#include <stdio.h>
#include <stdlib.h>
#include "FatTreeGraph.c"

#ifndef _LIB_OF_TABLE_IN_FATTREE_
#define _LIB_OF_TABLE_IN_FATTREE_

typedef struct _routing_table{
  enum TypesOfNode type;
  int subPrefix;
  int *suffixTable;
  int *prefixTable;
} RoutingTable;

typedef struct _tables{
  RoutingTable *tables;
}Tables;


void buildTables(Tables *tablesOfSwitches, int k){
  //In Fat-tree, build total k*k*5/4 routing tables
  tablesOfSwitches->tables = (RoutingTable *)malloc((k*k*5/4)*sizeof(RoutingTable));
  int i, j ;
  int numOfSwitches = k*k*5/4;
  #pragma region initialization
  for(i = 0; i < numOfSwitches; i++){
    if(i < k*k){
      #pragma region Build tables for edge switches
      if((i % k) >= 0 && (i %k) <= (k/2 - 1)){
        tablesOfSwitches->tables[i].type = EDGE_SWITCH;
        tablesOfSwitches->tables[i].prefixTable = NULL;
        tablesOfSwitches->tables[i].subPrefix = 0;
        int ipv4 = getIPv4OfSwitch(i, k);
        tablesOfSwitches->tables[i].suffixTable = 
              (int *)malloc((k/2)*sizeof(int));
        for(j = 0; j < k/2; j++){
          tablesOfSwitches->tables[i].suffixTable[j]
            = getNeighborIP(ipv4, EDGE_SWITCH, j + k/2, k);//AGG SWITCH
        }
      }
      #pragma endregion
      else{
      #pragma region Build tables for agg switch
        tablesOfSwitches->tables[i].type = AGG_SWITCH;
        int ipv4 = getIPv4OfSwitch(i, k);
        tablesOfSwitches->tables[i].suffixTable = 
              (int *)malloc((k/2)*sizeof(int));
        for(j = 0; j < k/2; j++){
          tablesOfSwitches->tables[i].suffixTable[j]
            = getNeighborIP(ipv4, AGG_SWITCH, j + k/2, k);//CORE SWITCH
        }
        int p = i/k;
        tablesOfSwitches->tables[i].subPrefix = (10 << 16) | (p << 8);
        tablesOfSwitches->tables[i].prefixTable = 
              (int *)malloc((k/2)*sizeof(int));
        for(j = 0; j < k/2; j++){
          tablesOfSwitches->tables[i].prefixTable[j]
            = getNeighborIP(ipv4, AGG_SWITCH, j, k);//EDGE SWITCH
        }
      #pragma endregion  
      }
    }
    else{
      int ipv4 = getIPv4OfSwitch(i, k);
      tablesOfSwitches->tables[i].type = CORE_SWITCH;
      tablesOfSwitches->tables[i].suffixTable = NULL;
      tablesOfSwitches->tables[i].subPrefix = 0;
      tablesOfSwitches->tables[i].prefixTable = 
            (int *)malloc(k*sizeof(int));
      for(j = 0; j < k; j++){
        tablesOfSwitches->tables[i].prefixTable[j]
          = getNeighborIP(ipv4, CORE_SWITCH, j, k);//AGG SWITCH
      }
    }
  }
  #pragma endregion
}

//int next(int source, int current, int destination, int k) {
int next(int srcIP, int currIP, int destIP, int k, 
            RoutingTable *table
          ) {
  int nextIP ;
  int podOfSrc = (srcIP >> 16) & 255;
  int podOfDst = (destIP >> 16) & 255;
  int subnetOfSrc = (srcIP >> 8) & 255;
  int subnetOfDest = (destIP >> 8) & 255;
  if(currIP == srcIP){
    return getNeighborIP(srcIP, HOST, 0, k);
  }
  else{
    int aheadOfDst = getNeighborIP(destIP, HOST, 0, k);
    if(aheadOfDst == currIP){
      return destIP;
    }
    int typeOfSwitch = typeOfNode(currIP, k);
    int suffix = destIP & 255; 
    int podOfCurr = (currIP >> 16) & 255;
    suffix -= 2;
    if(typeOfSwitch == CORE_SWITCH){
      nextIP = //tablesOfSwitches->tables[i].
                table->prefixTable[podOfDst];
      return nextIP;
    }  
    if(typeOfSwitch == AGG_SWITCH){
      
      if(podOfDst == podOfCurr){
        //agg nhan duoc goi tin yeu cau di den host cung pod (voi agg)
        //agg se chuyen goi tin sang edge cung pod
        nextIP = //tablesOfSwitches->tables[i].
                  table->prefixTable[subnetOfDest];
        return nextIP;
      }
      else{
        nextIP = //tablesOfSwitches->tables[i].
                  table->suffixTable[suffix];
        return nextIP;
      }
    }
    if(typeOfSwitch == EDGE_SWITCH){
      int subnetOfEdge = (currIP >> 8) & 255;
      if(subnetOfDest != subnetOfEdge 
          || podOfDst != podOfCurr){
        //If destination host is not the same subnet of edge
        //Or pod of dest is not the pod of edge    
        nextIP = table->suffixTable[suffix];
        return nextIP;
      }
      else{
        //Dest host is one of the neighbor of edge
        return destIP;
      }
    }
  }
}
#endif

