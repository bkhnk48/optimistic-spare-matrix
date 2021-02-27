#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "Event.c"

#ifndef _TYPES_OF_NODE_
#define _TYPES_OF_NODE_

typedef struct node {
    enum TypesOfEvent type; //type of event
    
    int idElementInGroup;//id of element in group of hosts or switches
    int portID;
    unsigned long endTime;
    struct node * next;
} Node;

Node *Events;

Node* new_node(int type, int idElementInGroup,
                int portID, 
                unsigned long endTime) {
  Node *n = (Node *)malloc(sizeof(Node));
  if (n == NULL) {
    return NULL;
  }
  n->type = type;
  n->idElementInGroup = idElementInGroup;
  n->portID = portID;
  n->endTime = endTime;
  n->next = NULL;

  return n;
}
#endif