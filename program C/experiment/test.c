#include <stdio.h>
#include <stdlib.h>
#include "timing.c"

void dummy(int * x, int * max, int * min)
{
    *x = *max;
    *max = *min;
    *min = *x;
}
int main(int argc, char** argv)
{
    double wc1 = 0, wc2 = 0, cpuT = 0;
    double value1, value2;
    int i =0;
    char *input1 = argv[1];
    char *input2 = argv[2];
    char *input3 = argv[3];
    int x = atoi(input1);
    int max = atoi(input2);
    int min = atoi(input3);
    int sum = 1;
    timing(&wc1, &cpuT);
    for(i = 0; i < 1000000; i++)
    {
        if(x >= min && x < max)
            sum++;
        if(sum < 0){ dummy(&x, &max, &min); }
        /* To understand why the if(sum < 0) and dummy here, please 
        read "Introduction to High Performance Computing 
                for Scientist and Engineers", page 5
        */
    }
    timing(&wc2, &cpuT);
    value1 = (wc2 - wc1)*1000;
    printf("Time of if(x >= min && x < max): %f ms\n", value1);
    printf("================================\n");
    sum = 1;
    i = 0;
    x++;
    timing(&wc1, &cpuT);
    for(i = 0; i < 1000000; i++)
    {
        if((unsigned)(x - min) < (max - min))
            sum++;
        if(sum < 0){ dummy(&x, &max, &min); }
        /* To understand why the if(sum < 0) and dummy here, please 
        read "Introduction to High Performance Computing 
                for Scientist and Engineers", page 5
        */
    }
    timing(&wc2, &cpuT);
    value2 = (wc2 - wc1)*1000;
    printf("Time of if((unsigned)(x - min) < (max - min)): %f ms\n", value2);
    printf("================================\n");
    if(value2 < value1)
    {
        printf("\nThe if(x >= min && x < max) is slower "); 
        printf("than if((unsigned)(x - min) < (max - min))\n");
    }

}