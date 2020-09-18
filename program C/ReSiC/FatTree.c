#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "ErrorReport.c"
#include "FatTreeTest.c"
#include "Splay.c"
//#include "FatTree.h"

int main(int argc, char** argv) 
{
    int trafficPattern = 2; //bisection bandwidth
                         //1; //all-to-all
    int numOfPorts = 4;
    int numOfSwitches = numOfPorts * numOfPorts * 5 / 4;
    int numPodSwitches = numOfPorts * numOfPorts;
    int numEdgeSwitches = numPodSwitches / 2;
    int numAggSwitches = numPodSwitches / 2;

    int numCoreSwitches = numOfPorts * numOfPorts / 4;

    int numOfHosts = numOfPorts * numOfPorts * numOfPorts / 4;
    int numOfInforAboutSwitch = 12;
    double bandwidth = 1; //bo nhan voi 1 ty, vi tu nay bandwidth se tinh don vi Gb, 1000*1000*1000;
    int BUFFER_SIZE = 5;
    int PACKET_SIZE = 1e5;
    int HOST_DELAY = 1e5; //ns
    int serialTime = (int)PACKET_SIZE/bandwidth;
    double length = 1; //1m
    double PROPAGATION_VELOCITY = 0.2;
    int propagationTime = (int)(length/PROPAGATION_VELOCITY);
    int numPktPerHalfSec = 1e9 / (2*HOST_DELAY);
    
    char *nP = argv[1];
    int numOfParallelProcessors = atoi(nP);

    int i, j;

    //De luu thong tin ve link va cac goi du lieu truyen tren link
    //cung nhu cac su kien xay ra tren link
    //ta dung mang 1 chieu nhu sau:
    //  tren mot chieu se co id cua nut nguon, id cua nut dich, id cua goi tin, id cua Host nguon,
    //  id cua Host dich, nix-vector (?), kich thuoc goi tin, bit dau tien cua goi tin se den vao luc nao,
    //  toan bo goi tin se den nut dich khi nao?, bang thong
    int countOfInfoInLink = 13;
    int numOfLinks = numOfSwitches * numOfPorts + numOfHosts;

    

    /*
    * Khai bao va khoi tao ma tran lien ke cho cac Edge Switches
    */
    int** adjEdge = NULL;
    adjEdge = malloc(sizeof * adjEdge * numEdgeSwitches);

    for(i = 0; i < numEdgeSwitches; i++)
    {
        adjEdge[i] = malloc(sizeof * adjEdge[i] * (3*numOfPorts));
        for(j = 0; j < numOfPorts; j++)
        {
            //adjEdge[i][j] ung voi Edge Switch thu i, o cong thu j 
            //se ket noi voi Switch hoac Host
            adjEdge[i][j*3] = 0; //Cong thu j*3 se ket noi voi node co id adjEdge[i][j*3]
            adjEdge[i][j*3 + 1] = 0;//Gia tri 0 bieu thi day la host, 
                                    // 1 la Edge Switch, 2 la Agg Switch, 3 la Core Switch
            adjEdge[i][j*3 + 2] = 0;//id cua phan tu trong mang SourceHosts, hoac Edge Switch hoac
                                    //trong Agg Switch hoac trong Core Switch
        }
    }

    /*
    * Khai bao va khoi tao ma tran lien ke cho cac Aggregasive Switches
    */
    int** adjAgg = NULL;
    adjAgg = malloc(sizeof * adjAgg * numAggSwitches);

    for(i = 0; i < numAggSwitches; i++)
    {
        adjAgg[i] = malloc(sizeof * adjAgg[i] * (3*numOfPorts));
        for(j = 0; j < numOfPorts; j++)
        {
            //adjAgg[i][j] ung voi Agg Switch thu i, o cong thu j 
            //se ket noi voi Switch hoac Host
            adjAgg[i][j*3] = 0; //Cong thu j*3 se ket noi voi node co id adjAgg[i][j*3]
            adjAgg[i][j*3 + 1] = 0;//Gia tri 0 bieu thi node lang gieng day la host, 
                                    // 1 la Edge Switch, 2 la Agg Switch, 3 la Core Switch
            adjAgg[i][j*3 + 2] = 0;//id cua phan tu trong mang Hosts, hoac Edge Switch hoac
                                    //trong Agg Switch hoac trong Core Switch
        }
    }

    /*
    * Khai bao va khoi tao ma tran lien ke cho cac Core Switches
    */
    int** adjCore = NULL;
    adjCore = malloc(sizeof * adjCore * numCoreSwitches);

    for(i = 0; i < numCoreSwitches; i++)
    {
        adjCore[i] = malloc(sizeof * adjCore[i] * (2*numOfPorts));
        for(j = 0; j < numOfPorts; j++)
        {
            //adjCore[i][j] ung voi Core Switch thu i, o cong thu j 
            //se ket noi voi Switch hoac Host
            adjCore[i][j*2] = 0; //Cong thu j*2 se ket noi voi node co id adjCore[i][j*2]
            adjCore[i][j*2 + 1] = 0;//id cua phan tu trong mang Agg Switch
        }
    }

    

    // each pod has numOfPorts^2/4 servers and numOfPorts switches
    int numEachPod = numOfPorts * numOfPorts / 4 + numOfPorts;
    int p, e, offset;
    int indexRow; //, indexCol ;
    int indexAgg;
    int indexEdge;
    for (p = 0; p < numOfPorts; p++) {
        offset = numEachPod * p;
        int a;
        

        // between server and edge
        for (e = 0; e < numOfPorts / 2; e++) {
            int edgeSwitch = offset + numOfPorts * numOfPorts / 4 + e;
            indexEdge = e + (p*numOfPorts/2);
            
            for (int s = 0; s < numOfPorts / 2; s++) {
                int server = offset + e * numOfPorts / 2 + s;
                //addEdge(edgeSwitch, server);
                
                adjEdge[indexEdge][s*3] = server;
                adjEdge[indexEdge][s*3 + 1] = 0;
                adjEdge[indexEdge][s*3 + 2] = s;
            }
            
            indexAgg = p*numOfPorts/2;

            // between agg and edge
            for (a = numOfPorts / 2; a < numOfPorts; a++) {
                int aggSwitch = offset + numOfPorts * numOfPorts / 4 + a;
                //addEdge(edgeSwitch, aggSwitch);
                adjEdge[indexEdge][a*3] = aggSwitch;
                adjEdge[indexEdge][a*3 + 1] = 2;
                adjEdge[indexEdge][a*3 + 2] = indexAgg;
                
                adjAgg[indexAgg][e*3] = edgeSwitch;
                adjAgg[indexAgg][e*3 + 1] = 1;
                adjAgg[indexAgg][e*3 + 2] = indexEdge;
                indexAgg++;
            }

            
        }

        indexAgg = p*numOfPorts/2;
        
        // between agg and core
        int c;
        for (a = 0; a < numOfPorts / 2; a++) {
            int aggSwitch = offset + numOfPorts * numOfPorts / 4 + numOfPorts / 2 + a;
            
            for (c = 0; c < numOfPorts / 2; c++) {
                int coreSwitch = a * numOfPorts / 2 + c + numPodSwitches + numOfHosts;
                int d = (c + numOfPorts/2)*3;
                //addEdge(aggSwitch, coreSwitch);
                adjAgg[indexAgg][d] = coreSwitch;
                adjAgg[indexAgg][d + 1] = 3;
                adjAgg[indexAgg][d + 2] = c;

                indexRow = a * numOfPorts / 2 + c;
                //indexCol = p;
                adjCore[indexRow][p*2] = aggSwitch;
                adjCore[indexRow][p*2 + 1] = indexAgg;
            }
            indexAgg++;
        }
    }

    //testAdjEdge(4, adjEdge, adjAgg, adjCore);

    int** addServer = NULL;
    int** addEdge = NULL;
    int** addAgg = NULL;
    int** addCore = NULL;

    addServer = malloc(sizeof * addServer * numOfHosts);
    addEdge = malloc(sizeof * addEdge * numEdgeSwitches);
    addAgg = malloc(sizeof * addAgg * numAggSwitches);
    addCore = malloc(sizeof * addAgg * numCoreSwitches);
    int numOfElemt = 2;

    //Khoi tao dia chi IP cho cac Server
    for(i = 0; i < numOfHosts; i++)
    {
        addServer[i] = malloc(sizeof * addServer[i] * numOfElemt);
        for(j = 0; j < numOfElemt; j++)
        {
            addServer[i][j] = 0;
        }
    }

    //Khoi tao dia chi IP cho cac Edge
    for(i = 0; i < numEdgeSwitches; i++)
    {
        addEdge[i] = malloc(sizeof * addEdge[i] * numOfElemt);
        for(j = 0; j < numOfElemt; j++)
        {
            addEdge[i][j] = 0;
        }
    }

    //Khoi tao dia chi IP cho cac Agg
    for(i = 0; i < numAggSwitches; i++)
    {
        addAgg[i] = malloc(sizeof * addAgg[i] * numOfElemt);
        for(j = 0; j < numOfElemt; j++)
        {
            addAgg[i][j] = 0;
        }
    }

    //Khoi tao dia chi IP cho cac Core
    for(i = 0; i < numCoreSwitches; i++)
    {
        addCore[i] = malloc(sizeof * addCore[i] * numOfElemt);
        for(j = 0; j < numOfElemt; j++)
        {
            addCore[i][j] = 0;
        }
    }

    int switchId = 0;
    int h, s;

    for (p = 0; p < numOfPorts; p++) {
        offset = numEachPod * p;
        for (s = 0; s < numOfPorts/2; s++) {
            switchId = offset + numOfPorts * numOfPorts / 4 + s;
            //Gan dia chi IP cho cac Edge
            indexEdge = p*numOfPorts/2 + s;
            addEdge[indexEdge][0] = switchId;
            addEdge[indexEdge][1] = (10 << 24);
            addEdge[indexEdge][1] |= (p << 16);
            addEdge[indexEdge][1] |= (s << 8);
            addEdge[indexEdge][1] |= 1;
            
            switchId += numOfPorts/2;
            
            //Gan dia chi IP cho cac Agg
            addAgg[indexEdge][0] = switchId;
            addAgg[indexEdge][1] = (10 << 24);
            addAgg[indexEdge][1] |= (p << 16);
            addAgg[indexEdge][1] |= ((s + numOfPorts / 2) << 8);
            addAgg[indexEdge][1] |= 1;
            //addEdge[switchId] = new Address(10, p, s, 1);
        }
    }
   

    int serverId = 0;
    int delta = 0;
    // address for servers
    for (p = 0; p < numOfPorts; p++) {
        int offset = numEachPod * p;
        for (e = 0; e < numOfPorts / 2; e++) {
            for (h = 2; h <= numOfPorts / 2 + 1; h++) {
                serverId = offset + e * numOfPorts / 2 + h - 2;
                delta = numOfPorts*p*(1 + ((serverId - numOfPorts) >> 31));
                //int delta = serverId - p*numOfPorts;
                //printf("%d, server ID = %d, delta = %d, p = %d, e = %d, h = %d\n", 
                //                serverId - delta, serverId, delta, p, e, h);
                delta = serverId - delta;
                addServer[delta][0] = serverId; //new Address(10, p, e, h);
                addServer[delta][1] = (10 << 24);
                addServer[delta][1] |= (p << 16);
                addServer[delta][1] |= (e << 8);
                addServer[delta][1] |= h;
            }
        }
    }

    //addresses for Core switches
    for (int j = 1; j <= numOfPorts / 2; j++) {
        for (int i = 1; i <= numOfPorts / 2; i++) {
            offset = numPodSwitches + numOfHosts;
            indexRow = (j - 1) * numOfPorts / 2 + i - 1;
            switchId = offset + indexRow;
            //address[switchId] = new Address(10, k, j, I);
            addCore[indexRow][0] = switchId;
            addCore[indexRow][1] = (10 << 24);
            addCore[indexRow][1] |= (numOfPorts << 16);
            addCore[indexRow][1] |= (j << 8);
            addCore[indexRow][1] |= i;
        }
    }

    
    //testAddresses(numOfPorts, addEdge, addAgg, addCore, addServer);

    //==========Build table=========//
    int** edgeTables = NULL;
    int** coreTables = NULL;
    int** aggSuffixTables = NULL;
    int** aggPrefixTables = NULL;
    int suffix, prefix;

    edgeTables = malloc(sizeof * edgeTables * numEdgeSwitches);
    for(i = 0; i < numEdgeSwitches; i++)
    {
        edgeTables[i] = malloc(sizeof * edgeTables[i] * (numOfPorts/2 + 2));
        for(j = 0; j < (numOfPorts/2); j++)
        {
            edgeTables[i][j] = 0;
        }
    }

    coreTables = malloc(sizeof * coreTables * numCoreSwitches);
    for(i = 0; i < numCoreSwitches; i++)
    {
        coreTables[i] = malloc(sizeof * coreTables[i] * numOfPorts);
        for(j = 0; j < numOfPorts; j++)
        {
            coreTables[i][j] = 0;
        }
    }

    aggSuffixTables = malloc(sizeof * aggSuffixTables * numAggSwitches);
    for(i = 0; i < numAggSwitches; i++)
    {
        aggSuffixTables[i] = malloc(sizeof * aggSuffixTables[i] * (numOfPorts/2 + 2));
        for(j = 0; j < (numOfPorts/2); j++)
        {
            aggSuffixTables[i][j] = 0;
        }
    }

    aggPrefixTables = malloc(sizeof * aggPrefixTables * numAggSwitches);
    for(i = 0; i < numAggSwitches; i++)
    {
        aggPrefixTables[i] = malloc(sizeof * aggPrefixTables[i] * (numOfPorts) /2 );
        for(j = 0; j < numOfPorts/2; j++)
        {
            aggPrefixTables[i][j] = 0;
        }
    }

    //build suffix table for edge switch
    for (indexEdge = 0; indexEdge < numEdgeSwitches; indexEdge++) {
        edgeTables[indexEdge][0] = 0;
        edgeTables[indexEdge][1] = 0;
        for(suffix = 2; suffix <= (numOfPorts/2) + 1; suffix++)
        {
            edgeTables[indexEdge][suffix] = 1;
        }
    }

    //build suffix table for core switch
    for(i = 0; i < numCoreSwitches; i++)
    {
        for(p = 0; p < numOfPorts; p++)
        {
            suffix = (10 << 8) | p;
            coreTables[i][p] = suffix;
        }
    }

    //build suffix and prefix table for agg switch 
    for(i = 0; i < numAggSwitches; i++)
    {
        aggSuffixTables[i][0] = 0;
        aggSuffixTables[i][1] = 0;
        
        for(suffix = 2; suffix <= (numOfPorts/2) + 1; suffix++)
        {
            aggSuffixTables[i][suffix] = 1;
        }
        p = i / (numOfPorts/2);
        for(e = 0; e < numOfPorts/2; e++)
        {
            prefix = (10 << 16) | (p << 8) | e;
            aggSuffixTables[i][e] = prefix;
        }
    }

    int numWaysFields = 9;//(1) id nut nguon; (2) id cua nut switch tiep theo
                          /*(3) trang thai ban dau
                            (4) cong k cua switch ke tiep
                            (5) id cua packet ben trong
                            (6) ip cua nut host nguon
                            (7) ip cua nut dich host
                            (8) so hop count
                            (9) thoi gian ket thuc cua su kien goi tin di tren link
                          */
    int** WayHE = NULL;
    int numOfSources = numOfHosts / trafficPattern;
    WayHE = malloc(sizeof * WayHE * numOfSources);
    for(i = 0; i < numOfSources; i++)
    {
        WayHE[i] = malloc(sizeof * WayHE[i] * numWaysFields);
        
        WayHE[i][0] = i; //id cua nut nguon host
        WayHE[i][1] = i  / ( numOfPorts / 2);//id cua nut edge switch tiep theo
        WayHE[i][2] = 0; //trang thai ban dau
        WayHE[i][3] = i % (numOfPorts / 2);//cong k cua edge switch ke tiep
            //ngam dinh rang edge switch co cong voi chi so k <= (numOfPorts/2) se 
            //ket noi voi host. Nguoc lai se ket noi voi agg switch
        WayHE[i][4] = 0;//id cua packet ben trong
        WayHE[i][5] = addServer[i][1];//ip cua nut nguon host
        WayHE[i][6] = 0; //ip cua nut dich host
        WayHE[i][7] = 0; //So hop count
        WayHE[i][8] = 0;//thoi gian ket thuc cua su kien

    }

    //testWayHE(numOfPorts, trafficPattern, WayHE);

    int** WayEH = NULL;
    int numOfDests = numOfHosts / trafficPattern;
    delta = (numEdgeSwitches / trafficPattern)*(trafficPattern - 1);
    WayEH = malloc(sizeof * WayEH * numOfDests);
    for(i = 0; i < numOfDests; i++)
    {
        WayEH[i] = malloc(sizeof * WayEH[i] * numWaysFields);
        
        WayEH[i][0] = i / ( numOfPorts / 2); // + delta; //id cua nut switch
        WayEH[i][1] = i;//id cua nut host tiep theo
        WayEH[i][2] = 0; //trang thai ban dau
        WayEH[i][3] = 0;//cong k cua host ke tiep
            //ngam dinh rang edge switch co cong voi chi so k <= (numOfPorts/2) se 
            //ket noi voi host. Nguoc lai se ket noi voi agg switch
        WayEH[i][4] = 0;//id cua packet ben trong
        WayEH[i][5] = 0;//ip cua nut nguon host cua goi tin
        WayEH[i][6] = 0; //ip cua nut dich host
        WayEH[i][7] = 0; //So hop count
        WayEH[i][8] = 0;//thoi gian ket thuc cua su kien

    }

    int** WayEA = NULL;
    int numWayEA = (numEdgeSwitches * (numOfPorts/2));
    WayEA = malloc(sizeof * WayEA * numWayEA);
    for(i = 0; i < numWayEA; i++)
    {
        WayEA[i] = malloc(sizeof * WayEA[i] * numWaysFields);

        WayEA[i][0] = i / (numOfPorts/2);//id cua nut switch
        WayEA[i][1] = 
                    (i / (numOfPorts*numOfPorts/4))//moi k pod, co chua k*k/4 link
                     + (i % (numOfPorts/2));//id cua nut agg
        WayEA[i][2] = 0; //trang thai ban dau
        WayEA[i][3] = (i / (numOfPorts/2)) % (numOfPorts / 2);//cong k cua agg switch ke tiep
            //ngam dinh rang edge switch co cong voi chi so k <= (numOfPorts/2) se 
            //ket noi voi host. Nguoc lai se ket noi voi agg switch
        WayEA[i][4] = 0;//id cua packet ben trong
        WayEA[i][5] = 0;//ip cua nut nguon host cua goi tin
        WayEA[i][6] = 0; //ip cua nut dich host
        WayEA[i][7] = 0; //So hop count
        WayEA[i][8] = 0;//thoi gian ket thuc cua su kien

    }

    //testWayEA(numOfPorts, WayEA);

    int** WayAE = NULL;
    int numWayAE = numAggSwitches * (numOfPorts/2);
    WayAE = malloc(sizeof * WayAE * numWayAE);
    for(i = 0; i < numWayAE; i++)
    {
        WayAE[i] = malloc(sizeof * WayAE[i] * numWaysFields);

        WayAE[i][0] = i / (numOfPorts/2);//id cua nut switch
        WayAE[i][1] = 
                    (i / (numOfPorts*numOfPorts/4))//moi k pod, co chua k*k/4 link
                     + (i % (numOfPorts/2));//id cua nut edge
        WayAE[i][2] = 0; //trang thai ban dau
        WayAE[i][3] = //(i / (numOfPorts/2)) % (numOfPorts / 2);
                ((i / (numOfPorts/2)) % (numOfPorts/2)) + (numOfPorts/2);
                //cong k cua edge switch ke tiep
            //ngam dinh rang edge switch co cong voi chi so k < (numOfPorts/2) se 
            //ket noi voi host. Nguoc lai se ket noi voi agg switch
        WayAE[i][4] = 0;//id cua packet ben trong
        WayAE[i][5] = 0;//ip cua nut nguon host cua goi tin
        WayAE[i][6] = 0; //ip cua nut dich host
        WayAE[i][7] = 0; //So hop count
        WayAE[i][8] = 0;//thoi gian ket thuc cua su kien
    }

    //testWayAE(numOfPorts, WayAE);

    int** WayAC = NULL;
    int numWayAC = numAggSwitches * (numOfPorts/2);
    WayAC = malloc(sizeof * WayAC * numWayAC);
    for(i = 0; i < numWayAC; i++)
    {
        WayAC[i] = malloc(sizeof * WayAC[i] * numWaysFields);

        WayAC[i][0] = i / (numOfPorts/2);//id cua nut switch
        WayAC[i][1] = 
                    //(i / (numOfPorts / 2))
                    //moi k pod, co chua k*k/4 link
                    //moi link nay se ket noi mot core khac nhau
                    //+ (i % (numOfPorts/2));//id cua nut core
                    i % (numOfPorts*numOfPorts/4);
        WayAC[i][2] = 0; //trang thai ban dau
        WayAC[i][3] = //(i / (numOfPorts/2)) % (numOfPorts / 2);
                (i / (numOfPorts * numOfPorts / 4));
                //(i % (numOfPorts*numOfPorts/4));
                //cong k cua core switch ke tiep
            //ngam dinh rang agg switch co cong voi chi so k < (numOfPorts/2) se 
            //ket noi voi edge. Nguoc lai se ket noi voi core switch
        WayAC[i][4] = 0;//id cua packet ben trong
        WayAC[i][5] = 0;//ip cua nut nguon host cua goi tin
        WayAC[i][6] = 0; //ip cua nut dich host
        WayAC[i][7] = 0; //So hop count
        WayAC[i][8] = 0;//thoi gian ket thuc cua su kien
    }

    //testWayAC(numOfPorts, WayAC);


    int** WayCA = NULL;
    int numWayCA = numCoreSwitches * (numOfPorts);
    WayCA = malloc(sizeof * WayCA * numWayCA);
    for(i = 0; i < numWayCA; i++)
    {
        WayCA[i] = malloc(sizeof * WayCA[i] * numWaysFields);

        WayCA[i][0] = i / (numOfPorts);//id cua nut core switch
        WayCA[i][1] = 
                    //id cua nut agg
                    (i % numOfPorts)*(numOfPorts/2) + (2*i / (numOfPorts * numOfPorts));
        WayCA[i][2] = 0; //trang thai ban dau
        WayCA[i][3] = //(i / (numOfPorts/2)) % (numOfPorts / 2);
                ((i / (numOfPorts)) % (numOfPorts / 2)) + (numOfPorts / 2);
                //(i % (numOfPorts*numOfPorts/4));
                //cong k cua agg switch ket noi
            //ngam dinh rang agg switch co cong voi chi so k < (numOfPorts/2) se 
            //ket noi voi edge. Nguoc lai se ket noi voi core switch
        WayCA[i][4] = 0;//id cua packet ben trong
        WayCA[i][5] = 0;//ip cua nut nguon host cua goi tin
        WayCA[i][6] = 0; //ip cua nut dich host
        WayCA[i][7] = 0; //So hop count
        WayCA[i][8] = 0;//thoi gian ket thuc cua su kien
    }

    //testWayCA(numOfPorts, WayCA);
    int* TimeGeneration = NULL;
    TimeGeneration = malloc(sizeof * TimeGeneration * (numOfHosts / trafficPattern));
    numOfSources = numOfHosts / trafficPattern;

    

    int* CounterH = NULL;
    CounterH = malloc(sizeof * CounterH * numOfSources);

    int** CounterE = NULL;
    CounterE = malloc(sizeof * CounterE * numEdgeSwitches);

    int** CounterA = NULL;
    CounterA = malloc(sizeof * CounterA * numAggSwitches);

    int** CounterC = NULL;

    

    CounterC = malloc(sizeof * CounterC * numCoreSwitches);

    for(i = 0; i < numEdgeSwitches; i++)
    {
        CounterE[i] = malloc(sizeof * CounterE[i] * numOfPorts); 
        for(j = 0; j < numOfPorts; j++)
        {
            CounterE[i][j] = BUFFER_SIZE;
        }
    }

    for(i = 0; i < numAggSwitches; i++)
    {
        CounterA[i] = malloc(sizeof * CounterA[i] * numOfPorts); 
        for(j = 0; j < numOfPorts; j++)
        {
            CounterA[i][j] = BUFFER_SIZE;
        }
    }

    for(i = 0; i < numCoreSwitches; i++)
    {
        CounterC[i] = malloc(sizeof * CounterC[i] * numOfPorts); 
        for(j = 0; j < numOfPorts; j++)
        {
            CounterC[i][j] = BUFFER_SIZE;
        }
    }

    for(i = 0; i < numOfSources; i++)
    {
        TimeGeneration[i] = 0;
        CounterH[i] = BUFFER_SIZE;
    }

    //Traffic pairs:
    int** trafficPairs = NULL;
    trafficPairs = malloc(sizeof * trafficPairs * numOfSources);
    int numOfDstPerSrc [3] = {0, 
                                numOfHosts - 1, //1: all-to-all
                                1 //2: bisection bandwidth
                                };
    int dstPerSrc = numOfDstPerSrc[trafficPattern];
    for(i = 0; i < numOfSources; i++)
    {
        trafficPairs[i] = malloc(sizeof * trafficPairs[i] * dstPerSrc);
        for(j = 0; j < dstPerSrc; j++)
        {
            trafficPairs[i][j] = i + numOfSources;//tam lam viec bisection bandwidth hang
        }
    }

    int** PacketInSQ = NULL;
    PacketInSQ = malloc(sizeof * PacketInSQ * numOfSources);
    int** PacketInEXBHost = NULL;
    PacketInEXBHost = malloc(sizeof * PacketInEXBHost * numOfSources);
    int sizeOfSQ = (dstPerSrc + 2);
    //2 phan tu dau tien cua PacketInSQ luu tru id cua pkt dau tien
    //va id cua pkt cuoi cung trong danh sach Source Queue
    //Cac phan tu tiep theo luu tru danh sach cac dst cua tung goi tin
    //trong Source Queue

    for(i = 0; i < numOfSources; i++)
    {
        PacketInSQ[i] = malloc(sizeof * PacketInSQ[i] * sizeOfSQ);
        for(j = 0; j < sizeOfSQ; j++)
        {
            PacketInSQ[i][j] = -1;
        }

        PacketInEXBHost[i] = malloc(sizeof * PacketInEXBHost[i] * (2 + BUFFER_SIZE));
        for(j = 0; j < 2 + BUFFER_SIZE; j++)
        {
            PacketInEXBHost[i][j] = -1;
            //PacketInEXBHost[i][0] : id cua pkt dau tien trong EXB cua host
            //PacketInEXBHost[i][1] : id cua pkt cuoi cung trong EXB cua host
            //PacketInEXBHost[i][j] : dst cua cac pkt trong EXB cua host
        }
    }

    int currentTime = 0; int endTime = 0;

    //Generate event A
    for(i = 0; i < numOfSources; i++)
    {

    }
    /*
    while(currentTime <= endTime)
    {
        //generate packets and update to source queue
        for(i = 0; i < numOfSources; i++)
        {
            //I. Check time and then execute event A
            int createPacketNow = checkEqual(currentTime, TimeGeneration[i]);
            //0 means wont generate a packet right now. 
            //1 means yes.
            switch(createPacketNow)
            {
                case 1:
                    int isEmptySQ = -(PacketInSQ[i][0] >> 31);
                    int indexOfUpdateSQ = 2 + (-PacketInSQ[i][0] + PacketInSQ[i][1] + 1);
                    indexOfUpdateSQ *= isEmptySQ;
                    int idOfNewPkt = currentTime / HOST_DELAY;
                    int dstOfNewPkt = trafficPairs[i][idOfNewPkt % dstPerSrc];
                    //indexOfUpdateSQ *= createPacketNow;
                    
                    
                    //ongoing work to change this element of array to temporary variable
                    PacketInSQ[i][1 - isEmptySQ] = idOfNewPkt;
                    

                    PacketInSQ[i][indexOfUpdateSQ] = dstOfNewPkt;

                    //II. Generate and execute the event B
                    //check if the EXB has no packet:
                    int indexOfUpdate = checkUpdateEXBHost(PacketInEXBHost[i][0]
                                                            , PacketInEXBHost[i][1]
                                                            , PacketInSQ[i][0]
                                                            , BUFFER_SIZE
                                                            );
                    int isFullEXB = indexOfUpdate & 1;
                    indexOfUpdate = indexOfUpdate >> 1;
                    //cap nhat lai xem SQ co bi empty khong?
                    isEmptySQ = - (PacketInSQ[i][0] >> 31);//0 nghia la co goi tin o vi tri dau tien. 1 nghia la SQ dang empty

                    int oldPktInEXBHost = PacketInEXBHost[i][indexOfUpdate];
                    
                    PacketInEXBHost[i][indexOfUpdate] = (isFullEXB)*PacketInEXBHost[i][indexOfUpdate] 
                                                + (1 - isFullEXB)*PacketInSQ[i][0]*(1 - isEmptySQ);
                    
                    ///III. Remove the first packet of Source queue
                    //After insert the packet into the empty slot of EXB (in host)
                    //all remaining packets in source queue need to be moved forward
                    
                    
                    ///If there was only one packet in SQ
                    int isOnePkt = - (PacketInSQ[i][2] >> 31); //0 nghia la co goi tin o vi tri cuoi cung. 1 nghia la chi co 1 goi tin trong SQ
                    int numPktInSQ = (PacketInSQ[i][2] - PacketInSQ[i][0] + 1)*(1 - isEmptySQ)*(1 - isOnePkt)
                                            + isOnePkt;
                    
                    int isMovedPktFromSQtoEXB = PacketInEXBHost[i][indexOfUpdate] - 
                                                    oldPktInEXBHost;
                    //this variable isMovedPktFromSQtoEXB is always greater than or equal 0.
                    //because the new value of PacketInEXBHost[i][indexOfUpdate]
                    //definitely greater than or equal the old one.
                    isMovedPktFromSQtoEXB--; //decrease so the new value might negative.
                                            //in case no packet is updated.
                    isMovedPktFromSQtoEXB = (isMovedPktFromSQtoEXB >> 31);
                                            //0 means a packet is moved from SQ to EXB
                                            //-1 means nothing is moved.
                    isMovedPktFromSQtoEXB++; //1 means a packet is moved from SQ to EXB
                                            //0 means nothing is moved.
                    break;
            }

            
        }
        currentTime++;
    }
    */

    return 0;
}