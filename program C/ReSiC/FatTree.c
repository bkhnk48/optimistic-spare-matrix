#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "ErrorReport.c"
#include "FatTreeTest.c"
#include "Splay.c"
#include "State.c"
//#include "FatTree.h"
enum TRAFFIC_PATTERN {ALL_TO_ALL, ONE_TO_ONE, BISECTION_BANDWIDTH};
int main(int argc, char** argv) 
{
    int trafficPattern = ONE_TO_ONE;
                         
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
    //int** adjEdge = NULL;
    //adjEdge = malloc(sizeof * adjEdge * numEdgeSwitches);
    int adjEdge[numEdgeSwitches][3*numOfPorts];

    for(i = 0; i < numEdgeSwitches; i++)
    {
        //adjEdge[i] = malloc(sizeof * adjEdge[i] * (3*numOfPorts));
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
    //int** adjAgg = NULL;
    //adjAgg = malloc(sizeof * adjAgg * numAggSwitches);
    int adjAgg[numAggSwitches][3*numOfPorts];

    for(i = 0; i < numAggSwitches; i++)
    {
        //adjAgg[i] = malloc(sizeof * adjAgg[i] * (3*numOfPorts));
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
    //int** adjCore = NULL;
    //adjCore = malloc(sizeof * adjCore * numCoreSwitches);
    int adjCore[numCoreSwitches][2*numOfPorts];

    for(i = 0; i < numCoreSwitches; i++)
    {
        //adjCore[i] = malloc(sizeof * adjCore[i] * (2*numOfPorts));
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

    //int** addServer = NULL;
    int addServer[numOfHosts][2];
    //int** addEdge = NULL;
    int addEdge[numEdgeSwitches][2];
    //int** addAgg = NULL;
    int addAgg[numAggSwitches][2];
    //int** addCore = NULL;
    int addCore[numCoreSwitches][2];

    //addServer = malloc(sizeof * addServer * numOfHosts);
    //addEdge = malloc(sizeof * addEdge * numEdgeSwitches);
    //addAgg = malloc(sizeof * addAgg * numAggSwitches);
    //addCore = malloc(sizeof * addAgg * numCoreSwitches);
    int numOfElemt = 2;

    //Khoi tao dia chi IP cho cac Server
    for(i = 0; i < numOfHosts; i++)
    {
        //addServer[i] = malloc(sizeof * addServer[i] * numOfElemt);
        //for(j = 0; j < numOfElemt; j++)
        {
            //addServer[i][j] = 0;
            addServer[i][0] = 0;
            addServer[i][1] = 0;
        }
    }

    //Khoi tao dia chi IP cho cac Edge
    for(i = 0; i < numEdgeSwitches; i++)
    {
        //addEdge[i] = malloc(sizeof * addEdge[i] * numOfElemt);
        //for(j = 0; j < numOfElemt; j++)
        {
            //addEdge[i][j] = 0;
            addEdge[i][0] = 0;
            addEdge[i][1] = 0;
        }
    }

    //Khoi tao dia chi IP cho cac Agg
    for(i = 0; i < numAggSwitches; i++)
    {
        //addAgg[i] = malloc(sizeof * addAgg[i] * numOfElemt);
        //for(j = 0; j < numOfElemt; j++)
        {
            //addAgg[i][j] = 0;
            addAgg[i][0] = 0;
            addAgg[i][1] = 0;
        }
    }

    //Khoi tao dia chi IP cho cac Core
    for(i = 0; i < numCoreSwitches; i++)
    {
        //addCore[i] = malloc(sizeof * addCore[i] * numOfElemt);
        //for(j = 0; j < numOfElemt; j++)
        {
            //addCore[i][j] = 0;
            addCore[i][0] = 0;
            addCore[i][1] = 0;
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
    int numOfSources = 0;
    int numOfDests = 0;
    int dstPerSrc = 0;

    switch(trafficPattern) 
    {
        case ALL_TO_ALL: 
                            numOfSources = numOfHosts; 
                            numOfDests = numOfHosts;
                            dstPerSrc = numOfSources - 1;
                            break;
        case ONE_TO_ONE: 
                            numOfSources = numOfHosts; 
                            numOfDests = numOfHosts;
                            dstPerSrc = 1;
                            break;
        case BISECTION_BANDWIDTH: 
                            numOfSources = numOfHosts / 2; 
                            numOfDests = numOfHosts / 2;
                            dstPerSrc = 1;
                            break;
    }

    WayHE = malloc(sizeof * WayHE * numOfSources);
    for(i = 0; i < numOfSources; i++)
    {
        WayHE[i] = malloc(sizeof * WayHE[i] * numWaysFields);
        
        WayHE[i][0] = i; //id cua nut nguon host
        WayHE[i][1] = i  / ( numOfPorts / 2);//id cua nut edge switch tiep theo
        WayHE[i][2] = NO_PACKET_TRANSFERABLE; //trang thai ban dau
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
    TimeGeneration = malloc(sizeof * TimeGeneration * numOfSources);
    
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
    /*int numOfDstPerSrc [3] = {0, 
                                numOfHosts - 1, //1: all-to-all
                                1 //2: bisection bandwidth
                                };
    int dstPerSrc = numOfDstPerSrc[trafficPattern];*/
    for(i = 0; i < numOfSources; i++)
    {
        trafficPairs[i] = malloc(sizeof * trafficPairs[i] * dstPerSrc);
        for(j = 0; j < dstPerSrc; j++)
        {
            trafficPairs[i][j] = i + numOfSources;
            //tam lam viec bisection bandwidth hang
        }
    }

    int** PacketInSQ = NULL;
    PacketInSQ = malloc(sizeof * PacketInSQ * numOfSources);
    int** PacketInEXBHost = NULL;
    PacketInEXBHost = malloc(sizeof * PacketInEXBHost * numOfSources);
    //int sizeOfSQ = (dstPerSrc + 2);
    //2 phan tu dau tien cua PacketInSQ luu tru id cua pkt dau tien
    //va id cua pkt cuoi cung trong danh sach Source Queue
    

    for(i = 0; i < numOfSources; i++)
    {
        PacketInSQ[i] = malloc(sizeof * PacketInSQ[i] * 2); //sizeOfSQ);
        //for(j = 0; j < sizeOfSQ; j++)
        //{
            PacketInSQ[i][0] = -1;
            PacketInSQ[i][1] = -1;
        //}

        PacketInEXBHost[i] = malloc(sizeof * PacketInEXBHost[i] * (2 /*+ BUFFER_SIZE*/));
        //for(j = 0; j < 2 + BUFFER_SIZE; j++)
        {
            //PacketInEXBHost[i][j] = -1;
            PacketInEXBHost[i][0] = -1;
            PacketInEXBHost[i][1] = -1;
            //PacketInEXBHost[i][0] : id cua pkt dau tien trong EXB cua host
            //PacketInEXBHost[i][1] : id cua pkt cuoi cung trong EXB cua host
        }
    }

    int currentTime = 0; int endTime = 300*1000;

    //Generate Tree for hosts
    Tree * rootHosts;
    rootHosts = NULL;              /* the empty tree */
    //Generate event A
    for(i = 0; i < numOfSources/*1*/; i++)
    {
        //I. Check time and then execute event A
        int createPacketNow = checkEqual(currentTime, TimeGeneration[i]);
        //0 means wont generate a packet right now. 
        //1 means yes.
     
        if(createPacketNow == 1)
        {
            rootHosts = add(A, //type A 
                                0, //packetID = 0
                                addServer[i][0], //location at this host in group of Nodes
                                i,//location in group of hosts
                                0, //startTime = 0 
                                0, //endTime = 0
                                rootHosts);
            
        }
    }

    //show(rootHosts);

    Tree * ev = (Tree *) malloc (sizeof (Tree));
    ev->endTime = -1;
    rootHosts = removeFirstEvent(ev, 
                                    rootHosts);
    show(rootHosts);

    while(currentTime <= endTime && ev->endTime != -1)
    {
        if(ev->endTime == currentTime)
        {
            i = ev->idElementInGroup;//Lay id cua host trong danh sach cac hosts
            j = ev->//idNode;//lay id cua node trong danh sach cac nodes
                    idElementInGroup;

            printf(
                "Event first is of type: %d, pktID = %d, location = %d, endTime = %d\n"
                    , ev->type, ev->//packetID,
                                   type , 
                    ev->//idNode, 
                        idElementInGroup,
                    ev->endTime
             );

            if(ev->type == A)
            {
                int isEmptySQ = -(PacketInSQ[i][0] >> 31);//kiem tra xem SQ co empty ko?
                //int indexOfUpdateSQ = 1 + (-PacketInSQ[i][0] + PacketInSQ[i][1] + 1);//cap nhat phan tu nao cua PacketInSQ?
                //indexOfUpdateSQ *= isEmptySQ;//cap nhat phan tu nao cua PacketInSQ?
                int idOfNewPkt = ev->type;
                                //packetID;//id cua packet trong host day
                int dstOfNewPkt = trafficPairs[i][idOfNewPkt % dstPerSrc];//destination of packet
                
                //ongoing work to change this element of array to temporary variable
                PacketInSQ[i][1 - isEmptySQ] = idOfNewPkt;
                //PacketInSQ[i][indexOfUpdateSQ] = dstOfNewPkt;

                rootHosts = add(A, //type A
                            idOfNewPkt + 1, //packetID 
                            j,
                            i, //location at this host
                            currentTime + HOST_DELAY, //startTime = currentTime + HOST_DELAY
                            currentTime + HOST_DELAY, //endTime = currentTime + HOST_DELAY 
                            rootHosts);
                //show(rootHosts);

                //II. Generate and execute the event B
                //check if the EXB has no empty slot:
                int isNotFullEXB = PacketInEXBHost[i][1] - PacketInEXBHost[i][0] + 1 - BUFFER_SIZE;
                isNotFullEXB = -(isNotFullEXB >> 31); //1 nghia la EXB chua full, 0 nghia la EXB da full.
                
                if(isNotFullEXB == 1)//event B co the duoc tao ra
                {
                    rootHosts = add(B, //type B
                            idOfNewPkt, //packetID 
                            j,
                            i, //location at this host
                            currentTime, //startTime = currentTime 
                            currentTime, //endTime = currentTime (right now)
                            rootHosts);
                    //show(rootHosts);
                }
            }else if(ev->type == B)
            {
                int isEmptyEXB = -(PacketInEXBHost[i][0] >> 31);//chi nhan gia tri 0 hoac 1.
                int indexOfUpdate = (1 - isEmptyEXB);
                PacketInEXBHost[i][indexOfUpdate] = PacketInSQ[i][0];
                ///III. Remove the first packet of Source queue
                //After insert the packet into the empty slot of EXB (in host)
                //all remaining packets in source queue need to be moved forward
                ///If there was only one packet in SQ
                int isOnePkt = -(PacketInSQ[i][1] >> 31); 
                //0 nghia la co hon 1 goi tin trong SQ. 1 nghia la chi co 1 goi tin trong SQ
                //2 cau lenh duoi day cap nhat lai id cua cac goi tin trong SQ
                if(isOnePkt)
                {
                    PacketInSQ[i][0] = -1;    
                }
                else{
                    PacketInSQ[i][0] = PacketInSQ[i][1];

                }
                //PacketInSQ[i][0] = -isOnePkt + (1-isOnePkt)*(PacketInSQ[i][0] + 1);
                //PacketInSQ[i][1] = -isOnePkt + (1-isOnePkt)*(PacketInSQ[i][1]);

                ///Kiem tra xem co tao event C duoc hay khong?
                //Cach kiem tra: CounterH > 0 va WayHE[i][2] != 1.
                if(CounterH[i] > 0)
                {
                    if(WayHE[i][2] == NO_PACKET_TRANSFERABLE)
                    {
                        rootHosts = add(C, //type B
                                    PacketInEXBHost[i][0], //packetID 
                                    j,
                                    i, //location of this host
                                    currentTime, //startTime = currentTime 
                                    currentTime, //endTime = currentTime (right now)
                                    rootHosts);
                        
                    }
                }
                
                
            }else if(ev->type == C)
            {
                CounterH[i]--;
                WayHE[i][2] = HAS_PACKET;
            }

            
            //show(rootHosts);
        }

        ev->endTime = -1;
        //show(rootHosts);
        //printf("\n prepare remove 1st event for next loop\n");
        rootHosts = removeFirstEvent(ev, 
                                    rootHosts);
        int isNegativeEndTime = -(ev->endTime>>31);//0 hoac 1
        printf("Next event type %d is executed at time = %d", ev->type, ev->endTime);
        currentTime = (1 - isNegativeEndTime)*(ev->endTime) + 
                isNegativeEndTime*currentTime;
        printf("  the current time is %d\n", currentTime);
        //show(rootHosts);
    }

    return 0;
}