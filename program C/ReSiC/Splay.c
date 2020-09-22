#include <stdio.h>
#include <stdlib.h>
#include "Event.c"

enum Side{LEFT, RIGHT};

typedef struct tree_node Tree;
struct tree_node {
    enum TypesOfEvent type; //type of event
    int packetID; //id of packet
    int idLocation; //id of location where the event finished
    int startTime;
    int endTime;
    Tree * left, * right, * father;
};

Tree * add(enum TypesOfEvent type, int packetID, int idLocation,
                int startTime, 
                int endTime
                , Tree * t);
Tree * splay (int i, Tree * t);
Tree * del(int i, Tree * t);
void show(Tree * t);
void leaf(Tree * t, enum Side side);


Tree * add(enum TypesOfEvent type, int packetID, int idLocation,
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
    newNode->packetID = packetID;
    newNode->idLocation = idLocation;
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

void show(Tree * t)
{
    if(t != NULL)
    {
        printf("\n===========> for event type = %d at end = %d in %d\n", 
                t->type, t->endTime, t->idLocation);
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
                t->type, t->endTime, t->idLocation);
        leaf(t->left, LEFT);
        leaf(t->right, RIGHT);
        printf("\n");
    }
    else{
        printf("NULL\n");
    }
}
