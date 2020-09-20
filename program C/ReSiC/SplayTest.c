/*
                An implementation of testing Splay 

  
*/
#include <stdio.h>
#include <stdlib.h>
#include "Splay.c"


void main() {
/* A sample use of these functions. Start with the empty tree,         */
/* insert some stuff into it, and then delete it                        */
    Tree * root;
    int i;
    root = NULL;              /* the empty tree */
    root = insert(A, 0, 0, 0, 10, root);
    root = insert(A, 1, 1, 0, 20, root);
    //show(root);
    root = insert(A, 2, 2, 0, 30, root);
    //show(root);
    root = insert(A, 3, 3, 0, 40, root);
    root = insert(A, 4, 4, 0, 50, root);
    root = insert(A, 5, 5, 0, 60, root);
    root = insert(A, 6, 6, 0, 70, root);
    root = insert(A, 4, 4, 0, 50, root);
    root = insert(A, 6, 6, 0, 70, root);
    root = insert(A, 4, 4, 0, 50, root);
    show(root);
    //root = insert(A, 2, 2, 0, 30, root);
    
//    size = 0;
//    for (i = 0; i < 1024*1024; i++) {
//	    root = insert((541*i) & (1024*1024 - 1), root);
//    }
//    for (i = 0; i < 1024*1024; i++) {
//	//root = delete((541*i) & (1024*1024 - 1), root);
//    }
//    printf("size = %d\n", size);
}
