#include <stdio.h>
#include "GenerationOfPackets.c"

void checkOutPortOfSwitch(int aSwitch, int host, int **AdjOfSwitches,
        IntegratedPort **IntegratedPortOfSwitches);


void checkOutPortOfSwitch(int aSwitch, int host, int **AdjOfSwitches,
        IntegratedPort **IntegratedPortOfSwitches)
{

}

int* getNumOfPort(int host, int **AdjOfSwitches, int w, int h)
{
    int i = 0;
    int j = 0;
    int *result = NULL;
    result = (int*)calloc(2, sizeof(int));
    
    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
            if(AdjOfSwitches[i][j] == host)
            {
                result[0] = i; result[1] = j;
                return result;
            }
    }
    return NULL;
}