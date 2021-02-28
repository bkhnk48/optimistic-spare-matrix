#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int contains(int *array, const int length, const int num);

void addToArray(int *array, int *lastIndex, int length, int elem);

enum PairStrategy{
    FORCE_TO_PAIR = 0,
    RANDOM = 1,
    STRIDE_INDEX = 2,
    INTERPOD_INCOMMING = 3,
    SAME_ID_OUTGOING = 4
};

typedef struct _pairPattern{
    int src;
    int dst;
} PairPattern;

void printfPairs(PairPattern *pairs, const int numOfFlows){
    int i = 0;
    //int j = 0;
    printf("Source vs Destination: ");
    for(i = 0; i < numOfFlows; i++){
        printf("%d -> %d, ", i, pairs[i].dst);
        if(i > 0 && (i + 1) % 4 == 0){
            printf("\n");
            printf("Source vs Destination: ");
        }
    }
}

void forceToPair(PairPattern *pairs, const int numOfFlows, const int option){
    int *sources = NULL; 
    printf("Works for k = 4, 1-to-1 pattern\n");
    if(numOfFlows != 16){
        exit(1);
    }
    int i, j;
     
    for(i = 0; i < numOfFlows; i++){
        pairs[i].src = i;
        pairs[i].dst = -1;
    }
    /*0 1 2 3 | 4 5  6  7 |  8  9 10 11 | 12 13 14 15
     *0 1 2 3 | 8 9 10 11 | 16 17 18 19 | 24 25 26 27
    */
    #pragma region Destination hosts
    int *destinations = NULL;
    destinations = malloc(16 * sizeof(int));
    destinations[0] = 7;//11
    destinations[1] = 8;//16
    destinations[2] = 13;//25
    destinations[3] = 10;//18
    destinations[4] = 11;//19
    destinations[5] = 12;//24
    destinations[6] = 14;//26
    destinations[7] = 15;//27
    destinations[8] = 0;//0
    destinations[9] = 1;//1
    destinations[10] = 2;//2
    destinations[11] = 3;//3
    destinations[12] = 4;//8
    destinations[13] = 5;//9
    destinations[14] = 6;//10
    destinations[15] = 9;//17
    #pragma endregion

    if(option == 0){
        //sources = malloc(16 * sizeof(int));
        for(i = 0; i < numOfFlows; i++){
            //int src = sources[i];
            pairs[i].src = i;
            pairs[i].dst = (i + 1) % numOfFlows;
        }
        return;
    }

    if(option == 1){
        sources = malloc(16 * sizeof(int));
        sources[0] = 2;//2
        sources[1] = 3;//3
        sources[2] = 0;//0
        sources[3] = 1;//1
        sources[4] = 6;//10
        sources[5] = 7;//11
        sources[6] = 4;//8
        sources[7] = 11;//19
        sources[8] = 5;//9
        sources[9] = 10;//18
        sources[10] = 15;//27
        sources[11] = 8;//16
        sources[12] = 9;//17
        sources[13] = 14;//26
        sources[14] = 12;//24
        sources[15] = 13;//25
    }

    if(option == 2){
        sources = malloc(16 * sizeof(int));
        sources[0] = 3;//3
        sources[1] = 0;//0
        sources[2] = 1;//1
        sources[3] = 2;//2
        sources[4] = 7;//11
        sources[5] = 4;//8
        sources[6] = 5;//9
        sources[7] = 8;//16
        sources[8] = 6;//10
        sources[9] = 11;//19
        sources[10] = 12;//24
        sources[11] = 9;//17
        sources[12] = 10;//18
        sources[13] = 15;//27
        sources[14] = 13;//25
        sources[15] = 14;//26
    }

    if(option == 3){
        sources = malloc(16 * sizeof(int));
        sources[0] = 0;//0
        sources[1] = 1;//1
        sources[2] = 2;//2
        sources[3] = 3;//3
        sources[4] = 4;//8
        sources[5] = 5;//9
        sources[6] = 6;//10
        sources[7] = 9;//17
        sources[8] = 7;//11
        sources[9] = 8;//16
        sources[10] = 13;//25
        sources[11] = 10;//18
        sources[12] = 11;//19
        sources[13] = 12;//24
        sources[14] = 14;//26
        sources[15] = 15;//27
    }

    if(option == 996){
        //Throughput reaches 100%
        sources = malloc(16 * sizeof(int));
        sources[0] = 4;//8->11//7
        sources[1] = 10;//18->16//8
        sources[2] = 15;//27->25//13
        sources[3] = 8;//16->18//10
        sources[4] = 9;//17->19//11
        sources[5] = 14;//26->24//12
        sources[6] = 13;//25->26//14
        sources[7] = 12;//24->27//15
        sources[8] = 3;//3->0//0
        sources[9] = 2;//2->1//1
        sources[10] = 1;//1->2//2
        sources[11] = 0;//0->3//3
        sources[12] = 6;//10->8//4
        sources[13] = 7;//11->9//5
        sources[14] = 5;//9->10//6
        sources[15] = 11;//19->17//9
    }

    if(option == 997){
        //Pass through only two cores: 16, 17
        sources = malloc(16 * sizeof(int));
        sources[0] = 11;//19->11
        sources[1] = 12;//24->16
        sources[2] = 2;//2->25
        sources[3] = 13;//25->18
        sources[4] = 15;//27->19
        sources[5] = 1;//1->24
        sources[6] = 0;//0->26
        sources[7] = 3;//3->27
        sources[8] = 4;//8->0
        sources[9] = 6;//10->1
        sources[10] = 5;//9->2
        sources[11] = 7;//11->3
        sources[12] = 8;//16->8
        sources[13] = 10;//18->9
        sources[14] = 9;//17->10
        sources[15] = 14;//26->17
    }
    
    if(option == 998){
        sources = malloc(16 * sizeof(int));
        sources[0] = 9;//17->11
        sources[1] = 2;//2->16
        sources[2] = 10;//18->25
        sources[3] = 3;//3->18
        sources[4] = 14;//26->19
        sources[5] = 6;//10->24
        sources[6] = 7;//11->26
        sources[7] = 11;//19->27
        sources[8] = 4;//8->0
        sources[9] = 12;//24->1
        sources[10] = 5;//9->2
        sources[11] = 13;//25->3
        sources[12] = 0;//0->8
        sources[13] = 8;//16->9
        sources[14] = 1;//1->10
        sources[15] = 15;//27->17
    }
    
    if(option == 999){
        sources = malloc(16 * sizeof(int));
        sources[0] = 0;//0->11
        sources[1] = 4;//8->16
        sources[2] = 9;//17->25
        sources[3] = 7;//11->18
        sources[4] = 1;//1->19
        sources[5] = 11;//19->24
        sources[6] = 3;//3->26
        sources[7] = 5;//9->27
        sources[8] = 12;//24->0
        sources[9] = 10;//18->1
        sources[10] = 14;//25->2
        sources[11] = 6;//10->3
        sources[12] = 8;//16->8
        sources[13] = 15;//27->9
        sources[14] = 2;//2->10
        sources[15] = 13;//25->17
    }
    
    if(option == 1000){
        sources = malloc(16 * sizeof(int));
        sources[0] = 0;//0->11
        sources[1] = 14;//26->16
        sources[2] = 5;//9->25
        sources[3] = 7;//11->18
        sources[4] = 1;//1->19
        sources[5] = 3;//3->24
        sources[6] = 11;//19->26
        sources[7] = 9;//17->27
        sources[8] = 10;//18->0
        sources[9] = 12;//24->1
        sources[10] = 6;//10->2
        sources[11] = 13;//25->3
        sources[12] = 2;//2->8
        sources[13] = 8;//16->9
        sources[14] = 15;//27->10
        sources[15] = 4;//8->17
    }
    
    for(i = 0; i < numOfFlows; i++){
        for(j = i + 1; j < numOfFlows; j++){
            if(sources[i] == sources[j])
                exit(1);
        }
    }

    for(i = 0; i < numOfFlows; i++){
        int src = sources[i];
        pairs[src].dst = destinations[i];
    }

    free(sources);
    free(destinations);
}

