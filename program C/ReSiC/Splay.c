#include <stdio.h>
#include <stdlib.h>
#include "Event.c"

enum Side{LEFT, RIGHT};

typedef struct tree_node Tree;
struct tree_node {
    enum TypesOfEvent type; //type of event
    //int packetID; //id of packet
    //int idNode; //id of Node in group of hosts, switches
    int idElementInGroup;//id of element in group of hosts or switches
    int startTime;
    int endTime;
    Tree * left, * right, * father;
};

Tree * add(enum TypesOfEvent type, int packetID, int idNode,
                int idElementInGroup,
                int startTime, 
                int endTime
                , Tree * t);
void * splay (Tree * e);

Tree * removeFirstEvent(Tree * first, 
            Tree * t);

Tree * del(int i, Tree * t);
void show(Tree * t);
void leaf(Tree * t, enum Side side);

Tree * add(enum TypesOfEvent type, int packetID, int idNode,
                int idElementInGroup,
                int startTime, 
                int endTime,
                Tree * t)
{
    Tree * newNode;
      
    newNode = (Tree *) malloc (sizeof (Tree));
    if (newNode == NULL) {
      printf("Ran out of space\n");
      exit(1);
    }

    newNode->type = type;
    //newNode->packetID = packetID;
    //newNode->idNode = idNode;
    newNode->idElementInGroup = idElementInGroup;
    newNode->startTime = startTime;
    newNode->endTime = endTime;
    newNode->father = NULL;
    if (t == NULL) {
      newNode->left = newNode->right = NULL;
      newNode->father = NULL;
      return newNode;
    }

    Tree * left = newNode;
    Tree * right = newNode;
    Tree * temp = NULL;
    int end_splay = 0;
    while (end_splay == 0) {
      if(endTime > t->endTime)
      {
          temp = t->right;
          if(temp == NULL)
          {
              //cas "zig"
              left->right = t;
              t->father = left;
              right->left = NULL;
              end_splay = 1;
          }
          else if(endTime < temp->endTime)
          {
              //cas "zig-zag" simplifie
              left->right = t;
              t->father = left;
              left = t;
              t = temp;
          }
          else {
              //cas "zig-zig"
              t->right = temp->left;
              if (temp->left != NULL)
                  temp->left->father = t;
              left->right = temp;
              temp->father = left;
              temp->left = t;
              t->father = temp;
              left = temp;
              t = temp->right;
              if (t == NULL) {
                  right->left = NULL;
                  end_splay = 1;
              }
          }
      
      } else {
          temp = t->left;
          if (temp == NULL) {
              //cas "zig"
              right->left = t;
              t->father = right;
              left->right = NULL;
              end_splay = 1;
          }
          else if (endTime > temp->endTime) {
              //cas "zig-zag" simplifie
              right->left = t;
              t->father = right;
              right = t;
              t = temp;
          }
          else {
              //cas "zig-zig"
              t->left = temp->right;
              if (temp->right != NULL)
                  temp->right->father = t;
              right->left = temp;
              temp->father = right;
              temp->right = t;
              t->father = temp;
              right = temp;
              t = temp->left;
              if (t == NULL) {
                  left->right = NULL;
                  end_splay = 1;
              }
          }
      }
    }
    temp = newNode->left;
    newNode->left = newNode->right;
    newNode->right = temp;

    return newNode;
}

Tree * removeFirstEvent(Tree * first, Tree *t)
{
    if(t == NULL)
        return NULL;
    Tree * temp = t;
    while(temp->left != NULL)
    {
        temp = temp->left;
    }
    //Tree * first = (Tree *) malloc (sizeof (Tree));
    first->type = temp->type;
    //first->packetID = temp->packetID;
    first->startTime = temp->startTime;
    first->endTime = temp->endTime;
    //first->idNode = temp->idNode;
    first->idElementInGroup = temp->idElementInGroup;
    first->left = NULL;
    first->right = NULL;
    first->father = NULL;
    splay(temp);

    Tree * leftTree = (Tree *) malloc (sizeof (Tree));
    leftTree = t->left;
    if(leftTree != NULL)
        leftTree->father = NULL;
    Tree * rightTree = (Tree *) malloc (sizeof (Tree));
    rightTree = t->right;
    if(rightTree != NULL)
        rightTree->father = NULL;
    int removedFather = 0;
    if(temp == t->father)
    {
        removedFather = 1;
    }

    free(temp);

    if(rightTree == NULL && leftTree == NULL && removedFather == 1
            )
        return t;
    if(rightTree == NULL)
        t = leftTree;
    else if(leftTree == NULL)
    {
        t = rightTree;
    }
    else 
    {
        Tree * newRoot = rightTree;
        while(newRoot->left != NULL)
        {
            newRoot = rightTree->left;
        }

        splay(newRoot);
        newRoot->left = leftTree;
        leftTree->father = newRoot;
        return newRoot;
        
    }

    return t;
}

