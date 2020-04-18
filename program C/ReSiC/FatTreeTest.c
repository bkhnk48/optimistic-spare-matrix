#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


void testAdjEdge(int k, int** adjEdge, int** adjAgg, int** adjCore);


void testAdjEdge(int k, int** adjEdge, int** adjAgg, int** adjCore) 
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
                    printf("\nInvalid: node with small number should locate in front of\n");
                }
                else{
                    printf("k = %d -> %d ", j, adjEdge[i][j*3]);
                }
            }
            else{
                printf("k = %d -> %d ", j, adjEdge[i][j*3]);
            }
        }
        printf("\n");
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
            else{
                if(adjAgg[i][j*3 + 1] != 3)//Gia tri 0 bieu thi day la host, 
                                        // 1 la Edge Switch, 2 la Agg Switch, 3 la Core Switch
                {
                    printf("\nadjAgg[%d][%d] = %d Invalid: at big ports (index = %d) of Agg Switch, there must be a core switch.\n", 
                                i, j*3 + 1, adjAgg[i][j*3 + 1], j);
                }
            }
        }
    }

    for(i = 0; i < numCoreSwitches; i++)
    {
        for(j = 0; j < numOfPorts; j++)
        {
            if(j > 0)
            {
                if(adjCore[i][j*2] <= adjCore[i][(j-1)*2])
                {
                    //printf("adjCore[%d][%d] = %d > adjCore[%d][%d] = %d\n"
                    //    , i, j*2, adjCore[i][j*2], i, (j-1)*2, adjCore[i][(j-1)*2]
                    //    );
                    printf("\nInvalid: agg switch with small number should locate in front of\n");
                }
            }
        }
    }
}

void testAddresses(int k, int** addEdge, int** addAgg, int** addCore, int** addServer)
{
    int numOfPorts = k;
    int numOfSwitches = numOfPorts * numOfPorts * 5 / 4;
    int numPodSwitches = numOfPorts * numOfPorts;
    int numEdgeSwitches = numPodSwitches / 2;
    int numAggSwitches = numPodSwitches / 2;

    int numCoreSwitches = numOfPorts * numOfPorts / 4;

    int numOfHosts = numOfPorts * numOfPorts * numOfPorts / 4;

    int i, j;
    for(i = 1; i < numEdgeSwitches; i++)
    {
        if(addEdge[i][0] < addEdge[i-1][0])
        {
            printf("\nInvalid: Edge switch that has small number ID should be located at small index\n");
            break;
        }
    }

    int t;
    int ip1, ip2, ip3, ip4;

    for(i = 0; i < numEdgeSwitches; i++)
    {
        t = addEdge[i][1];
        ip1 = t >> 24;
        ip2 = (t << 8) >> 24;
        ip3 = (t << 16) >> 24;
        ip4 = (t & 255);
        if(ip1 != 10 || (ip2 < 0 || ip2 >= numOfPorts) 
            || (ip3 < 0 || ip3 >= numOfPorts / 2)
            || ip4 != 1
                )
        {
            printf("The address of edge %d is not in correct format 10.p.s.1 as %d.%d.%d.%d\n"
                    , i, ip1, ip2, ip3, ip4
                    );
            break;

        }
        //else{
        //    printf("%d.%d.%d.%d\n"
        //            , ip1, ip2, ip3, ip4
        //            );
        //}
    }

    for(i = 1; i < numAggSwitches; i++)
    {
        if(addAgg[i][0] <= addAgg[i-1][0])
        {
            printf("\nInvalid: Agg switch that has small number ID should be located  at small index\n");
            break;
        }
    }

    for(i = 0; i < numAggSwitches; i++)
    {
        t = addAgg[i][1];
        ip1 = t >> 24;
        ip2 = (t << 8) >> 24;
        ip3 = (t << 16) >> 24;
        ip4 = (t & 255);
        if(ip1 != 10 || (ip2 < 0 || ip2 >= numOfPorts) 
            || (ip3 < numOfPorts / 2 || ip3 >= numOfPorts)
            || ip4 != 1
                )
        {
            printf("The address of agg %d is not in correct format 10.p.s.1 as %d.%d.%d.%d\n"
                    , i, ip1, ip2, ip3, ip4
                    );
            break;

        }
        //else{
        //    printf("%d.%d.%d.%d\n"
        //            , ip1, ip2, ip3, ip4
        //            );
        //}
    }

    for(i = 1; i < numCoreSwitches; i++)
    {
        if(addCore[i][0] <= addCore[i-1][0])
        {
            printf("\nInvalid: Core switch that has small number ID should be located  at small index\n");
            break;
        }
    }

    for(i = 0; i < numCoreSwitches; i++)
    {
        t = addCore[i][1];
        ip1 = t >> 24;
        ip2 = (t << 8) >> 24;
        ip3 = (t << 16) >> 24;
        ip4 = (t & 255);
        if(ip1 != 10 || (ip2 != numOfPorts) 
            || (ip3 < 1 || ip3 > numOfPorts /2)
            || ip4 < 1 || ip4 > numOfPorts / 2
                )
        {
            printf("The address of core switch %d is not in correct format 10.p.s.h as %d.%d.%d.%d\n"
                    , i, ip1, ip2, ip3, ip4
                    );
            break;

        }
        /*else
        {
            printf("%d.%d.%d.%d\n"
                    , ip1, ip2, ip3, ip4
                    );
        }*/
    }

    for(i = 1; i < numOfHosts; i++)
    {
        if(addServer[i][0] <= addServer[i-1][0])
        {
            printf("\nInvalid: Host that has small number ID should be located at small index\n");
            break;
        }
    }

    for(i = 0; i < numOfHosts; i++)
    {
        t = addServer[i][1];
        ip1 = t >> 24;
        ip2 = (t << 8) >> 24;
        ip3 = (t << 16) >> 24;
        ip4 = (t & 255);
        if(ip1 != 10 || (ip2 < 0 || ip2 >= numOfPorts) 
            || (ip3 < 0 || ip3 >= numOfPorts /2)
            || ip4 < 2 || ip4 > numOfPorts / 2 + 1
                )
        {
            printf("The address of server %d is not in correct format 10.p.s.h as %d.%d.%d.%d\n"
                    , i, ip1, ip2, ip3, ip4
                    );
            break;

        }
        /*else
        {
            printf("%d.%d.%d.%d\n"
                    , ip1, ip2, ip3, ip4
                    );
        }*/
    }
}