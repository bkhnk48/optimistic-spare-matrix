#include "timing.h"

void timing_(double* wcTime, double* cpuTime)
{
   timing(wcTime, cpuTime);
}

void timing(double* wcTime, double* cpuTime)
{
   struct timeval tp;
   struct rusage ruse;

   gettimeofday(&tp, NULL);
   *wcTime=(double) (tp.tv_sec + tp.tv_usec/1000000.0); 
  
   getrusage(RUSAGE_SELF, &ruse);
   *cpuTime=(double)(ruse.ru_utime.tv_sec+ruse.ru_utime.tv_usec / 1000000.0);
}

/*
void initArray(int k)
{
    double wc1 = 0, wc2 = 0, cpuT = 0;
 
    timing(&wc1, &cpuT);

    int arr1[320][7];
    for(i = 0; i < 320; i++)
    {
        for(j = 0; j < 7; j++)
        {
            arr1[i][j] = 0;
        }
    }

    timing(&wc2, &cpuT);
    printf("Time: %f ms\n", (wc2 - wc1)*1000);
    printf("================================\n");
    double loadedBytes = (double)7*4*sizeArr/(1000*1000);
    printf("Performance %lf MB/s\n", loadedBytes/(wc2 - wc1));
    return;
}
*/
