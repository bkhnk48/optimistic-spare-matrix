#include <stdio.h>
#include <stdlib.h>
#include "Event.c"
#include <limits.h>

typedef struct array_splay {
    int size;
    unsigned long arr[3*1024][7];
    struct array_splay *next;
    struct array_splay *prev;
}ArrayTree;

typedef struct splay_tree{
    int size;
    int root;
    int min;
    struct array_splay *tree;
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
                //int *root,
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

int indexOfNewNode(SplayTree *splayTree){
    if(splayTree->size == 0)
        return 0;
    if(splayTree->size % (3*1024) != 0)
    {
        int step = splayTree->size / (3*1024);
        int i = 0;
        
        int result = 0;
        for(i = 0; i < 32*3; i++)
        {
            int u = i / 3;
            int v = i % 3;
            result = (arraySplay->arr[u][4 + v] >> 32);
            if(result != UINT_MAX)
                break;
        }
        if(i >= 32*3)
            return -1;
        result = ~result;//dao het cac bit ben trong
        int ctz = __builtin_ctz(result) + i*32; 
        return ctz;
    }
    return -1;
}


SplayTree* new_splay_tree(){
  SplayTree *t = malloc(sizeof(SplayTree));
  t->tree = malloc(sizeof(ArrayTree));
  t->tree->prev = NULL;
  t->tree->next = NULL;
  int i = 0;
  for(i = 0; i < 1024; i++)
  {
      t->tree->arr[i][0] = -1;
      t->tree->arr[i][1] = -1;
      t->tree->arr[i][2] = -1;
      t->tree->arr[i][3] = -1;
      t->tree->arr[i][4] = UINT_MAX;
      t->tree->arr[i][5] = UINT_MAX;
      t->tree->arr[i][6] = UINT_MAX;
  }
  t->size = 0;
  t->root = -1;
  t->min = -1;
  
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
                SplayTree *splayTree
                ){
    ResultOfFinding finding = getSplayTree(splayTree);
    SplayTree *temp = finding.tree;
    int idNewNode = finding.indexOfEmpty;

    arraySplay->size++;
    temp->arr[idNewNode][0] = type;
    temp->arr[idNewNode][1] = idElementInGroup;
    temp->arr[idNewNode][2] = portID;
    temp->arr[idNewNode][3] = endTime;
    int i = idNewNode / 32;
    int alias = idNewNode % 32;
    temp->arr[i/3][4 + (i % 3)] |= (1L << alias) << 32;

    printf("idNewNode = %d and size = %d\n", idNewNode, arraySplay->size);
    int formerFather = temp->arr[idNewNode][4];
    if(formerFather != -1){

    }
}
