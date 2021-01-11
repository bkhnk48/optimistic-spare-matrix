#include <stdio.h>
#include <stdlib.h>
#include "MergeSplay.c"
#include <limits.h>
#include <assert.h>
#include <omp.h>
#include "timing.c"

#define MAX 4000000000L



int main(){
    unsigned long i, j;
    unsigned int x, y;
    double wc1 = 0, wc2 = 0, cpuT = 0;
    
    timing(&wc1, &cpuT);

    #pragma omp parallel for num_threads(4) private(x, y, i, j)
    for(i = MAX; i < 200000 + MAX; i++){
        for(j = MAX; j < 200000 + MAX; j++){
            x = j >> 32;
            y = (unsigned int)j;
            int 
                z = compare(i, x, y);
            if(i > j){ 
                assert(z > 0);
            }
            else if(i < j){
                assert(z < 0);
            }
            else{
                assert(z == 0);
            }
            
        }
    }

    timing(&wc2, &cpuT);
    printf("\n Time: %f ms\n", (wc2 - wc1)*1000);
}