void * splay (Tree * e)
{
    int left;
    Tree * f;
    Tree * gf;
    Tree * ggf;
    while(e->father != NULL)
    {
        f = e->father;
        gf = f->father;
        left = (e == f->left ? 1 : 0);
        if(left)
        {
            // cas du fils gauche
            if (gf == NULL) {
               // cas "zig", on fait la rotation de f (la racine) et e
               f->father = e;
               f->left = e->right;
               if(f->left != NULL)
                  f->left->father = f;
               e->right = f;
               e->father = NULL;
            }
            else if (gf->right == f) {
               // cas "zig-zag", simplifie, pareil que le cas "zig"
               gf->right = e;

               f->father = e;
               f->left = e->right;
               if(f->left != NULL)
                  f->left->father = f;
               e->right = f;
               e->father = gf;
            }
            else {
               // cas "zig-zig", on fait la rotation de gf avec
               // f, suivis de la rotation de e avec f
               ggf = gf->father;

               gf->left = f->right;
               if(gf->left != NULL)
                  gf->left->father = gf;
               f->right = gf;
               gf->father = f;

               f->left = e->right;
               if(f->left != NULL)
                  f->left->father = f;
               f->father = e;
               e->right = f;

               // on rattache e a son nouveau pere
               e->father = ggf;
               if(ggf != NULL)
                  if(ggf->left == gf)
                     ggf->left = e;
                  else
                     ggf->right = e;
            }
        }else
        {
            //cas du fils droit
            if(gf == NULL) {
               // cas "zig", on fait la rotation de f (la racine) et e

               f->father = e;
               f->right = e->left;
               if(f->right != NULL)
                  f->right->father = f;
               e->left = f;
               e->father = NULL;
            }
            else if(gf->left == f) {
               // cas "zig-zag", simplifie, pareil que le cas "zig"
               gf->left = e;

               f->father = e;
               f->right = e->left;
               if(f->right != NULL)
                  f->right->father = f;
               e->left = f;
               e->father = gf;
            }
            else {
               // cas "zig-zig", on fait la rotation de gf avec
               // f, suivis de la rotation de e avec f
               ggf = gf->father;

               gf->right = f->left;
               if(gf->right != NULL)
                  gf->right->father = gf;
               f->left = gf;
               gf->father = f;

               f->right = e->left;
               if(f->right != NULL)
                  f->right->father = f;
               f->father = e;
               e->left = f;

               // on rattache e a son nouveau pere
               e->father = ggf;
               if(ggf != NULL)
                  if(ggf->left == gf)
                     ggf->left = e;
                  else
                     ggf->right = e;
            }
        }
    }
}

void show(Tree * t)
{
    if(t != NULL)
    {
        printf("\n===========> for event type = %d at end = %d in %d\n", 
                t->type, t->endTime, t->
                                        //idNode
                                        idElementInGroup
                                    );
        leaf(t->left, LEFT);
        leaf(t->right, RIGHT);
        printf("\n");
    }
    else{
        printf("===========> left NULL\n");
    }
}

void leaf(Tree * t, enum Side side)
{
    printf("===========> ");
    if(side == LEFT)
    {
        printf("left ");
    }
    else{
        printf("right ");
    }
    if(t != NULL)
    {
        printf("for event type = %d at end = %d in %d\n", 
                t->type, t->endTime, t->
                            //idNode
                            idElementInGroup
                    );
        leaf(t->left, LEFT);
        leaf(t->right, RIGHT);
        printf("\n");
    }
    else{
        printf("NULL\n");
    }
}
