#include <stdio.h>
#include <stdlib.h>

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
    int i;
     
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

    
    for(i = 0; i < numOfFlows; i++){
        int src = sources[i];
        pairs[src].dst = destinations[i];
    }
}