void importPairs(PairPattern *pairs, const int k, const char *fileName){

}

void checkValid(int *sources, int * dests, const int k){
    //int numOfCores = k*k/4;
    int numOfHosts = k*k*k/4;
    int i, j, realCore = 0, sizeOfPod = k*k/4;
    for(i = 0; i < numOfHosts; i++){
        if(sources[i] == -1 || dests[i] == -1){
            printf("Not enough pair!\n");
            exit(1);
        }
    }
}

void interpodIncomming(PairPattern *pairs, const int k){
    srand(time(NULL));   // Initialization, should only be called once.
    int *sources = NULL;
    int *destinations = NULL;

    int numOfFlows = k*k*k/4;
    int *allHosts = NULL;
    sources = malloc(numOfFlows * sizeof(int));
    int lastOfSources = -1;
    int lastOfDests = -1;
    destinations = malloc(numOfFlows * sizeof(int));
    allHosts = malloc(numOfFlows * sizeof(int));
    int i, j;
    for(i = 0; i < numOfFlows; i++){
        sources[i] = -1;
        destinations[i] = -1;
        allHosts[i] = i;
    }

    int delta = rand(); // % (k*k/4);
    //printf("Delta = %d\n", delta);

    int numOfHosts = numOfFlows;
    int sizeOfPod = k*k/4;
    int currPod = 0, prePod = 0;
    for(int i = 0; i < numOfHosts; i++) {
        int dst = allHosts[i];
        prePod = currPod;
        if(contains(destinations, numOfHosts, dst) == 0){
            int index = (i + sizeOfPod + delta) % numOfHosts;
            if(index / sizeOfPod == prePod)
            {
                index = (index + sizeOfPod) % numOfHosts;
                //if(allHosts[index] /elmOfPod == dst / elmOfPod)
                if(index / sizeOfPod == dst / sizeOfPod)
                {
                    index = (index + sizeOfPod) % numOfHosts;
                }
            }
            int count = 0;
            int expectedSrc = allHosts[index];
            int found = 0;
            //int j =
            while(!found && count < k)
            {
                if(contains(sources, numOfHosts, expectedSrc))
                {
                    for(int j = index + 1; j < (index/sizeOfPod + 1)*sizeOfPod; j++)
                    {
                        expectedSrc = allHosts[j];
                        if(contains(sources, numOfHosts, expectedSrc) == 0
                                    && ((expectedSrc / sizeOfPod) != (dst / sizeOfPod))
                            )
                        {
                            found = 1;
                            addToArray(sources, &lastOfSources, numOfHosts, expectedSrc);
                            addToArray(destinations, &lastOfDests, numOfHosts, dst);
                            if((i + 1) % sizeOfPod == 0)
                            {
                                currPod = (i + 1) / sizeOfPod;
                            }
                            else {
                                currPod = j / sizeOfPod;
                            }

                            addToArray(sources, &lastOfSources, numOfHosts, dst);
                            //sources.add(dst);
                            addToArray(destinations, &lastOfDests, numOfHosts, expectedSrc);
                            //destinations.add(expectedSrc);
                            break;
                        }
                    }
                }//endof if(contains(sources, numOfHosts, expectedSrc))
                else{
                    if(expectedSrc / sizeOfPod != dst / sizeOfPod)
                    {
                        found = 1;
                        addToArray(sources, &lastOfSources, numOfHosts, expectedSrc);
                        addToArray(destinations, &lastOfDests, numOfHosts, dst);
                            
                        addToArray(sources, &lastOfSources, numOfHosts, dst);
                        addToArray(destinations, &lastOfDests, numOfHosts, expectedSrc);
                            
                        if((i + 1) % sizeOfPod == 0){
                            currPod = (i + 1) / sizeOfPod;
                        }
                        else {
                            currPod = index / sizeOfPod;
                        }
                        break;
                    }
                }

                if(!found)
                {
                    count++;
                    index = (index + sizeOfPod) % numOfHosts;
                    expectedSrc = allHosts[index];
                }
            }
            //end of while(!found && count < k)

        }
        //end of if(!destinations.contains(dst))
        else {
            //currPod = (i + 1) / elmOfPod;
            currPod = i / sizeOfPod;
        }
    }

    checkValid(sources, destinations, k);

    int src;
    for(i = 0; i < numOfHosts; i++){
        src = sources[i];
        pairs[src].dst = destinations[i];
    }

    free(sources);
    free(destinations);
    free(allHosts);
}

int contains(int *array, const int length, const int num){
    int i = 0;
    for(i = 0; i < length; i++){
        if(array[i] == -1)
            return 0;
        else if(array[i] == num){
            return 1;
        }
    }
    return 0;
}

void addToArray(int *array, int *lastIndex, int length, int elem){
    if(length == *lastIndex){
        return;
    }
    if(*lastIndex == -1){
        array[0] = elem;
        *lastIndex = 0;
        return;
    }
    if((*lastIndex < (length - 1)) && (*lastIndex >= 0)){
        array[*lastIndex + 1] = elem;
        *lastIndex = *lastIndex + 1;
        return;
    }
}