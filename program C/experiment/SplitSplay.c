#include <stdio.h>
#include <stdlib.h>
#include "Event.c"


int data[20250][3];
unsigned long arr[20250][3];
void add(int type, int idElementInGroup,
                int portID, 
                unsigned long endTime,
                int *root
                );
/* From now on, an event has 7 fields about:
   + it's type
   + idElementInGroup is the index of element in each group Hosts, Ways, Switches Edge, Agg, Core
   + the portID where the event happens (= -1 in case event - A, B, C, G, H occurs on host)
   + endTime: the time of event to be finished.
   + index of father.
   + index of left
   + index of right
*/

void splay(int e//, unsigned long arr[20250][3]
                  );

void removeFirst(int * first, int * root
                  //, unsigned long arr[20250][3]
                  );

void show(//unsigned long arr[20250][3], 
               int root);
void leaf(//unsigned long arr[20250][3], 
               int root, enum Side side);

void add(int type, int idElementInGroup,
                int portID, 
                unsigned long endTime,
                int *root//,
                //unsigned long arr[20250][3]
                )
{
   /* Quy ước các event từ A đến G (tức type = 0..4) sẽ là 
      các event xảy ra trên hosts
      Các biến type < 0 sẽ là event xảy ra trên các edge switch
      Các biến type có bit cuối cùng là 1 sẽ là các event xảy ra trên Core switch
      Các biến type có bit cuối cùng là 0 sẽ là các event xảy ra trên Agg switch
   */
   int idNewNode = 0;
   if(type == A || type == B || type == C || type == H_HOST || type == G)
   {
      idNewNode = idElementInGroup*3 + type;//Nhan 3 vi hien tai moi chi co 3 loai su kien A, B, C
   }
   else if(type < 0)//Is event of edge switch
   {
      int positiveType = -type;
      //type *= -1;
      idNewNode = 16*5 + idElementInGroup*((4/2)*4 + 3*(4/2));
      if(portID < 4/2)
      {
         idNewNode += portID * 3 + (positiveType - D);
      } // + type;
      else{
         idNewNode += (4/2) * 3 + (portID - 4/2) * 4 + (positiveType - D);
      }
   }  
   else{
      int isCoreSwitch = type & 1;
      type = type >> 1;
      idNewNode = 16*5 + (4*4/2)*((4/2)*4 + 3*(4/2)) 
                     + idElementInGroup*4*4 + portID*4 + (type - D);
   }

   data[idNewNode][0] = type;
   data[idNewNode][1] = idElementInGroup;
   data[idNewNode][2] = portID;
   arr[idNewNode][0] = endTime;
   int formerFather = (int)arr[idNewNode][1]; // & 0x7fffffff;
   if(formerFather != -1)
   { 
      if(arr[formerFather][2] >> 32 == idNewNode)
      {
         arr[formerFather][2] |= (unsigned long)0x7fffffff << 32;
      }
      else if(arr[formerFather][2] & 0x7fffffff == idNewNode)
      {
         arr[formerFather][2] |= 0x7fffffff;
      }
   }
   arr[idNewNode][1] = 0x7fffffff;
   arr[idNewNode][2] = ((unsigned long)0x7fffffff << 32) | 0x7fffffff;

   if(*root == -1)
   {
      *root = idNewNode;
      return;
   }
   int left = idNewNode, right = idNewNode, temp = -1;
   int end_splay = 0;
   int t = *root;
   while (end_splay == 0) {
      if(endTime > arr[t][0])//if(endTime > t->endTime)
      {
         temp = (int)arr[t][2];//temp = t->right;
         if(temp == __INT32_MAX__)//if(temp == NULL)
         {
            //cas "zig"
            arr[left][2] &= ((unsigned long)0x7fffffff << 32); 
            arr[left][2] |= t; 
            //left->right = t;
            //arr[t][1] &= ((unsigned long)0x7fffffff << 32);
            arr[t][1] = left;
            //t->father = left;
            arr[right][2] |= ((unsigned long)0x7fffffff << 32);
            //right->left = NULL;
            end_splay = 1;
         }
         else if(endTime < arr[temp][0])//if(endTime < temp->endTime)
         {
            //cas "zig-zag" simplifie
            arr[left][2] &= ((unsigned long)0x7fffffff << 32);
            arr[left][2] |= t;
            //left->right = t;
            //arr[t][1] &= ((unsigned long)0x7fffffff << 32);
            arr[t][1] = left;
            //t->father = left;
            left = t;//left = t;
            t = temp;//t = temp;
         }
         else {
            //cas "zig-zig"
            arr[t][2] &= ((unsigned long)0x7fffffff << 32);
            arr[t][2] |= arr[temp][2] >> 32; 
            //t->right = temp->left;
            if(arr[temp][2] >> 32 != __INT32_MAX__)
            //if (temp->left != NULL)
            {
               int x = arr[temp][2] >> 32; //x = temp->left
               arr[x][1] &= ((unsigned long)0x7fffffff << 32);
               arr[x][1] |= t; 
               //x->father = t;
            }
            arr[left][2] &= ((unsigned long)0x7fffffff << 32);
            arr[left][2] |= temp;
            //left->right = temp;
            arr[temp][1] &= ((unsigned long)0x7fffffff << 32);
            arr[temp][1] |= left;
            //temp->father = left;
            arr[temp][2] &= 0x7fffffff;
            arr[temp][2] |= (unsigned long)t << 32;
            //temp->left = t;
            arr[t][1] &= ((unsigned long)0x7fffffff << 32);
            arr[t][1] |= temp;
            //t->father = temp;
            left = temp;
            t = (int)arr[temp][2];//t = temp->right;
            if(t == __INT32_MAX__){//if (t == NULL) {
               arr[right][2] |= ((unsigned long)0x7fffffff << 32);
               //right->left = NULL;
               end_splay = 1;
            }
         }
      }else {
         temp = arr[t][2] >> 32; //temp = t->left;
         if(temp == __INT32_MAX__){//if (temp == NULL) {
            //cas "zig"
            arr[right][2] &= 0x7fffffff;
            arr[right][2] |= ((unsigned long)t << 32); 
            //right->left = t;
            arr[t][1] &= ((unsigned long)0x7fffffff << 32);
            arr[t][1] |= right; 
            //t->father = right;
            arr[left][2] |= 0x7fffffff;     
            //left->right = NULL;
            end_splay = 1;
         }
         else if (endTime > arr[temp][0]) {//if (endTime > temp->endTime) {
            //cas "zig-zag" simplifie
            arr[right][2] &= 0x7fffffff;
            arr[right][2] |= ((unsigned long)t << 32); 
            //right->left = t;
            arr[t][1] &= ((unsigned long)0x7fffffff << 32);
            arr[t][1] |= right;
            //t->father = right;
            right = t;//right = t;
            t = temp;//t = temp;
         }
         else {
            //cas "zig-zig"
            arr[t][2] &= 0x7fffffff;
            arr[t][2] |= ((unsigned long)arr[temp][2] << 32); 
            //t->left = temp->right;
            if (((int)arr[temp][2]) != __INT32_MAX__){
            //if (temp->right != NULL)
               int x = (int)arr[temp][2];//x = temp->right
               arr[x][1] &= ((unsigned long)0x7fffffff << 32);
               arr[x][1] |= t; 
               //temp->right->father = t;
            }
            arr[right][2] &= 0x7fffffff;
            arr[right][2] |= ((unsigned long)temp << 32); 
            //right->left = temp;
            arr[temp][1] &= ((unsigned long)0x7fffffff << 32);
            arr[temp][1] |= right; 
            //temp->father = right;
            arr[temp][2] &= ((unsigned long)0x7fffffff << 32);
            arr[temp][2] |= t; 
            //temp->right = t;
            arr[t][1] &= ((unsigned long)0x7fffffff << 32);
            arr[t][1] |= temp; 
            //t->father = temp;
            right = temp;         //right = temp;
            t = (int)(arr[temp][2] >> 32); //t = temp->left;
            if(t == __INT32_MAX__) {//if (t == NULL) {
               arr[left][6] = -1; //left->right = NULL;
               end_splay = 1;
            }
         }
      }
   }

   temp = arr[idNewNode][5]; //temp = newNode->left;
   arr[idNewNode][5] = arr[idNewNode][6]; //newNode->left = newNode->right;
   arr[idNewNode][6] = temp; //newNode->right = temp;

   //return newNode;
   *root = idNewNode;

}

