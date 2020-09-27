#include "timing.c"

typedef struct NODE{
    int a, b, c, d;
    int * left;
    int * right;
    int * father;
} Node;

void initArray(int k)
{
    double wc1 = 0, wc2 = 0, cpuT1 = 0, cpuT2 = 0;
    int sizeArr = 5*k*k*k;
    int i = 0, j = 0;
    printf("Check timing of initialization %d elements\n", 
                sizeArr);

    timing(&wc1, &cpuT1);

    int** arr1 = NULL;
    
    arr1 = malloc(sizeof * arr1 * sizeArr);
    for(i = 0; i < sizeArr; i++)
    {
        arr1[i] = malloc(sizeof * arr1[i] * 7);
        for(j = 0; j < 7; j++)
        {
            arr1[i][j] = 0;
        }
    }

    timing(&wc2, &cpuT2);
    printf("Time: %f ms\n", (wc2 - wc1)*1000);
    printf("================================\n");
    double loadedBytes = (double)7*4*sizeArr/(1000*1000);
    printf("Performance %lf MB/s\n", loadedBytes/(wc2 - wc1));
    
    free(arr1);
    return;
}

void mallocNodes(int numOfInitialization)
{
    Node * node; 
    double wc1 = 0, wc2 = 0, cpuT1 = 0, cpuT2 = 0;
    
    int i = 0, j = 0;

    timing(&wc1, &cpuT1);
    while(i < numOfInitialization)
    {
        node = (Node *)malloc(sizeof(Node));
        node->a = i;
        node->b = i + 1;
        node->c = i + 2;
        node->d = i + 3;
        node->left = NULL;
        node->right = NULL;
        node->father = NULL;
        i++;
        free(node);
    }
    timing(&wc2, &cpuT2);
    printf("Time: %f ms\n", (wc2 - wc1)*1000);
    printf("================================\n");
    double loadedBytes = (double)7*4*numOfInitialization/(1000*1000*1000);
    printf("Performance %lf GB/s\n", loadedBytes/(wc2 - wc1));

}

void main(int argc, char** argv) {
    int att = atoi(argv[1]);

    if(att == 1)
    {
        int k = 4;
        initArray(k);
        return;
    }

    if(att == 2)
    {
        int numOfInitialization = 2479412;
        mallocNodes(numOfInitialization);
        return;
    }
    
}   