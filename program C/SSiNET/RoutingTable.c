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
            HavingSuffix[edgeSwitch][0] = index;
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
    for (int c = 0; c < numOfPorts * numOfPorts / 4; c++) {
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