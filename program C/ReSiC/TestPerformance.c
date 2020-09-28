#include "timing.c"

typedef struct NODE{
    int a, b, c, d;
    int * left;
    int * right;
    int * father;
} Node;

void initArray(int k)
{
    double wc1 = 0, wc2 = 0, cpuT = 0;
    int sizeArr = 5*k*k*k;
    int i = 0, j = 0;
    printf("Check timing of initialization %d elements\n", 
                sizeArr);

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
    
    //free(arr1);
    return;
}

void mallocNodes(int numOfInitialization)
{
    printf("Check timing of MALLOC %d elements\n", 
                numOfInitialization);
    
    double wc1 = 0, wc2 = 0, cpuT = 0;
    
    int i = 0, j = 0;

    timing(&wc1, &cpuT);
    Node * node; 
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
    timing(&wc2, &cpuT);
    printf("Time: %f ms\n", (wc2 - wc1)*1000);
    printf("================================\n");
    double loadedBytes = (double)7*4*
                                numOfInitialization
                                /(1000*1000);
    printf("Performance %lf MB/s\n", loadedBytes/(wc2 - wc1));

}

void mallocOneNode(int *i, Node * node)
{
    node = (Node *)malloc(sizeof(Node));
    node->a = *i;
    node->b = (*i) + 1;
    node->c = (*i) + 2;
    node->d = (*i) + 3;
    node->left = i;
    node->right = NULL;
    node->father = NULL;
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
        int numOfInitialization = //2479412;
                                    320;
        mallocNodes(numOfInitialization);
        return;
    }
    
    if(att == 3)
    {
        int numOfInitialization = //2479412;
                                    320;
        int i[6] = {0};
        int j = 0;
        double wc1 = 0, wc2 = 0, cpuT = 0;
    

        timing(&wc1, &cpuT);
        Node *t;
        while (j < numOfInitialization)
        {
            mallocOneNode(i, t);
            j++;
        }

        timing(&wc2, &cpuT);
        printf("Time: %f ms\n", (wc2 - wc1)*1000);
        printf("================================\n");
        double loadedBytes = (double)7*4*numOfInitialization/(1000*1000);
        printf("Performance %lf MB/s\n", loadedBytes/(wc2 - wc1));
        return;
    }
}   