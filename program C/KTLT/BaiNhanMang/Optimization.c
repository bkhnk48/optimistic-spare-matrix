#include <stdio.h>
#include "timing.c"
#include <omp.h>

int main(){
    
    int** table = NULL;
    table = (int **)malloc(sizeof * table * 2000);
    for(int i = 0; i < 2000; i++){
        table[i] = (int *)malloc(sizeof * table[i] * 250000);
    }
    for(int i = 0; i < 2000; i++)
    {
        for(int j = 0; j < 250000; j++)
            table[i][j] = i+j;
    }
    long sum = 0;
    
    double wc1 = 0, wc2 = 0, cpuT = 0;
    
    timing(&wc1, &cpuT);

    #pragma omp parallel for reduction (+:sum) num_threads(4)
    for (int i = 0; i < 2000; i += 2){
        for (int j = 0; j < 500; j++)
            sum = sum + table[i][j*j];
    }
    
    timing(&wc2, &cpuT);
    printf("\n Time: %f ms\n", (wc2 - wc1)*1000);
    printf("sum = %ld\n", sum);
    
}