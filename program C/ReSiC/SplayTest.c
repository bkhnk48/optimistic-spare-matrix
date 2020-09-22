/*
                An implementation of testing Splay 
int main() {
  splay_tree *t = new_splay_tree();

  node *a, *b, *c, *d, *e, *f, *g, *h, *i, *j, *k, *l, *m;
  a = new_node(10);
  b = new_node(20);
  c = new_node(30);
  d = new_node(100);
  e = new_node(90);
  f = new_node(40);
  g = new_node(50);
  h = new_node(60);
  i = new_node(70);
  j = new_node(80);
  k = new_node(150);
  l = new_node(110);
  m = new_node(120);

  insert(t, a);
  insert(t, b);
  insert(t, c);
  insert(t, d);
  insert(t, e);
  insert(t, f);
  insert(t, g);
  insert(t, h);
  insert(t, i);
  insert(t, j);
  insert(t, k);
  insert(t, l);
  insert(t, m);

  delete(t, a);
  delete(t, m);

  inorder(t, t->root);

  return 0;
}
  
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
        1:  10, 10
        2:  10, 20, 30, 40, 50, 60, 70, 50 
        3:  10, 20, 30, 40, 50, 60, 70, 50, 70, 50
    */
    //splay_tree *t = new_splay_tree();
    Tree * root;

    /*node *e1, *e1a, *e2, *e3, *e4, *e5, *e6, *e7, *e5a, *e7a, *e5b, *e6a; //, *l, *m;
    e1 = new_node(A, 0, 0, 0, 10);
    e1a = new_node(A, 0, 1, 0, 10);
    e2 = new_node(A, 0, 1, 0, 20);
    e3 = new_node(A, 0, 2, 0, 30);
    e4 = new_node(A, 0, 3, 0, 40);
    e5 = new_node(A, 0, 4, 0, 50);
    e6 = new_node(A, 0, 5, 0, 60);
    e7 = new_node(A, 0, 6, 0, 70);
    e5a = new_node(A, 0, 7, 0, 50);
    e7a = new_node(A, 0, 8, 0, 70);
    e5b = new_node(A, 0, 9, 0, 50);
    e6a = new_node(A, 0, 10, 0, 60);*/
    //l = new_node(110);
    //m = new_node(120);
    root = NULL;              /* the empty tree */
    
    if(testID == 1)
    {
        root = add(A, 0, 0, 0, 10, root);
        root = add(A, 1, 1, 0, 20, root);
    }

    if(testID == 2)
    {
        root = add(A, 0, 0, 0, 10, root);
        root = add(A, 1, 0, 0, 20, root);
        root = add(A, 0, 0, 0, 30, root);
        root = add(A, 1, 0, 0, 40, root);
        root = add(A, 0, 0, 0, 50, root);
        root = add(A, 1, 0, 0, 60, root);
        root = add(A, 0, 0, 0, 70, root);
        root = add(A, 1, 1, 0, 50, root);
        root = add(A, 1, 1, 1, 70, root);
        root = add(A, 1, 1, 2, 50, root);
        root = add(A, 1, 1, 1, 60, root);
        /*insert(t, e1);
        insert(t, e2);
        insert(t, e3);
        insert(t, e4);
        insert(t, e5);
        insert(t, e6);
        insert(t, e7);
        insert(t, e5a);*/
    }

    if(testID == 3)
    {
        
    }

    if(testID == 4)
    {
        
    }
    
    show(root);
}
