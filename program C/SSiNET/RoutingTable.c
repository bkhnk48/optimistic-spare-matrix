#include <stdio.h>

int setHavingSuffix(int **HavingSuffix, int numOfPorts)
{
    int p, e, a, index = 0;
    int numEachPod = numOfPorts * numOfPorts / 4 + numOfPorts;

    for (p = 0; p < numOfPorts; p++) {
        int offset = numEachPod * p;
        for (e = 0; e < numOfPorts / 2; e++) {
            int edgeSwitch = offset + numOfPorts * numOfPorts / 4 + e;
            HavingSuffix[edgeSwitch][0] = 1;//Chi co suffix (tuc la chi la edge switch)
            //printf("\nEdge switch: %d has index: %d", edgeSwitch, index);
            //int position = index;
            HavingSuffix[edgeSwitch][1] = index; //position;
            //printf("\nEdge switch: %d has index: %d or %d", edgeSwitch, index, HavingSuffix[edgeSwitch][1]);
            index++;
        }
    }

    for (p = 0; p < numOfPorts; p++) {
        int offset = numEachPod * p;
        for (int a = 0; a < numOfPorts / 2; a++) {
            int aggSwitch = offset + numOfPorts * numOfPorts / 4 + numOfPorts / 2 + a;
            HavingSuffix[aggSwitch][0] = 1;//Co suffix
            HavingSuffix[aggSwitch][1] = index;
            index++;
        }
    }

    return index;
}


int setHavingPrefix(int **HavingPrefix, int numOfPorts)
{
    int p, a, index = 0;
    int numEachPod = numOfPorts * numOfPorts / 4 + numOfPorts;
    for (p = 0; p < numOfPorts; p++) {
        int offset = numEachPod * p;
        for (int a = 0; a < numOfPorts / 2; a++) {
            int aggSwitch = offset + numOfPorts * numOfPorts / 4 + numOfPorts / 2 + a;
            HavingPrefix[aggSwitch][0] = 1;//Co prefix 
            HavingPrefix[aggSwitch][1] = index;
            index++;
        }
    }

    return index;
}

int setHavingCorePrefix(int **HavingCorePrefix, int numOfPorts)
{
    int c, index = 0;
    int numEachPod = numOfPorts * numOfPorts / 4 + numOfPorts;
    for (c = 0; c < numOfPorts * numOfPorts / 4; c++) {
        int core = numOfPorts * numOfPorts * numOfPorts / 4 + numOfPorts * numOfPorts + c;
        HavingCorePrefix[core][0] = 1; //Co core prefix
        HavingCorePrefix[core][1] = index;
        index++;
    }
    return index;
}


void setSuffix(int **Suffix, int numOfPorts)
{
    int p, e, a, suffix, index = 0;
    int k = numOfPorts;
    int numEachPod = numOfPorts * numOfPorts / 4 + numOfPorts;

    for (p = 0; p < numOfPorts; p++) {
        int offset = numEachPod * p;
        for (e = 0; e < numOfPorts / 2; e++) {
            int edgeSwitch = offset + numOfPorts * numOfPorts / 4 + e;
            int delta = 0;
            for (suffix = 2; suffix <= k / 2 + 1; suffix++) {
                int agg = offset + k * k / 4 + (e + suffix - 2) % (k / 2) + (k / 2);
                Suffix[index][delta] = suffix;
                delta ++;
                Suffix[index][delta] = agg;
                delta ++;
                //printf("\nSuffix[%d][%d] = %d, Suffix[%d][%d] = %d",
                //        index, delta - 2, Suffix[index][delta - 2], index, delta - 1, Suffix[index][delta - 1]
                //        );
            }
            index++;
        }
    }

    for (p = 0; p < numOfPorts; p++) {
        int offset = numEachPod * p;
        for (int a = 0; a < numOfPorts / 2; a++) {
            int aggSwitch = offset + numOfPorts * numOfPorts / 4 + numOfPorts / 2 + a;
            int delta = 0;
            for (suffix = 2; suffix <= k / 2 + 1; suffix++) {
                    int core = a * k / 2 + (suffix + a - 2) % (k / 2) + numEachPod * k;
                    Suffix[index][delta] = suffix;
                    delta++;
                    Suffix[index][delta] = core;
                    delta++;
                    //printf("\nSuffix[%d][%d] = %d, Suffix[%d][%d] = %d",
                    //    index, delta, suffix, index, delta, core
                    //    );
            }
            index++;
        }
    }
}


