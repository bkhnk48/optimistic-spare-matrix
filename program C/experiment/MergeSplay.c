#include <stdio.h>
#include <stdlib.h>
#include "Event.c"
#include <limits.h>

unsigned long data[384];
unsigned int arr[384][5];
const unsigned long RIGHT_MASK = 0x7fffffff;
const unsigned long LEFT_MASK = ((unsigned long)0x7fffffff << 32);
const int MASK_INT = 65535;
//int sizeOfTree = 0;
//int maxSize = 0;


//unsigned long arr[20250][7];
void add(int type, int idElementInGroup,
                int portID, 
                unsigned long endTime,
                unsigned int *root,
                unsigned int idNewNode
                //int k,
                //unsigned long arr[20250][7]
                );
/* From now on, an event (is stored in arr) has 5 fields:
   + endTime: the time of event to be finished.
   + index of father.
   + index of left
   + index of right

   Location, type of event are stored in array named data:
   + it's type
   + idElementInGroup is the index of element in each group Hosts, Ways, Switches Edge, Agg, Core
   + the portID where the event happens (= -1 in case event - A, B, C, G, H occurs on host)
   
*/

void splay(unsigned int e//, //unsigned long arr[20250][7]
                  //unsigned long arr[384][7]
                  );

void removeFirst(unsigned int * first, 
                  unsigned int * root //unsigned long arr[20250][7]
                     //unsigned long arr[384][7]
                  );

void show(//unsigned long arr[20250][7], 
            //unsigned long arr[384][7],
            unsigned int root);
void leaf(//unsigned long arr[20250][7], 
            //unsigned long arr[384][7],
               int root, enum Side side);

static inline int compare(unsigned long endTime, 
                        unsigned int upper, 
                        unsigned int lower){
   unsigned int part2 = (unsigned int)(endTime >> 32);
   if(part2 > upper)
      return 1;
   if(part2 < upper)
      return -1;
   if(part2 == upper){
      unsigned int part1 = (unsigned int)endTime;
      if(part1 > lower)
         return 1;
      if(part1 < lower)
         return -1;
   }
   return 0;
}   

