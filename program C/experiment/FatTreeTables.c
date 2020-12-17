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
  int i ;
  int numOfSwitches = k*k*5/4;
  #pragma region initialization
  for(i = 0; i < numOfSwitches; i++){
    if(i < k*k){
      if((i % k) >= 0 && (i %k) <= (k/2 - 1)){
        tablesOfSwitches->tables[i].type = EDGE_SWITCH;
        tablesOfSwitches->tables[i].prefixTable = NULL;
        tablesOfSwitches->tables[i].subPrefix = 0;
      }
      else{
        tablesOfSwitches->tables[i].type = AGG_SWITCH;
      }
    }
    else{
      tablesOfSwitches->tables[i].type = CORE_SWITCH;
      tablesOfSwitches->tables[i].suffixTable = NULL;
    }
  }
  #pragma endregion

  
}
#endif

