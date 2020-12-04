#include <stdio.h>
#include <stdlib.h>
#include "timing.c"
#define N 1000
//Nhan ma tran kich thuoc lon

int main(){
    double** A = NULL;
    double** B = NULL;
    double** C = NULL;
    A = (double **)malloc(sizeof * A * N);
    B = (double **)malloc(sizeof * B * N);
    C = (double **)malloc(sizeof * C * N);
    #pragma region Cap phat bo nho va gan gia tri
    int i, j;
    for(i = 0; i < N; i++)
    {
        A[i] = (double *)malloc(sizeof * A[i] * N);
        B[i] = (double *)malloc(sizeof * B[i] * N);
        C[i] = (double *)malloc(sizeof * C[i] * N);
        
        for(j = 0; j < N; j++)
        {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }
    #pragma endregion

    double wc1, wc2, cpuT;
    double r = 0;
    int k = 0;
    timing(&wc1, &cpuT);

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            r = 0;
            for(k = 0; k < N; k++)
            {
                r += A[i][k]*B[k][j];
            }
            C[i][j] = r;
        }
    }

    timing(&wc2, &cpuT);
    printf("Execution time is %f (s)\n", wc2 - wc1);
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            if(C[i][j] == 0)
            {
                printf("Wrong multiplication\n");
                exit(1);
            }
        }
    }
    return 0;
}