void add(int type, int idElementInGroup,
                int portID, 
                unsigned long endTime,
                unsigned int *root,
                unsigned int idNewNode
                //unsigned long arr[20250][7]
                //unsigned long arr[384][7]
                )
{
   /* Quy ước arr[i][3] se luu cac phan tu
    *  arr[i][0] luu tru 4 bytes tren cua endTime
    *  arr[i][1] luu tru 4 bytes cuoi cua endTime 
    *  arr[i][2] luu tru id cua father
    *  arr[i][3] luu tru id cua left
    *  arr[i][4] luu tru id cua right
   */
   data[idNewNode] = ((unsigned long)idElementInGroup << 32)
                         | ((portID) & MASK_INT) << 16 
                         | (type & MASK_INT);

   arr[idNewNode][0] = (unsigned int)(endTime >> 32);
   arr[idNewNode][1] = (unsigned int)(endTime);
   
   int formerFather = arr[idNewNode][2];
   if(formerFather != UINT_MAX)
   { 
      if(arr[formerFather][3] == idNewNode)
      {
         arr[formerFather][3] = UINT_MAX;
      }
      else if(arr[formerFather][4] == idNewNode)
      {
         arr[formerFather][4] = UINT_MAX;
      }
   }
   arr[idNewNode][2] = UINT_MAX;
   arr[idNewNode][3] = UINT_MAX;
   arr[idNewNode][4] = UINT_MAX;

   if(*root == UINT_MAX)
   {
      *root = idNewNode;
      return;
   }
   unsigned int left = idNewNode, right = idNewNode, temp = -1;
   int end_splay = 0;
   unsigned int t = *root;
   while (end_splay == 0) {
      int comp = compare(endTime, arr[t][0], arr[t][1]);
      if(comp > 0)//if(endTime > t->endTime)
      {
         temp = arr[t][4];//temp = t->right;
         if(temp == UINT_MAX)//if(temp == NULL)
         {
            //cas "zig"
            arr[left][4] = t; //left->right = t;
            arr[t][2] = left; //t->father = left;
            arr[right][3] = UINT_MAX;//right->left = NULL;
            end_splay = 1;
         }
         else if(compare(endTime, arr[temp][0], arr[temp][1]) < 0)
         //if(endTime < temp->endTime)
         {
            //cas "zig-zag" simplifie
            arr[left][4] = t;//left->right = t;
            arr[t][2] = left;//t->father = left;
            left = t;//left = t;
            t = temp;//t = temp;
         }
         else {
            //cas "zig-zig"
            arr[t][4] = arr[temp][3]; //t->right = temp->left;
            if(arr[temp][3] != UINT_MAX)//if (temp->left != NULL)
               arr[  arr[temp][3]  ][  2  ] = t; 
               //temp->left->father = t;
            arr[left][4] = temp;//left->right = temp;
            arr[temp][2] = left;//temp->father = left;
            arr[temp][3] = t;//temp->left = t;
            arr[t][2] = temp;//t->father = temp;
            left = temp;
            t = arr[temp][4];//t = temp->right;
            if(t == UINT_MAX){//if (t == NULL) {
               arr[right][3] = UINT_MAX;//right->left = NULL;
               end_splay = 1;
            }
         }
      }else {
         temp = arr[t][3]; //temp = t->left;
         if(temp == UINT_MAX){//if (temp == NULL) {
            //cas "zig"
            arr[right][3] = t; //right->left = t;
            arr[t][2] = right; //t->father = right;
            arr[left][4] = UINT_MAX;     //left->right = NULL;
            end_splay = 1;
         }
         else if (compare(endTime, arr[temp][0], arr[temp][1]) > 0) {
            //if (endTime > temp->endTime) {
            //cas "zig-zag" simplifie
            arr[right][3] = t; //right->left = t;
            arr[t][2] = right; //t->father = right;
            right = t;//right = t;
            t = temp;//t = temp;
         }
         else {
            //cas "zig-zig"
            arr[t][3] = arr[temp][4]; //t->left = temp->right;
            if (arr[temp][4] != UINT_MAX)//if (temp->right != NULL)
               arr[  arr[temp][4]  ][2] = t; 
               //temp->right->father = t;
            arr[right][3] = temp; //right->left = temp;
            arr[temp][2] = right; //temp->father = right;
            arr[temp][4] = t; //temp->right = t;
            arr[t][2] = temp; //t->father = temp;
            right = temp;         //right = temp;
            t = arr[temp][3]; //t = temp->left;
            if(t == UINT_MAX) {//if (t == NULL) {
               arr[left][4] = UINT_MAX; //left->right = NULL;
               end_splay = 1;
            }
         }
      }
   }

   temp = arr[idNewNode][3]; //temp = newNode->left;
   arr[idNewNode][3] = arr[idNewNode][4]; 
   //newNode->left = newNode->right;
   arr[idNewNode][4] = temp; //newNode->right = temp;

   //return newNode;
   *root = idNewNode;

}

