#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FatTreeGraph.c"
#include "FatTreeTables.c"
#include "FatTreeNode.c"
#include "OptimizationSplay.c"
#include "timing.c"
#include <locale.h>
#include <math.h>

int main(int argc, char** argv) {
    double wc1 = 0, wc2 = 0, cpuT = 0;
    int first = -1;
    int defaultSec = 1;
    int defaultBias = 1;
    ProcStatm proc_statm;
    long page_size = sysconf(_SC_PAGESIZE);
    int k = 4;

    if(argc >= 2)
    {
        k = atoi(argv[1]);
        defaultSec = atoi(argv[2]);
        if(argc >= 3)
            defaultBias = atoi(argv[3]);
    }
    unsigned long currentTime = 0;
    unsigned long endTime = defaultSec*((unsigned long)(1000*1000));

    unsigned long arr[384][7];//384 = 6*(k*k*k) as k = 4
    unsigned long i;
    int j, N, root = -1;
    for(i = 0; i < 384; i++)
    {
      for(j = 0; j < 7; j++)
      {
         arr[i][j] = -1L;
      }
    }
    setlocale(LC_NUMERIC, "");
   
    printf("Simulation time is %ld (s)\n", endTime / (1000*1000));
    unsigned long mem = mem_avail();
    printf("Free memory Available = %'ld\n", mem / (1024*1024));

    printf("Start Simulating ......\n");
    unsigned long count = 0;
    //char* p = malloc(1 * 1024 * 1024 * 1024);
    timing(&wc1, &cpuT);

    root = -1;

    return 0;

}