void splay(int e//, unsigned long arr[20250][3]
               )
{
   int left;
   int f;   // Tree * f;
   int gf;  // Tree * gf;
   int ggf; // Tree * ggf;
   while(arr[e][4] != -1)//while(e->father != NULL)
   {
      f = arr[e][4];  //  f = e->father;
      gf = arr[f][4]; //  gf = f->father;
      left = (e == arr[f][5] ? 1 : 0);  //left = (e == f->left ? 1 : 0);
      if(left)
      {
         // cas du fils gauche
         if (gf == -1) {//if (gf == NULL) {
            // cas "zig", on fait la rotation de f (la racine) et e
            arr[f][4] = e; //f->father = e;
            arr[f][5] = arr[e][6]; //f->left = e->right;
            if(arr[f][5] != -1) //if(f->left != NULL)
               arr[arr[f][5]][4] = f; //f->left->father = f;
            arr[e][6] = f; //e->right = f;
            arr[e][4] = -1; //e->father = NULL;
         }
         else if(arr[gf][6] == f) { //if (gf->right == f) {
            // cas "zig-zag", simplifie, pareil que le cas "zig"
            arr[gf][6] = e; //gf->right = e;

            arr[f][4] = e;  //f->father = e;
            arr[f][5] = arr[e][6]; //f->left = e->right;
            if(arr[f][5] != -1) //if(f->left != NULL)
               arr[arr[f][5]][4] = f; //f->left->father = f;
            arr[e][6] = f; //e->right = f;
            arr[e][4] = gf; //e->father = gf;
         }
         else {
            // cas "zig-zig", on fait la rotation de gf avec
            // f, suivis de la rotation de e avec f
            ggf = arr[gf][4]; //ggf = gf->father;

            arr[gf][5] = arr[f][6]; //gf->left = f->right;
            if(arr[gf][5] != -1)//if(gf->left != NULL)
               arr[arr[gf][5]][4] = gf; //gf->left->father = gf;
            arr[f][6] = gf; //f->right = gf;
            arr[gf][4] = f; //gf->father = f;

            arr[f][5] = arr[e][6]; //f->left = e->right;
            if(arr[f][5] != -1) //if(f->left != NULL)
               arr[ arr[f][5] ][4] = f; //f->left->father = f;
            arr[f][4] = e; //f->father = e;
            arr[e][6] = f; //e->right = f;

            // on rattache e a son nouveau pere
            arr[e][4] = ggf; //e->father = ggf;
            if(ggf != -1) //if(ggf != NULL)
               if(arr[ggf][5] == gf) //if(ggf->left == gf)
                  arr[ggf][5] = e; //ggf->left = e;
               else
                  arr[ggf][6] = e; //ggf->right = e;
         }
      }
      else
      {
         //cas du fils droit
         if(gf == -1){ //if(gf == NULL) {
            // cas "zig", on fait la rotation de f (la racine) et e

            arr[f][4] = e; //f->father = e;
            arr[f][6] = arr[e][5]; //f->right = e->left;
            if(arr[f][6] != -1) //if(f->right != NULL)
               arr[arr[f][6]][4] = f; //f->right->father = f;
            arr[e][5] = f; //e->left = f;
            arr[e][4] = -1; //e->father = NULL;
         }
         else if( arr[gf][5] == f) {//if(gf->left == f) {
            // cas "zig-zag", simplifie, pareil que le cas "zig"
            arr[gf][5] = e; //gf->left = e;

            arr[f][4] = e; //f->father = e;
            arr[f][6] = arr[e][5]; //f->right = e->left;
            if(arr[f][6] != -1)//if(f->right != NULL)
               arr[arr[f][6]][4] = f;//f->right->father = f;
            arr[e][5] = f; //e->left = f;
            arr[e][4] = gf; //e->father = gf;
         }
         else {
            // cas "zig-zig", on fait la rotation de gf avec
            // f, suivis de la rotation de e avec f
            ggf = arr[gf][4]; //ggf = gf->father;

            arr[gf][6] = arr[f][5]; //gf->right = f->left;
            if( arr[gf][6] != -1 ) //if(gf->right != NULL)
               arr[arr[gf][6]][4] = gf; //gf->right->father = gf;
            arr[f][5] = gf; //f->left = gf;
            arr[gf][4] = f; //gf->father = f;

            arr[f][6] = arr[e][5]; //f->right = e->left;
            if( arr[f][6] != -1 )//if(f->right != NULL)
               arr[ arr[f][6] ][4] = f; //f->right->father = f;
            arr[f][4] = e; //f->father = e;
            arr[e][5] = f; //e->left = f;

            // on rattache e a son nouveau pere
            arr[e][4] = ggf; //e->father = ggf;
            if( ggf != -1 )//if(ggf != NULL)
               if(arr[ggf][5] == gf) //if(ggf->left == gf)
                  arr[ggf][5] = e; //ggf->left = e;
               else
                  arr[ggf][6] = e; //ggf->right = e;
         }
      }
   }
}

