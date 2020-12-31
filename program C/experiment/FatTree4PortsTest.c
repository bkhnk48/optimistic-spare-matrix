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

}
