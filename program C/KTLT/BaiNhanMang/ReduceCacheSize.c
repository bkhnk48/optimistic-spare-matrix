#include <stdio.h>
#include <stdlib.h>
#include "timing.c"
#include <assert.h>
#define N 1000
//Nhan ma tran kich thuoc lon

int min(int x, int y){
    return (x > y ? y : x);
}

int main(){
    double** A = NULL;
    double** B = NULL;
    double** C = NULL;
    double** D = NULL;
    A = (double **)malloc(sizeof * A * N);
    B = (double **)malloc(sizeof * B * N);
    C = (double **)malloc(sizeof * C * N);
    D = (double **)malloc(sizeof * D * N);
    int i, j, ii, jj;
    int step = 20;

    #pragma region Cap phat bo nho va gan gia tri
    
    for(i = 0; i < N; i++)
    {
        A[i] = (double *)malloc(sizeof * A[i] * N);
        B[i] = (double *)malloc(sizeof * B[i] * N);
        C[i] = (double *)malloc(sizeof * C[i] * N);
        D[i] = (double *)malloc(sizeof * D[i] * N);
        
        for(j = 0; j < N; j++)
        {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
            D[i][j] = 0;
        }
    }
    #pragma endregion

    double wc1, wc2, cpuT;
    double r = 0;
    int k = 0, kk;
    timing(&wc1, &cpuT);

    #pragma region Reduce cache size
    for(ii = 0; ii < min(ii + step, N); ii += step)
    {
        for(jj = 0; jj < min(jj + step, N); jj += step)
        {
            for(kk = 0; kk < min(kk + step, N); kk += step){

                for(i = ii; i < min(ii + step, N); i++){
                    for(j = jj; j < min(jj + step, N); j++){
                        r = 0;
    /*Vong lap line 52*/for(k = kk; k < min(kk + step, N); k++)
                        {
                            r += A[i][k]*B[k][j];
                        }
                        C[i][j] += r;
                    }
                }
            }
        }
    }
    #pragma endregion

    timing(&wc2, &cpuT);
    printf("Reduced Cache Size: Execution time is %f (s)\n", wc2 - wc1);
    printf("============================================\n");
    
    #pragma region Tinh toan D theo cach binh thuong
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            r = 0;
            for(k = 0; k < N; k++)
            {
                r += A[i][k]*B[k][j];
            }
            D[i][j] = r;
        }
    }
    #pragma endregion
   

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            assert(C[i][j] == D[i][j]);//Check assert
        }
    }
    printf("The results of multiplication matrices in this way are correct!!!\n");   

    return 0;
}
