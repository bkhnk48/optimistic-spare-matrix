#include "timing.c"

typedef struct NODE{
    int a, b, c, d;
    int * left;
    int * right;
    int * father;
} Node;
typedef struct VALUE{
    int a, b, c, d, e, f, g;
} Value;

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

void passIntegers(int x, int y, int z, 
                    int u, int v, int r, int s, 
                    Node *t)
{
    if(t == NULL)
    {
        t = (Node *)malloc(sizeof(Node));
    }
    t->a = x;
    t->b = y;
    t->c = z;
    t->d = u;
    t->left = NULL;
    t->right = NULL;
    t->father = NULL;
}

void passStruct(Value v, Node *t)
{
    if(t == NULL)
    {
        t = (Node *)malloc(sizeof(Node));
    }
    t->a = v.a + v.e;
    t->b = v.b + v.f;
    t->c = v.c + v.g;
    t->d = v.d;
    t->left = NULL;
    t->right = NULL;
    t->father = NULL;
}


void main(int argc, char** argv) {
    int att = atoi(argv[1]);

    int numOfInitialization = 320;
    int i = 0;
    double wc1 = 0, wc2 = 0, cpuT = 0;

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

    if(att == 4)
    {
        timing(&wc1, &cpuT);
        Node *t = NULL;
        t = (Node *)malloc(sizeof(Node));
        Value v;
        v.a = 5; v.b = 6; v.c = 7; v.d = 8;
        v.e = 1; v.f = 5; v.g = 2;
        while (i < numOfInitialization)
        {
            passStruct(v, t);
            i++;
            if(i < 0) { v.a ++;}
        }

        timing(&wc2, &cpuT);
        printf("Time passStruct: %f ms\n", (wc2 - wc1)*1000);
        printf("================================\n");
        double loadedBytes = (double)7*4*numOfInitialization/(1000*1000);
        printf("Performance %lf MB/s\n", loadedBytes/(wc2 - wc1));

        wc1 = 0; wc2 = 0; cpuT = 0;
        timing(&wc1, &cpuT);
        t = (Node *)malloc(sizeof(Node));
        i = 0;
        int x = 8, y = 9, z = 10, u = 8;
        while (i < numOfInitialization)
        {
            passIntegers(x, y, z, u, x + 2, y + 2, z + 4, t);
            i++;
            if(i < 0) { x ++;}
        }

        timing(&wc2, &cpuT);
        printf("Time passIntegers: %f ms\n", (wc2 - wc1)*1000);
        printf("================================\n");
        
        printf("Performance %lf MB/s\n", loadedBytes/(wc2 - wc1));
        return;
    }

    if(att == 5)
    {
        numOfInitialization *= numOfInitialization/10;
        //numOfInitialization *= numOfInitialization;
        printf("test performance of IF, SWITCH and SHIFT statements as #loops = %d\n", numOfInitialization);
        timing(&wc1, &cpuT);
        i = 0; int j = 15;
        //while(i < numOfInitialization)
        for(i = 0; i < numOfInitialization; i++)
        {
            if(j % 2 == 0)
            {
                j++;
            }else{
                j += 2;
            }
            j += 2;
            if(j < 0){ j--; }
        }
        timing(&wc2, &cpuT);
        printf("Time IF: %f ms\n", (wc2 - wc1)*1000);
        printf("================================\n");
        
        timing(&wc1, &cpuT);
        i = 0; j = 15;
        
        for(i = 0; i < numOfInitialization; i++)
        {
            switch(j % 2)
            {
                case 0:
                    j++;
                    break;
                case 1:
                    j += 2;
                    break;
            }
            j += 2;
            
            if(j < 0){ j--; }
        }
        timing(&wc2, &cpuT);
        printf("Time SWITCH: %f ms\n", (wc2 - wc1)*1000);
        printf("================================\n");
        
        timing(&wc1, &cpuT);
        i = 0; j = 15;
        //while(i < numOfInitialization)
        for(i = 0; i < numOfInitialization; i++)
        {
            j += 1 + (j & 1);
            j += 2;
            if(j < 0){ j--; }
        }
        timing(&wc2, &cpuT);
        printf("Time SHIFT: %f ms\n", (wc2 - wc1)*1000);
        printf("================================\n");
        
        printf("Conclusion: IF and SWITCH have the same performance\n");
        printf("If the shift increases the number of LOAD operations it will slow performance down T.T\n");
        return;
    }

    if(att == 6)
    {
        numOfInitialization *= numOfInitialization/10;
        //numOfInitialization *= numOfInitialization;
        printf("test performance of IF, SWITCH and SHIFT statements as #loops = %d\n", numOfInitialization);
        timing(&wc1, &cpuT);
        i = 0; int j = 15;
        //while(i < numOfInitialization)
        for(i = 0; i < numOfInitialization; i++)
        {
            int temp = j % 5;
            if(temp == 0)
            {
                j++;
            }else if(temp == 1)
            {
                j += 2;
            }else if(temp == 2)
            {
                j -= 1;
            }else if(temp == 3)
            {
                j -= 2;
            }else if(temp == 4)
            {
                j += 1;
            }
            

            j += 2;
            if(j < 0){ j--; }
        }
        timing(&wc2, &cpuT);
        printf("Time IF: %f ms\n", (wc2 - wc1)*1000);
        printf("================================\n");
        
        timing(&wc1, &cpuT);
        i = 0; j = 15;
        
        for(i = 0; i < numOfInitialization; i++)
        {
            switch(j % 5)
            {
                case 0:
                    j++;
                    break;
                case 1:
                    j += 2;
                    break;
                case 2:
                    j -= 1;
                    break;
                case 3:
                    j -= 2;
                    break;
                case 4:
                    j += 1;
                    break;
            }
            j += 2;
            
            if(j < 0){ j--; }
        }
        timing(&wc2, &cpuT);
        printf("Time SWITCH: %f ms\n", (wc2 - wc1)*1000);
        printf("================================\n");
        
        return;
    }

    if(att == 7)
    {

    }
}   