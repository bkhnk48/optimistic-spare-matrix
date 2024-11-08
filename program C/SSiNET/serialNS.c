#include <stdio.h>
#include "LinkSimulation.c"

int main(int argc, char** argv) 
{
    int numOfPorts = 4;
    int numOfSwitches = numOfPorts * numOfPorts * 5 / 4;
    int numOfHosts = numOfPorts * numOfPorts * numOfPorts / 4;
    int numOfInforAboutSwitch = 12;
    double bandwidth = 1; //bo nhan voi 1 ty, vi tu nay bandwith se tinh don vi Gb, 1000*1000*1000;
    int BUFFER_SIZE = 5;
    int PACKET_SIZE = 1e5;
    int serialTime = (int)PACKET_SIZE/bandwidth;
    double length = 1; //1m
    double PROPAGATION_VELOCITY = 0.2;
    int propagationTime = (int)(length/PROPAGATION_VELOCITY);


    char *a = argv[1];
    int stop = atoi(a);

    int i, j;

    //De luu thong tin ve link va cac goi du lieu truyen tren link
    //cung nhu cac su kien xay ra tren link
    //ta dung mang 1 chieu nhu sau:
    //  tren mot chieu se co id cua nut nguon, id cua nut dich, id cua goi tin, id cua Host nguon,
    //  id cua Host dich, nix-vector (?), kich thuoc goi tin, bit dau tien cua goi tin se den vao luc nao,
    //  toan bo goi tin se den nut dich khi nao?, bang thong
    int countOfInfoInLink = 13;
    int numOfLinks = numOfSwitches * numOfPorts + numOfHosts;
    int **Links = NULL;
    Links = malloc(sizeof * Links * numOfLinks);

    for(i = 0; i < numOfLinks; i++)
    {
      Links[i] = malloc(sizeof * Links[i] * countOfInfoInLink);
    }

    
    assignLink(Links, numOfPorts, serialTime, propagationTime);
    //showLink(Links, numOfLinks);

    int *IsHost = NULL;
    IsHost = malloc( sizeof * IsHost * (numOfHosts + numOfSwitches)); 
    for(i = 0; i < numOfHosts + numOfSwitches; i++)
    {
      IsHost[i] = 0;
    }


    /*
      this array: *sourceQueue stores all source queues of all hosts.
      the total number is: numOfHosts + numOfSwitches, 
      the index of sourceQueue is the index of hosts
     */
    Queue **sourceQueue = NULL;
    sourceQueue = malloc(sizeof * sourceQueue * (numOfHosts));
    for(i = 0; i < numOfHosts; i++)
    {
      sourceQueue[i] = malloc(sizeof * sourceQueue[i] * 2);
    }

    setSourceQueue(sourceQueue, numOfHosts);

    //int **Hosts = NULL;
    Host *Hosts = NULL;
    //int NUM_OF_FIELD_IN_HOST = 16;
    Hosts = malloc( sizeof * Hosts * numOfHosts);

    /*for(i = 0; i < numOfHosts; i++)
    {
      Hosts[i] = malloc( sizeof * Hosts[i] * NUM_OF_FIELD_IN_HOST);
    }*/
    assignHosts(Hosts, IsHost, numOfHosts, BUFFER_SIZE);
    assignLinkID(Hosts, Links, IsHost, numOfLinks);

    

    //show(Hosts, numOfHosts);

    int *SwitchIndexes = NULL;//This array holds the indexes of switch IDs. For example, we have 4 switches with IDs: 4, 8, 9, 10
                              //their indexes are: 1, 2, 3, 4
    int countOfInfoInSwitch = 15;

    SwitchIndexes = malloc( sizeof * SwitchIndexes * (numOfSwitches + numOfHosts));

    assignSwitchIndexes(SwitchIndexes, IsHost, numOfHosts + numOfSwitches);
    //showSwitchIDs(SwitchIndexes, numOfHosts + numOfSwitches);

    int **MapFromNodesToPorts = NULL;//Array stores all mapping from Destination ID to Port 
          //of switches
    MapFromNodesToPorts = malloc( sizeof * MapFromNodesToPorts * numOfSwitches);
    for(i = 0; i < numOfSwitches /*+ numOfHosts*/; i++)
    {
      MapFromNodesToPorts[i] = malloc( sizeof * MapFromNodesToPorts[i] * (numOfPorts));
      for(j = 0; j < numOfPorts; j++)
      {
        MapFromNodesToPorts[i][j] = 0;
      }
    }
    mappingNodeToPort(MapFromNodesToPorts, Links, SwitchIndexes, 
                            numOfLinks, numOfPorts);
    assignPortInfo(MapFromNodesToPorts, Links, SwitchIndexes, numOfLinks, numOfPorts);
    //showPortOfSwitch(MapFromNodesToPorts, Links, SwitchIndexes, numOfLinks, numOfPorts);
    

    int **SwitchPortPID = NULL; //Array stores IDs of packets in inport
    //int **SwitchOutportPID = NULL;//Array stores IDs of packets in outport

    int **SwitchPortSrcIDs = NULL;
    int **SwitchPortDstIDs = NULL;

    int **SwitchPortHopCount = NULL;

    //int **SwitchOutportSrcIDs = NULL;
    //int **SwitchOutportDstIDs = NULL;

    

    SwitchPortPID = malloc( sizeof * SwitchPortPID * (numOfSwitches * numOfPorts));
    SwitchPortSrcIDs = malloc( sizeof * SwitchPortSrcIDs * (numOfSwitches * numOfPorts));
    SwitchPortDstIDs = malloc( sizeof * SwitchPortDstIDs * (numOfSwitches * numOfPorts));
    SwitchPortHopCount = malloc( sizeof * SwitchPortHopCount * (numOfSwitches * numOfPorts));

    for(i = 0; i < numOfSwitches * numOfPorts; i++)
    {
      SwitchPortPID[i] = malloc( sizeof * SwitchPortPID[i] * (2*BUFFER_SIZE + 3));//Nhan 2 vi chua ca
                      //inport va outport, ngoai ra luu tru ID cua link, ID cua current Node 
                      //va ID cua destination node
      SwitchPortSrcIDs[i] = malloc( sizeof * SwitchPortSrcIDs[i] * (2*BUFFER_SIZE));
      SwitchPortDstIDs[i] = malloc( sizeof * SwitchPortDstIDs[i] * (2*BUFFER_SIZE));
      SwitchPortHopCount[i] = malloc( sizeof * SwitchPortHopCount[i] * (2*BUFFER_SIZE));
      for(j = 0; j < 2*BUFFER_SIZE; j++)
      {
        SwitchPortPID[i][j] = 0;
        SwitchPortSrcIDs[i] = 0;
        SwitchPortDstIDs[i] = 0;
        SwitchPortHopCount[i] = 0;
      }
      SwitchPortPID[i][j + 1] = 0;
      SwitchPortPID[i][j + 2] = 0;
    }

    

    //assignSwitchPackets(SwitchPortPID, Links, 
    //                          BUFFER_SIZE, numOfLinks, numOfPorts);

    char **SwitchEvtTypes = NULL;
    int **SwitchEvtTimes = NULL;//Co k inport, k outport, k channel tai 1 thoi diem
    //Vay co tong cong toi da k + 2*k + k = 4*k events tai 1 switch tai 1 thoi diem

    SwitchEvtTypes = malloc( sizeof * SwitchEvtTypes * (numOfSwitches * numOfPorts));
    SwitchEvtTimes = malloc( sizeof * SwitchEvtTimes * (numOfSwitches * numOfPorts));

    for(i = 0; i < numOfSwitches * numOfPorts; i++)
    {
      SwitchEvtTypes[i] = malloc(sizeof * SwitchEvtTypes[i] * (4));
      SwitchEvtTimes[i] = malloc(sizeof * SwitchEvtTimes[i] * (4));
      for(j = 0; j < 4; j++)
      {
        SwitchEvtTypes[i][j] = 0;
        SwitchEvtTimes[i][j] = 0;
      }
    }

    int **switchRT = NULL;
    

    int **Addresses = NULL;
    Addresses = malloc(sizeof * Addresses * (numOfHosts + numOfSwitches));
    for(i = 0; i < numOfSwitches + numOfHosts; i++)
    {
      Addresses[i] = malloc(sizeof * Addresses[i] * 4);
    }

    setAddresses(Addresses, numOfPorts);


    //assignEvents(SwitchEvtTypes, SwitchEvtTimes, numOfPorts, numOfSwitches);

    //showAddresses(Addresses, numOfHosts + numOfSwitches);

    
    showEvents(SwitchEvtTypes, SwitchEvtTimes, numOfPorts, numOfSwitches);

    
    int **HavingSuffix = NULL;
    int **HavingPrefix = NULL;
    int **HavingCorePrefix = NULL;
    
    HavingSuffix = malloc(sizeof * HavingSuffix * (numOfHosts + numOfSwitches));
    HavingPrefix = malloc(sizeof * HavingPrefix * (numOfHosts + numOfSwitches));
    HavingCorePrefix = malloc(sizeof * HavingCorePrefix * (numOfHosts + numOfSwitches));
    for(i = 0; i < numOfSwitches + numOfHosts; i++)
    {
      HavingSuffix[i] = malloc(sizeof * HavingSuffix[i] * 2);
      HavingSuffix[i][0] = 0; HavingSuffix[i][1] = 0;
      HavingPrefix[i] = malloc(sizeof * HavingPrefix[i] * 2);
      HavingPrefix[i][0] = 0; HavingPrefix[i][1] = 0;
      HavingCorePrefix[i] = malloc(sizeof * HavingCorePrefix[i] * 2);
      HavingCorePrefix[i][0] = 0; HavingCorePrefix[i][1] = 0;
    }


    int numOfSuffix = setHavingSuffix(HavingSuffix, numOfPorts);

    int numOfPrefix = setHavingPrefix(HavingPrefix, numOfPorts);

    int numOfCorePrefix = setHavingCorePrefix(HavingCorePrefix, numOfPorts);

    int **Suffix = NULL; int **Prefix = NULL;
    int **CorePrefix = NULL;
    Suffix = malloc(sizeof * Suffix * numOfSuffix);
    Prefix = malloc(sizeof * Prefix * numOfPrefix);
    CorePrefix = malloc(sizeof * CorePrefix * numOfCorePrefix);

    for(i = 0; i < numOfSuffix; i++)
    {
      Suffix[i] = malloc(sizeof * Suffix[i] * numOfPorts);
      for(j = 0; j < numOfPorts; j++)
      {
        Suffix[i][j] = 0; 
      }
    }

    for(i = 0; i < numOfPrefix; i++)
    {
      Prefix[i] = malloc(sizeof * Prefix[i] * (2 * numOfPorts));
      for(j = 0; j < 2*numOfPorts; j++)
      {
        Prefix[i][j] = 0; 
      }
    }

    for(i = 0; i < numOfCorePrefix; i++)
    {
      CorePrefix[i] = malloc(sizeof * CorePrefix[i] * (3 * numOfPorts));
      for(j = 0; j < 3*numOfPorts; j++)
      {
        CorePrefix[i][j] = 0; 
      }
    }

    setSuffix(Suffix, numOfPorts);
    setPrefix(Prefix, numOfPorts);

    setCorePrefix(CorePrefix, numOfPorts);

    /*showTwoLevelsRoutingTable(HavingSuffix, HavingPrefix, HavingCorePrefix
                        , Suffix, Prefix, CorePrefix, numOfPorts
                            );*/
   
    
    Graph graph ;
    graph = (Graph)malloc(sizeof(struct GRAPH));
    graph->numOfLinks = numOfLinks;
    graph->numOfHosts = numOfHosts;
    graph->numOfPorts = numOfPorts;
    graph->numOfSwitches = numOfSwitches;
    graph->BUFFER_SIZE = BUFFER_SIZE;
    graph->CYCLE_PACKET = 100;
    graph->RETRY_TIME = 3;
    graph->Links = Links;
    graph->IsHost = IsHost;
    graph->Hosts = Hosts;
    graph->SwitchIndexes = SwitchIndexes;
    graph->MapFromNodesToPorts = MapFromNodesToPorts;
    graph->Addresses = Addresses;
    graph->queues = sourceQueue;
    //showLinksInGraph(graph);
    //showHosts(graph);
    //showSwitchGraph(graph);
    //showAddresses(graph->Addresses, graph->numOfHosts + graph->numOfSwitches);
    //showPortOfSwitch(graph->MapFromNodesToPorts, graph->Links, 
    //                  graph->SwitchIndexes, graph->numOfLinks, graph->numOfPorts);
    
    RAlgorithm ra;
    ra = (RAlgorithm)malloc(sizeof(struct ROUTING_ALGORITHM));
    ra->numOfLinks = numOfLinks;
    ra->numOfHosts = numOfHosts;
    ra->numOfPorts = numOfPorts;
    ra->numOfSwitches = numOfSwitches;
    ra->SwitchEvtTypes = SwitchEvtTypes;
    ra->SwitchEvtTimes = SwitchEvtTimes; 
    ra->SwitchPortPID = SwitchPortPID; //Array stores IDs of packets in inport
    ra->SwitchPortSrcIDs = SwitchPortSrcIDs;
    ra->SwitchPortDstIDs = SwitchPortDstIDs;
    ra->HavingSuffix = HavingSuffix;
    ra->HavingPrefix = HavingPrefix;
    ra->HavingCorePrefix = HavingCorePrefix;
    ra->Suffix = Suffix; 
    ra->Prefix = Prefix;
    ra->CorePrefix = CorePrefix;

    TrafficPattern tp = NULL;
    int srcs[1] = {0}; int dsts[1] = {8};
    //tp = getTrafficPattern(srcs, dsts, 1);
    //show2LevelsRTable(ra);
    //int hopCount = 0;
    int *path = getNixVector(0, 8, ra, graph//, &hopCount 
                    );
    int curr = 0; //, stop = 2004;
    int numOfRepeat = 0;
    while (curr <= stop && 0)
    {
      int check = (curr - stop) >> (8*sizeof(int) - 1);//this calculation fixes the bug [BUG][SSiNET][00002]Can not execute at the stop time
      int minNextTime = stop + (1 + check);
      
      int minFromElements = runHosts(graph, path, curr);
      check = minNextTime - minFromElements;
      check = check >> (8*sizeof(int) - 1);//check = -1 nghia la minNextTime < x, 0 nghia la minNextTime >= x
      minNextTime = (1 + check)*minFromElements - check*minNextTime; 

      minFromElements = runLinks(graph, path, curr);
      check = minNextTime - minFromElements;
      check = check >> (8*sizeof(int) - 1);//check = -1 nghia la minNextTime < x, 0 nghia la minNextTime >= x
      minNextTime = (1 + check)*minFromElements - check*minNextTime; 
      curr = minNextTime;

      printf("\n\tNext curr finally is %d\n", curr);
    }
    
    showPacketsInHost(graph->Hosts, graph->queues, graph->numOfHosts, graph->BUFFER_SIZE);


    return 0;
}

