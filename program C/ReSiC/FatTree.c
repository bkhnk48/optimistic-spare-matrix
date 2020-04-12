#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "ErrorReport.c"
#include "FatTreeTest.c"
//#include "FatTree.h"

int main(int argc, char** argv) 
{
    int numOfPorts = 4;
    int numOfSwitches = numOfPorts * numOfPorts * 5 / 4;
    int numPodSwitches = numOfPorts * numOfPorts;
    int numEdgeSwitches = numPodSwitches / 2;
    int numAggSwitches = numPodSwitches / 2;

    int numCoreSwitches = numOfPorts * numOfPorts / 4;

    int numOfHosts = numOfPorts * numOfPorts * numOfPorts / 4;
    int numOfInforAboutSwitch = 12;
    double bandwidth = 1; //bo nhan voi 1 ty, vi tu nay bandwith se tinh don vi Gb, 1000*1000*1000;
    int BUFFER_SIZE = 5;
    int PACKET_SIZE = 1e5;
    int serialTime = (int)PACKET_SIZE/bandwidth;
    double length = 1; //1m
    double PROPAGATION_VELOCITY = 0.2;
    int propagationTime = (int)(length/PROPAGATION_VELOCITY);


    //char *a = argv[1];
    //int stop = atoi(a);

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

    for(i = 0; i < numOfHosts; i++)
    {
        addServer[i] = malloc(sizeof * addServer[i] * 5);
        for(j = 0; j < 5; j++)
        {
            addServer[i][j] = 0;
        }
    }

    for(i = 0; i < numEdgeSwitches; i++)
    {
        addEdge[i] = malloc(sizeof * addEdge[i] * 5);
        for(j = 0; j < 5; j++)
        {
            addEdge[i][j] = 0;
        }
    }

    for(i = 0; i < numAggSwitches; i++)
    {
        addAgg[i] = malloc(sizeof * addAgg[i] * 5);
        for(j = 0; j < 5; j++)
        {
            addAgg[i][j] = 0;
        }
    }

    for(i = 0; i < numCoreSwitches; i++)
    {
        addCore[i] = malloc(sizeof * addCore[i] * 5);
        for(j = 0; j < 5; j++)
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
            //printf("switchID of Edge = %d\n", switchId);
            indexEdge = p*numOfPorts/2 + s;
            addEdge[indexEdge][0] = switchId;
            addEdge[indexEdge][1] = 10;
            addEdge[indexEdge][2] = p;
            addEdge[indexEdge][3] = s;
            addEdge[indexEdge][4] = 1;
            
            switchId += numOfPorts/2;
            
            addAgg[indexEdge][0] = switchId;
            addAgg[indexEdge][1] = 10;
            addAgg[indexEdge][2] = p;
            addAgg[indexEdge][3] = s + (numOfPorts / 2);
            addAgg[indexEdge][4] = 1;
            //addEdge[switchId] = new Address(10, p, s, 1);
        }
    }
    /*for (j = 1; j <= numOfPorts / 2; j++) {
        for (i = 1; i <= numOfPorts / 2; i++) {
            int offset = numPodSwitches + numServers;
            int switchId = offset + (j - 1) * numOfPorts / 2 + i - 1;
            address[switchId] = new Address(10, k, j, i);
        }
    }*/

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
                addServer[delta][1] = 10;
                addServer[delta][2] = p;
                addServer[delta][3] = e;
                addServer[delta][4] = h;
            }
        }
    }

    return 0;
}