void removeFirst(int * first, int * root
                  //, unsigned long arr[20250][3]
                  )
{
   int t = *root;
   if(t == -1)
   {  
      *first = -1; 
      return;//return NULL;
   }
   int temp = t;
   while(arr[temp][5] != -1)//while(temp->left != NULL)
   {
      temp = arr[temp][5]; //temp = temp->left;
   }
   *first = temp;
   
   splay(*first//, arr
                  );
   while(arr[t][4] != -1)
   {
      t = arr[t][4];
   }

   int leftTree, rightTree;
   leftTree = arr[t][5];//leftTree = t->left;
   if(leftTree != -1)//if(leftTree != NULL)
   {
      arr[leftTree][4] = -1;//leftTree->father = NULL;
   }
   rightTree = arr[t][6]; //rightTree = t->right;
   if(rightTree != -1) //if(rightTree != NULL)
      arr[rightTree][4] = -1; //rightTree->father = NULL;
   int removedFather = 0;
   if(temp == arr[t][4])//if(temp == t->father)
   {
        removedFather = 1;
   }  
   if(rightTree == -1 && 
            leftTree == -1 && 
               removedFather == 1
               //if(rightTree == NULL && leftTree == NULL && removedFather == 1
            )
   {  
      *first = t; return; //return t;
   }
   if(rightTree == -1)//if(rightTree == NULL)
   {     
      t = leftTree; //t = leftTree;
   }
   else if(leftTree == -1) //if(leftTree == NULL)
   {
      t = rightTree; //t = rightTree;
   }
   else 
   {
      int newRoot = rightTree; //Tree * newRoot = rightTree;
      while(arr[newRoot][5] != -1) //while(newRoot->left != NULL)
      {
         newRoot = arr[rightTree][5]; //newRoot = rightTree->left;
      }

      //splay(newRoot, arr);
      splay(newRoot);
      while(arr[newRoot][4] != -1)
      {
         newRoot = arr[newRoot][4];
      }

      arr[newRoot][5] = leftTree; //newRoot->left = leftTree;
      arr[leftTree][4] = newRoot; //leftTree->father = newRoot;
      //return newRoot;
      *first = newRoot;
      return;
   }

   *root = t; //return t;
   return;
   
}

void show(//unsigned long arr[20250][3], 
            int root)
{
   if(root != -1 && arr[root][3] != -1)
   {
      printf("\n===========> for event type = %ld at end = %ld in %ld\n", 
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

void leaf(//unsigned long arr[20250][3], 
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
      printf("for event type = %ld at end = %ld in %ld. It's index = %d\n", 
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

void validate(//unsigned long arr[20250][3], 
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
         printf("Wrong content as arr[%d][5] = %ld and arr[%d][6] = %ld while it's child(index = %d) has father: %ld\n"
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
