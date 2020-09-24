/*
            An implementation of testing Splay 
*/

#include <stdio.h>
#include <stdlib.h>
#include "Splay.c"


void main(int argc, char** argv) {
/* A sample use of these functions. Start with the empty tree,         */
/* insert some stuff into it, and then delete it                        */
    char *tID = argv[1];
    int testID = atoi(tID);
    /*
        1:  10, 20
        2:  10, 20, 30, 40, 50, 60, 70, 50, 70, 50, 60
    */
    
    Tree * root;

    
    root = NULL;              /* the empty tree */
    
    if(testID == 1)
    {
        root = add(A, 0, 0, 0, 0, 10, root);
        root = add(A, 1, 1, 0, 0, 20, root);
    }

    if(testID == 2)
    {
        root = add(A, 0, 0, 0, 0, 10, root);
        root = add(A, 1, 0, 0, 0, 20, root);
        root = add(A, 0, 0, 0, 0, 30, root);
        root = add(A, 1, 0, 0, 0, 40, root);
        root = add(A, 0, 0, 0, 0, 50, root);
        root = add(A, 1, 0, 0, 0, 60, root);
        root = add(A, 0, 0, 0, 0, 70, root);
        root = add(A, 1, 1, 0, 0, 50, root);
        root = add(A, 1, 1, 0, 1, 70, root);
        root = add(A, 1, 1, 0, 2, 50, root);
        root = add(A, 1, 1, 0, 1, 60, root);
        
    }

    if(testID == 3)
    {
        
    }

    if(testID == 4)
    {
        
    }
    
    show(root);
}
