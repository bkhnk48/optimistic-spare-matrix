#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FatTreeGraph.c"
#include "FatTreeTables.c"
#include "HCalendarQueue.c"
#include "ControlFlow.c"
#include "timing.c"
#include "TestAPI.c"
#include "Throughput.c"
#include "PairStrategies.c"
#include "ProcessData.c"
#include <limits.h>
#include <locale.h>
#include <math.h>

int main(int argc, char **argv)
{
    double wc1 = 0, wc2 = 0, cpuT = 0;
    unsigned int first = UINT_MAX;
    int defaultSec = 1;
    int defaultBias = 0;
    ProcStatm proc_statm;
    long page_size = sysconf(_SC_PAGESIZE);
    int k = 4;
    int PACKET_SIZE = 96 * 100; //9.6KB
    int SWITCH_CYCLE = 10;
    //int packets_per_sec = 1000;//each sec generated 1000 packets
    int T = 100 * 1000;
    unsigned long BANDWIDTH_HOST = (unsigned long)96 * 1000 * 1000; //96MByte/s
    unsigned long BANDWIDTH_CORE = (unsigned long)10667 * 10 * 1000; //106.67MByte/s
    unsigned long loadingTime1 = ((unsigned long)PACKET_SIZE * 1000 * 1000 * 1000) / BANDWIDTH_HOST; //BANDWIDTH_HOST / PACKET_SIZE ;//+ 13;
    unsigned long loadingTime2 = ((unsigned long)PACKET_SIZE * 1000 * 1000 * 1000) / BANDWIDTH_CORE; //BANDWIDTH_HOST / PACKET_SIZE ;//+ 13;
    
    unsigned long **receivedPkts = NULL;
    int STEP = 100;
    enum PairStrategy stragegy = FORCE_TO_PAIR;
    
    unsigned long currentTime = ULLONG_MAX;
    
    if (argc >= 2)
    {
        k = atoi(argv[1]);
        if (argc > 2)
        defaultSec = atoi(argv[2]);
        if (argc > 3)
        defaultBias = atoi(argv[3]);
    }

    unsigned long endTime = defaultSec * ((unsigned long)(1000 * 1000 * 1000));
    unsigned long STEP_TIME = endTime / STEP;
    
    int numOfHosts = k * k * k / 4;
    unsigned long i, j;
    unsigned int N, root = UINT_MAX;
    
    PairPattern *pairs = NULL;
    pairs = malloc(numOfHosts * sizeof(PairPattern)); 
    //interpodIncomming(pairs, k);
    //forceToPair(pairs, numOfHosts, 1000);
    importPairs(pairs, "K8Pairs1.in");
    //importPairs(pairs, "K16Pairs1.in");
    //pairs[2].dst = 9;
    printfPairs(pairs, numOfHosts);

    receivedPkts = malloc(sizeof *receivedPkts * numOfHosts);
    for (i = 0; i < numOfHosts; i++){
        receivedPkts[i] = malloc(sizeof *receivedPkts[i] * (STEP + 1));
        for (j = 0; j < STEP + 1; j++){
        receivedPkts[i][j] = 0;
        }
    }

    Tables *tablesOfSwitches = malloc(sizeof(Tables));
    buildTables(tablesOfSwitches, k);
    BufferHost *bufferHosts = initBufferHosts(k * k * k / 4);
    BufferSwitch *bufferSwitches = initBufferSwitches(k * k * 5 / 4, k);
    assignTypeOfSwitch(bufferSwitches, k);
    NetworkNode *allNodes = initNetworkNodes(k * k * k / 4, 5 * k * k / 4, k);

    setlocale(LC_NUMERIC, "");
    
    int nextIP = 0;
    int nextIndex;
    int nextPort = 0;
    int generateEventB, generateEventC, generateEventD;
    int generateEventE, generateEventF, generateEventH;
    int generateEventH_HOST;
    int generateEventG;
    int idNodeInTree = 0;
    int numOfFlows = 0;
    int type;
    
    
    root = UINT_MAX;
    Flow *flows = malloc(numOfHosts * sizeof(Flow));
    for(i = 0; i < numOfHosts; i++){
        flows[i].indexOfDst = i;
        flows[i].srcIP = -1;
        flows[i].receivedPackets = malloc(STEP * sizeof(unsigned long));
        for (j = 0; j < STEP; j++)
        {
        flows[i].receivedPackets[j] = 0;
        }
    }

    //FILE *file = fopen("ACQ_Out", "w");

    printf("Simulation time is %ld (s)\n", endTime / (1000*1000*1000));
    unsigned long mem = mem_avail();
    printf("Free memory Available = %'ld\n", mem / (1024*1024));

    printf("Start Simulating ......\n");
    timing(&wc1, &cpuT);
    MAX_ARR = (25 * k * k * k/4);
    arr = malloc(sizeof *arr * MAX_ARR);
    data = malloc(MAX_ARR * sizeof(unsigned long));
    for (i = 0; i < MAX_ARR; i++)
    {
        arr[i] = malloc(sizeof *arr[i] * 4);
        for (j = 0; j < 4; j++)
        {
        arr[i][j] = UINT_MAX;
        }
        data[i] = 0;
    }

    buildData(data, k);
    initqueueH(numOfHosts, k*k*5/4);   
}