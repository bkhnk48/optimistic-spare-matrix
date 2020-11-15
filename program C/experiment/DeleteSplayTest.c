#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h> // __fpurge(stdin);
#include "VoidSplay.c"
#include "timing.c"

int main()
{
    splay_tree *t = new_splay_tree();              /* the empty tree */
    double wc1 = 0, wc2 = 0, cpuT = 0;
    int i = 0;
    long count = 0;
    timing(&wc1, &cpuT);
    int currentTime = 0;
    int endTime = 1000*1000;
    
    for(i = 0; i < 10; i++)
    {
        insert(t, new_node(A, i, 0, 0));
    }

    node * max = new_node(A, 10, 0, 10);
    insert(t, max);

    for(i = 11; i < 16; i++)
    {
        insert(t, new_node(A, i, 0, i));
    }

    //show(t);

    delete(t, max);
    node * node8 = new_node(A, 10, 0, 8);
    //show(t);
    insert(t, node8);
    delete(t, node8);
    show(t);
    char c = 0;
    int num = 0;
    int index = 16;

    printf("* Plese select the number for appropritate tasks *\n");
    printf("* 1. Quit program. *\n");
    printf("* 2. Insert. *\n");
    printf("* 3. Delete. *\n");
    printf("* 4. Show. *\n");
    printf("* *\n");
    printf("***********************************************************\n");
    printf("\nInput a number to run its procedure: ");
    while(c != '1')
    {
        fflush(stdin);
        c = getchar();
        if(c != '\n' && c != EOF)
        {
            //printf("=> typed: %c\n", c);
            switch(c)
            {
                case '1': 
                    printf("Bye\n");
                    exit(1); 
                    break;
                case '2': 
                    printf("Type number to insert: "); scanf("%d", &num);
                    __fpurge(stdin);
                    insert(t, new_node(A, index, 0, num));
                    index++;
                    
                    break;

                case '3':
                    printf("Type number to DELETE: "); scanf("%d", &num);
                    __fpurge(stdin);
                    delete_key(t, num);
                    break;
                case '4':
                    show(t);
                    __fpurge(stdin);
                    break;
            }
            printf("OK, type again: ");
        }
    }

    
    timing(&wc2, &cpuT);
    printf("Time: %f ms with count = %ld\n", (wc2 - wc1)*1000, count);
    printf("================================\n");

    return 0;
}