void splay(unsigned int e //unsigned long arr[20250][7]
                  //unsigned long arr[384][7]
               )
{
   int left;
   int f;   // Tree * f;
   int gf;  // Tree * gf;
   int ggf; // Tree * ggf;
   while(arr[e][2] != UINT_MAX)//while(e->father != NULL)
   {
      f = arr[e][2];  //  f = e->father;
      gf = arr[f][2]; //  gf = f->father;
      left = (e == arr[f][3] ? 1 : 0);  
      //left = (e == f->left ? 1 : 0);
      if(left)
      {
         // cas du fils gauche
         if (gf == UINT_MAX) {//if (gf == NULL) {
            // cas "zig", on fait la rotation de f (la racine) et e
            arr[f][2] = e; //f->father = e;
            arr[f][3] = arr[e][4]; //f->left = e->right;
            if(arr[f][3] != UINT_MAX) //if(f->left != NULL)
               arr[arr[f][3]][2] = f; //f->left->father = f;
            arr[e][4] = f; //e->right = f;
            arr[e][2] = UINT_MAX; //e->father = NULL;
         }
         else if(arr[gf][4] == f) { //if (gf->right == f) {
            // cas "zig-zag", simplifie, pareil que le cas "zig"
            arr[gf][4] = e; //gf->right = e;

            arr[f][2] = e;  //f->father = e;
            arr[f][3] = arr[e][4]; //f->left = e->right;
            if(arr[f][3] != UINT_MAX) //if(f->left != NULL)
               arr[arr[f][3]][2] = f; //f->left->father = f;
            arr[e][4] = f; //e->right = f;
            arr[e][2] = gf; //e->father = gf;
         }
         else {
            // cas "zig-zig", on fait la rotation de gf avec
            // f, suivis de la rotation de e avec f
            ggf = arr[gf][2]; //ggf = gf->father;

            arr[gf][3] = arr[f][4]; //gf->left = f->right;
            if(arr[gf][3] != UINT_MAX)//if(gf->left != NULL)
               arr[arr[gf][3]][2] = gf; //gf->left->father = gf;
            arr[f][4] = gf; //f->right = gf;
            arr[gf][2] = f; //gf->father = f;

            arr[f][3] = arr[e][4]; //f->left = e->right;
            if(arr[f][3] != UINT_MAX) //if(f->left != NULL)
               arr[ arr[f][3] ][2] = f; //f->left->father = f;
            arr[f][2] = e; //f->father = e;
            arr[e][4] = f; //e->right = f;

            // on rattache e a son nouveau pere
            arr[e][2] = ggf; //e->father = ggf;
            if(ggf != UINT_MAX) //if(ggf != NULL)
               if(arr[ggf][3] == gf) //if(ggf->left == gf)
                  arr[ggf][3] = e; //ggf->left = e;
               else
                  arr[ggf][4] = e; //ggf->right = e;
         }
      }
      else
      {
         //cas du fils droit
         if(gf == UINT_MAX){ //if(gf == NULL) {
            // cas "zig", on fait la rotation de f (la racine) et e

            arr[f][2] = e; //f->father = e;
            arr[f][4] = arr[e][3]; //f->right = e->left;
            if(arr[f][4] != UINT_MAX) //if(f->right != NULL)
               arr[arr[f][4]][2] = f; //f->right->father = f;
            arr[e][3] = f; //e->left = f;
            arr[e][2] = UINT_MAX; //e->father = NULL;
         }
         else if( arr[gf][3] == f) {//if(gf->left == f) {
            // cas "zig-zag", simplifie, pareil que le cas "zig"
            arr[gf][3] = e; //gf->left = e;

            arr[f][2] = e; //f->father = e;
            arr[f][4] = arr[e][3]; //f->right = e->left;
            if(arr[f][4] != UINT_MAX)//if(f->right != NULL)
               arr[arr[f][4]][2] = f;//f->right->father = f;
            arr[e][3] = f; //e->left = f;
            arr[e][2] = gf; //e->father = gf;
         }
         else {
            // cas "zig-zig", on fait la rotation de gf avec
            // f, suivis de la rotation de e avec f
            ggf = arr[gf][2]; //ggf = gf->father;

            arr[gf][4] = arr[f][3]; //gf->right = f->left;
            if( arr[gf][4] != UINT_MAX ) //if(gf->right != NULL)
               arr[arr[gf][4]][2] = gf; //gf->right->father = gf;
            arr[f][3] = gf; //f->left = gf;
            arr[gf][2] = f; //gf->father = f;

            arr[f][4] = arr[e][3]; //f->right = e->left;
            if( arr[f][4] != UINT_MAX )//if(f->right != NULL)
               arr[ arr[f][4] ][2] = f; //f->right->father = f;
            arr[f][2] = e; //f->father = e;
            arr[e][3] = f; //e->left = f;

            // on rattache e a son nouveau pere
            arr[e][2] = ggf; //e->father = ggf;
            if( ggf != UINT_MAX )//if(ggf != NULL)
               if(arr[ggf][3] == gf) //if(ggf->left == gf)
                  arr[ggf][3] = e; //ggf->left = e;
               else
                  arr[ggf][4] = e; //ggf->right = e;
         }
      }
   }
}

