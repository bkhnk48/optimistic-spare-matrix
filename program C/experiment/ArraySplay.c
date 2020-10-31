#include <stdio.h>
#include <stdlib.h>
#include "Event.c"
#include <limits.h>

typedef struct array_splay {
    unsigned long arr[1024][7];
    struct array_splay *next;
}SplayTree;

typedef struct _result {
    int indexOfEmpty;
    SplayTree *tree;
}ResultOfFinding;

#pragma region prototype of functions
int indexOfNewNode(SplayTree *arraySplay);

void add(int type, int idElementInGroup,
                int portID, 
                unsigned long endTime,
                int *root,
                SplayTree *arraySplay
                );
/* From now on, an event has 7 fields about:
   + it's type
   + idElementInGroup is the index of element in each group Hosts, Ways, Switches Edge, Agg, Core
   + the portID where the event happens (= -1 in case event - A, B, C, G, H occurs on host)
   + endTime: the time of event to be finished.
   + 32 bit for index of elements in array, 32 bits later is index of father.
   + 32 bit for index of elements in array, 32 bits later is index of left
   + 32 bit for index of elements in array, 32 bits later is index of right
*/

void splay(int e, SplayTree *arraySplay);

void removeFirst(int * first, int * root, SplayTree *arraySplay);

void show(SplayTree *arraySplay, int root);
void leaf(SplayTree *arraySplay, int root, enum Side side);

SplayTree* new_splay_tree();
#pragma endregion

int indexOfNewNode(SplayTree *arraySplay){
    /*int index[33] = {   arraySplay->arr[0][4] >> 32, arraySplay->arr[0][5] >> 32, arraySplay->arr[0][6] >> 32,
                        arraySplay->arr[1][4] >> 32, arraySplay->arr[1][5] >> 32, arraySplay->arr[1][6] >> 32,
                        arraySplay->arr[2][4] >> 32, arraySplay->arr[2][5] >> 32, arraySplay->arr[2][6] >> 32,
                        arraySplay->arr[3][4] >> 32, arraySplay->arr[3][5] >> 32, arraySplay->arr[3][6] >> 32,
                        arraySplay->arr[4][4] >> 32, arraySplay->arr[4][5] >> 32, arraySplay->arr[4][6] >> 32,
                        arraySplay->arr[5][4] >> 32, arraySplay->arr[5][5] >> 32, arraySplay->arr[5][6] >> 32,
                        arraySplay->arr[6][4] >> 32, arraySplay->arr[6][5] >> 32, arraySplay->arr[6][6] >> 32,
                        arraySplay->arr[7][4] >> 32, arraySplay->arr[7][5] >> 32, arraySplay->arr[7][6] >> 32,
                        arraySplay->arr[8][4] >> 32, arraySplay->arr[8][5] >> 32, arraySplay->arr[5][6] >> 32,
                        arraySplay->arr[9][4] >> 32, arraySplay->arr[9][5] >> 32, arraySplay->arr[6][6] >> 32,
                        arraySplay->arr[10][4] >> 32, arraySplay->arr[10][5] >> 32, arraySplay->arr[10][6] >> 32,
                    };*/
    
    int i = 0;
    int result = 0;
    for(i = 0; i < 33; i++)
    {
        int u = i / 3;
        int v = i % 3;
        result = (arraySplay->arr[u][4 + v] >> 32);
        if(result != UINT_MAX)
            break;
    }
    if(i >= 33)
        return -1;
    return result;
}

SplayTree* new_splay_tree(){
  SplayTree *t = malloc(sizeof(SplayTree));
  int i = 0;
  for(i = 0; i < 1024; i++)
  {
      t->arr[i][0] = -1;
      t->arr[i][1] = -1;
      t->arr[i][2] = -1;
      t->arr[i][3] = -1;
      t->arr[i][4] = -1;
      t->arr[i][5] = -1;
      t->arr[i][6] = -1;
  }
  t->next = NULL;
  return t;
}

ResultOfFinding getSplayTree(SplayTree *root){
    SplayTree *temp = root;
    SplayTree *y = root;
    int index = -1;
    ResultOfFinding result;
    while(index == -1 && temp != NULL)
    {
        index = indexOfNewNode(temp);
        y = temp;
        temp = temp->next;
    }
    if(index == -1)
    {
        temp = new_splay_tree();
        y->next = temp;
        result.tree = temp;
        result.indexOfEmpty = 0;
        return result;
    }
    else{
        result.tree = y;
        result.indexOfEmpty = index;
    }
    return result;
}

void add(int type, int idElementInGroup,
                int portID, 
                unsigned long endTime,
                int *root,
                SplayTree *arraySplay
                ){
    ResultOfFinding finding = getSplayTree(arraySplay);
    SplayTree *temp = finding.tree;
    int index = finding.indexOfEmpty;
}