#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


void testAdjEdge(int k, int** adjEdge, int** adjAgg);


void testAdjEdge(int k, int** adjEdge, int** adjAgg) 
{
    int numOfPorts = k;
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


    int i, j;

    int numOfLinks = numOfSwitches * numOfPorts + numOfHosts;

    /*
    * Kiem tra ma tran lien ke cua cac Edge Switches
    */

    for(i = 0; i < numEdgeSwitches; i++)
    {
        for(j = 0; j < numOfPorts; j++)
        {
            if(j < numOfPorts / 2)
            {
                //adjEdge[i][j] ung voi Edge Switch thu i, o cong thu j 
                //se ket noi voi Switch hoac Host
                //adjEdge[i][j*3] = 0; //Cong thu j*3 se ket noi voi node co id adjEdge[i][j*3]
                if(adjEdge[i][j*3 + 1] != 0)//Gia tri 0 bieu thi day la host, 
                                        // 1 la Edge Switch, 2 la Agg Switch, 3 la Core Switch
                {
                    printf("\nInvalid: at small ports (index < %d) of Edge Switch, there must be a host.\n", 
                                (numOfPorts/2));
                }
                //adjEdge[i][j*3 + 2] = 0;//id cua phan tu trong mang SourceHosts, hoac Edge Switch hoac
                                        //trong Agg Switch hoac trong Core Switch
            }
            else{
                if(adjEdge[i][j*3 + 1] != 2)//Gia tri 0 bieu thi day la host, 
                                        // 1 la Edge Switch, 2 la Agg Switch, 3 la Core Switch
                {
                    printf("\nInvalid: at big ports (index > %d) of Edge Switch, there must be an aggre switch.\n", 
                                (numOfPorts/2));
                }
            }

            if(j > 0)
            {
                if(adjEdge[i][j*3] <= adjEdge[i][(j-1)*3])
                {
                    printf("\nInvalid: server with small number should locate in front of\n");
                }
            }
        }
    }


    for(i = 0; i < numEdgeSwitches; i++)
    {
        for(j = 0; j < numOfPorts; j++)
        {
            if(j < numOfPorts / 2)
            {
                if(adjAgg[i][j*3 + 1] != 1)//Gia tri 0 bieu thi day la host, 
                                        // 1 la Edge Switch, 2 la Agg Switch, 3 la Core Switch
                {
                    printf("\n%d Invalid: at small ports (index < %d) of Agg Switch, there must be an edge switch.\n", 
                                adjAgg[i][j*3 + 1], (numOfPorts/2));
                }
            }
        }
    }
}