void removeFirst(unsigned int * first, unsigned int * root 
                     //unsigned long arr[20250][7]
                     //unsigned long arr[384][7]
                  )
{
   //sizeOfTree--;
   unsigned int t = *root;
   if(t == -1)
   {  
      *first = UINT_MAX; 
      return;//return NULL;
   }
   unsigned int temp = t;
   while(arr[temp][3] != UINT_MAX)//while(temp->left != NULL)
   {
      temp = arr[temp][3]; //temp = temp->left;
   }
   *first = temp;
   
   splay(*first//, arr
                  );
   while(arr[t][2] != UINT_MAX)
   {
      t = arr[t][2];
   }

   int leftTree, rightTree;
   leftTree = arr[t][3];//leftTree = t->left;
   if(leftTree != UINT_MAX)//if(leftTree != NULL)
   {
      arr[leftTree][2] = UINT_MAX;//leftTree->father = NULL;
   }
   rightTree = arr[t][4]; //rightTree = t->right;
   if(rightTree != UINT_MAX) //if(rightTree != NULL)
      arr[rightTree][2] = UINT_MAX; //rightTree->father = NULL;
   int removedFather = 0;
   if(temp == arr[t][2])//if(temp == t->father)
   {
        removedFather = 1;
   }  
   if(rightTree == UINT_MAX && 
            leftTree == UINT_MAX && 
               removedFather == 1
               //if(rightTree == NULL && leftTree == NULL 
                  //&& removedFather == 1
            )
   {  
      *first = t; return; //return t;
   }
   if(rightTree == UINT_MAX)//if(rightTree == NULL)
   {     
      t = leftTree; //t = leftTree;
   }
   else if(leftTree == UINT_MAX) //if(leftTree == NULL)
   {
      t = rightTree; //t = rightTree;
   }
   else 
   {
      unsigned int newRoot = rightTree; 
      //Tree * newRoot = rightTree;
      while(arr[newRoot][3] != UINT_MAX) 
      //while(newRoot->left != NULL)
      {
         newRoot = arr[rightTree][3]; 
         //newRoot = rightTree->left;
      }

      //splay(newRoot, arr);
      splay(newRoot);
      while(arr[newRoot][2] != UINT_MAX)
      {
         newRoot = arr[newRoot][2];
      }

      arr[newRoot][3] = leftTree; //newRoot->left = leftTree;
      arr[leftTree][2] = newRoot; //leftTree->father = newRoot;
      //return newRoot;
      *first = newRoot;
      return;
   }

   *root = t; //return t;
   return;
   
}

void show(//unsigned long arr[20250][7], 
            //unsigned long arr[384][7],
            unsigned int root)
{
   if(root != -1 && arr[root][3] != -1)
   {
      printf("\n===========> for event type = %d at end = %d in %d\n", 
               arr[root][0], arr[root][3], arr[root][1]);
      leaf(//arr, 
               arr[root][5], LEFT);
      leaf(//arr, 
               arr[root][6], RIGHT);
   }
   else{
      printf("===========> left NULL\n");
   }
}

void leaf(//unsigned long arr[20250][7], 
            //unsigned long arr[384][7],
            int root, enum Side side)
{
   printf("===========> ");
   if(side == LEFT)
   {
       printf("left ");
   }
   else{
       printf("right ");
   }
   if(root != -1 && arr[root][3] != -1)
   {
      printf("for event type = %d at end = %d in %d. It's index = %d\n", 
               arr[root][0], arr[root][3], arr[root][1], root);
      leaf(//arr, 
            arr[root][5], LEFT);
      leaf(//arr, 
            arr[root][6], RIGHT);
   }
   else{
      printf("NULL\n");
   }
}

void validate(//unsigned long arr[20250][7], 
               //unsigned long arr[384][7],
               int index)
{
   //return;
   //if(arr[33][4] == 45 && arr[45][5] == 33){
   //   printf("\tarr[33][4] = %d and arr[45][5, 6] = %d, %d\n", arr[33][4], arr[45][5], arr[45][6]);
   //   printf("The type is %d, endTime is %d\n", arr[33][0], arr[33][3]);
   //}
   if(arr[index][5] == -1 && arr[index][6] == -1)
   {
      return;
   }
   int father = arr[index][4];
   if(father != -1)
   {
      if(arr[father][5] != index && arr[father][6] != index)
      {
         printf("Wrong content as arr[%d][5] = %d and arr[%d][6] = %d while it's child(index = %d) has father: %d\n"
                  , father, arr[father][5], father, arr[father][6], index, arr[index][4]);
         exit(1);
      }
   }
   if(arr[index][5] != -1)
   {
      validate(//arr, 
                  arr[index][5]);
   }
   if(arr[index][6] != -1)
   {
      validate(//arr, 
                  arr[index][6]);
   }
}