void setPrefix(int **Prefix, int numOfPorts)
{
    int p, e, a, suffix, index = 0;
    int k = numOfPorts;
    int numEachPod = numOfPorts * numOfPorts / 4 + numOfPorts;

    for (p = 0; p < numOfPorts; p++) {
        int offset = numEachPod * p;
        for (int a = 0; a < numOfPorts / 2; a++) {
            int aggSwitch = offset + numOfPorts * numOfPorts / 4 + numOfPorts / 2 + a;
            int delta = 0;
            for (e = 0; e < k / 2; e++) 
            {
                    int edgeSwitch = offset + k * k / 4 + e;
                    Prefix[index][delta]= 10;
                    Prefix[index][delta + 1]= p;
                    Prefix[index][delta + 2] = e;
                    Prefix[index][delta + 3] = edgeSwitch;
                    delta += 4;
            }
            index++;
            
        }


    }
}

void setCorePrefix(int **CorePrefix, int numOfPorts)
{
    int p, c, index = 0;
    int k = numOfPorts;
    int numEachPod = numOfPorts * numOfPorts / 4 + numOfPorts;
    for (c = 0; c < numOfPorts * numOfPorts / 4; c++) {
        int core = numOfPorts * numOfPorts * numOfPorts / 4 + numOfPorts * numOfPorts + c;
        int delta = 0;
        for(p = 0; p < k; p++) {
            int offset = numEachPod * p;
            int agg = (c / (k / 2)) + k / 2 + k * k / 4 + offset;
            CorePrefix[index][delta] = 10;
            CorePrefix[index][delta + 1] = p;
            CorePrefix[index][delta + 2] = agg;
            delta += 3;
        }
        index++;
    }

}


void showTwoLevelsRoutingTable(int **HavingSuffix, int **HavingPrefix, int **HavingCorePrefix
                        , int **Suffix, int **Prefix, int **CorePrefix, int numOfPorts
                            )
{
    int k = numOfPorts;
    int p = 0;
    int numOfSwitches = 5*k*k/4;
    int numOfHosts = k*k*k/4;
    int i, j, c, e, a;
    for(i = 0; i < numOfHosts + numOfSwitches; i++)
    {
        if(HavingSuffix[i][0] == 1)
        {
            int index = HavingSuffix[i][1];
            printf("\nAt %d", index);
            for(j = 0; j < k/2; j++)
            {
                printf("\n\tSwitch %d has Suffix Table %d reversed for %d", i, 
                    Suffix[index][j*2], Suffix[index][j*2 + 1]
                    );
            }
            //printf("\n\tSwitch %d has Suffix Table %d reversed for %d", i, 
            //        Suffix[index][2], Suffix[index][3]
            //        );
        }
    }

    printf("\n====================================\n");

    for(i = 0; i < numOfHosts + numOfSwitches; i++)
    {
        if(HavingPrefix[i][0] == 1)
        {
            for (j = 0; j < k / 2; j++) {
                printf("\nSwitch %d has PREfix Table (%d.%d.%d) reversed for %d", i, 
                    Prefix[HavingPrefix[i][1]][j*4], 
                    Prefix[HavingPrefix[i][1]][j*4 + 1], 
                    Prefix[HavingPrefix[i][1]][j*4 + 2]
                    , Prefix[HavingPrefix[i][1]][j*4 + 3]
                    );
            }
        }
    }

    printf("\n====================================\n");

    for(i = 0; i < numOfHosts + numOfSwitches; i++)
    {
        if(HavingCorePrefix[i][0] == 1)
        {
            c = HavingCorePrefix[i][1];
            printf("\nCoreprefix of the core id: %d", i);
            printf(" it has: \n");
            int delta = 0;
            for(p = 0; p < k; p++) {
                printf("\t %d.%d => %d\n", CorePrefix[c][delta]
                            ,CorePrefix[c][delta + 1]
                            , CorePrefix[c][delta + 2]
                );
                delta += 3;
            }
        }
    }
}