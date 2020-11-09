#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Node.c"
#include "LinkedList.c"
#include <limits.h>

#ifndef _TYPES_OF_CAQUEUE_
#define _TYPES_OF_CAQUEUE_
enum BOOLEAN{FALSE, TRUE};


struct calendar_queue{
    //int firstSub;
    List *bucket;
    unsigned long bucket_top;
    int width;
    int nbuckets;
    int qsize;
    int last_bucket;
    unsigned long last_prio;
    int top_threshold;
    int bot_threshold;
    int resize_enabled;
};

typedef struct calendar_queue CalendarQueue;

void put(Node* entry, CalendarQueue* q);
Node* dequeue(CalendarQueue* q);
void enqueue(Node* entry, //unsigned long priority,
                    CalendarQueue* q);
void resize(CalendarQueue* q, int newsize);
int swap(int *type, int idElementInGroup,
                int *portID,
                unsigned long *endTime,
                int haveEmptySlot,
                int *idNewNode,
                CalendarQueue* q);

int mask_modulo(int m, int n) { return m & (n - 1);}

void localInit(CalendarQueue* q, int qbase, int nBuck, int bWidth, int start_prio){
    int i;
    unsigned long n;

    // khoi tao cac tham so
    //q->firstSub = qbase;
    q->bucket = (Node**) calloc(nBuck,sizeof(Node));
    q->width = bWidth;
    q->nbuckets = nBuck;

    // khoi tao cac bucket
    q->qsize = 0;
    for(i=0; i < q->nbuckets; i++){
        q->bucket[i] = NULL;
    }

    // khoi tao cac chi so ban dau cua bucket dau tien
    q->last_prio = start_prio;
    n = start_prio / bWidth;
    q->last_bucket = n % nBuck;
    q->bucket_top = (n+1)*q->width + 0.5*q->width;

    // khoi tao 2 linh canh dau vao cuoi
    q->bot_threshold = q->nbuckets/2 - 2;
    q->top_threshold = 2*q->nbuckets;
}

CalendarQueue* initqueue() {
    CalendarQueue* init_q = (CalendarQueue*)malloc(sizeof(CalendarQueue));
	localInit(init_q, 0, 2, 1, 0);
    init_q->resize_enabled = TRUE;
	return init_q;
}

int newwidth(CalendarQueue* q) {
    /* This calculates the width to use for buckets. */
    int nsamples;
    /* Decide how many queue elements to sample. */
    if(q->qsize < 2) return (1);
    if(q->qsize <= 5)
        nsamples = q->qsize;
    else
        nsamples = 5 + q->qsize/10;
    if(nsamples > 25) nsamples = 25;
    int lastprio = q->last_prio;
    int lastbucket = q->last_bucket;
    int buckettop = q->bucket_top;

    q->resize_enabled = FALSE;
    List* dqsamples = makelist();
    unsigned long * samplesprio
         = (unsigned long*)malloc(sizeof(unsigned long) * nsamples);

    for(int i = 0; i < nsamples; ++i) {
        Node* n = dequeue(q);
        attach(n, dqsamples);
        samplesprio[i] = n->endTime;
    }

    for(int i = 0; i < nsamples; ++i) {
        Node* n = delete(*dqsamples, dqsamples);
        enqueue(n, q);
    }
    destroy(dqsamples);

    q->last_prio = lastprio;
    q->last_bucket = lastbucket;
    q->bucket_top = buckettop;
    q->resize_enabled = TRUE;

    double sum = 0;
    for(int i = 0; i < nsamples - 1; ++i) {
        sum += (samplesprio[i + 1] - samplesprio[i]);
    }
    double avg_seperation = sum / (nsamples - 1);
    int m = 0;
    sum = 0;
    for(int i = 0; i < nsamples - 1; ++i) {
		double spr = samplesprio[i + 1] - samplesprio[i];
        if(spr < 2*avg_seperation) {
            sum += spr;
            ++m;
        }
    }
    double final_seperation = (sum == 0)? (1.0 / 3) : sum / m;
    free(samplesprio);
    final_seperation *= 3;
    return (int)final_seperation;
}

void put(Node* entry, CalendarQueue* q)
{
    unsigned long priority = entry->endTime;

    // i la vi tri bucket ma entry chen vao
    int i;
    i = priority / q->width;
    i = i % q->nbuckets;

    // tren i vao vi tri hop li tren buckets[i]
    if(q->bucket[i] == NULL || q->bucket[i]->endTime >= priority){
        entry->next = q->bucket[i];
        q->bucket[i] = entry;
    } else {
        Node* current = q->bucket[i];
        while(current->next != NULL ){
            if(current->next->endTime < priority)
                current = current->next;
            else break;
        }

        entry->next = current->next;
        current->next = entry;
    }

    if(priority < q->last_prio){
        int n = priority / q->width;
        q->bucket_top = (n+1)*q->width + 0.5*q->width;
    }

    // cap nhat qsize : so event cua hang doi
    q->qsize++;

    return;
}
void enqueue(Node* entry, /*unsigned long priority,*/ CalendarQueue* q)
/* This adds one entry to the queue. */
{
    register unsigned long i;
    /* Calculate the number of the bucket in which to
    place the new entry. */
    i = //priority
        entry->endTime
            / q->width; /* Find virtual bucket.*/
    i = i % q->nbuckets; /* Find actual bucket. */
    attach(entry, &(q->bucket[i])); /*Insert entry into bucket i in sorted list. */
    ++(q->qsize); /* Update record of queue size. */
    /* Double the calendar size if needed. */
    if (q->qsize > q->top_threshold) resize(q, 2 * q->nbuckets);
}

Node* dequeue(CalendarQueue* q)
/* This removes the lowest priority node from the
queue and returns a pointer to the node containing
it. */
{
    register int i;
    if (q->qsize == 0) return(NULL);
    for (i = q->last_bucket; ; ) /* Search buckets */
    {
        /* Check bucket i */
        if (q->bucket[i] != NULL && q->bucket[i]->endTime < q->bucket_top)
        {   /* Item to dequeue has been found. */
            Node* dequeue_i = delete(q->bucket[i], &(q->bucket[i]));
            /* Update position on calendar. */
            q->last_bucket = i; q->last_prio = dequeue_i->endTime;
            --(q->qsize);
            /* Halve calendar size if needed. */
            if (q->qsize < q->bot_threshold) resize(q, q->nbuckets/2);
            return dequeue_i;
        }
        else{/* Prepare to check next bucket or else go to a direct search. */
            ++i;
            if(i == q->nbuckets) i = 0;
            q->bucket_top += q->width;
            if(i == q->last_bucket) break; /* Go to direct search */
        }
    }
    /* Directly search for minimum priority event. */
	int firstprio = TRUE;
    int lowestprio = 0;
    int lowestbucket = 0;
    for(int i = 0; i < q->nbuckets; ++i) {
		if (q->bucket[i] == NULL) continue;
		if(firstprio) {
			lowestbucket = i;
			lowestprio = q->bucket[i]->endTime;
			firstprio = FALSE;
		}
		if (q->bucket[i]->endTime < lowestprio) {
			lowestbucket = i;
			lowestprio = q->bucket[i]->endTime;
		}
	}
    q->last_bucket = lowestbucket;
    q->last_prio = lowestprio;
    int vbucket = (int)(lowestprio / (q->width));
    q->bucket_top = (vbucket + 1) * q->width + 0.5 * q->width;
    return(dequeue(q)); /* Resume search at minnode. */
}

void resize(CalendarQueue* q, int newsize)
/* This copies the queue onto a calendar with newsize
buckets. The new bucket array is on the opposite
end of the array a[QSPACE] from the original. */
{
    double bwidth;
    int i;
    int oldnbuckets;
    List * oldbucket;
    if (q->resize_enabled == FALSE) return;
    bwidth = newwidth(q); /* Find new bucket width. */
    /* Save location and size of old calendar for use
    when copying calendar. */
    oldbucket = q->bucket; oldnbuckets = q->nbuckets;
    /* Initialize new calendar. */
    localInit(q, 0, newsize, bwidth, q->last_prio);
	q->resize_enabled = TRUE;
    /* Copy queue elements to new calendar. */
	for (i = oldnbuckets - 1; i >= 0; --i) {
		Node* current = oldbucket[i];
		while (current != NULL) {
			Node* next = current->next;
			enqueue(current, q);
			current = next;
		}
		//Transfer elements from bucket i to new calendar
		//by enqueueing them;
	}
}


int swap(int *type, int idElementInGroup,
                int *portID,
                unsigned long *endTime,
                int haveEmptySlot,
                int *idNewNode,
                CalendarQueue* q){
    //leftBound:   1 vs 0
    //middle:      3 vs 0
    //rightBound:  5 vs 0
    if(haveEmptySlot == 0)
    {
        if(q == NULL)  return 0;
        enqueue(new_node(*type,
                            idElementInGroup, *portID, *endTime), q);
        return 0;
    }
    else{
        if(q == NULL)  return 1;
        if(q->qsize == 0)
        {
            return 1;
        }
        else{
            if(q->last_prio >= *endTime)//neu tham so can them be hon hoac bang phan
            //tu min cua calendar queue
            {
                return 1;
            }
            else{
                enqueue(new_node(*type,
                            idElementInGroup, *portID, *endTime), q);
                Node* temp = dequeue(q);
                *type = temp->type;
                *portID = temp->portID;
                *endTime = temp->endTime;
                free(temp);

                return 1;
            }
        }
    }

    return 1;
}


void raw_add(unsigned long type, unsigned long idElementInGroup,
                unsigned long portID,
                unsigned long endTime,
                unsigned long idNewNode,
                int *root,
                unsigned long arr[20250][7]){
   arr[idNewNode][0] = type;
   arr[idNewNode][1] = idElementInGroup;
   arr[idNewNode][2] = portID;
   arr[idNewNode][3] = endTime;
   int formerFather = arr[idNewNode][4];
   if(formerFather != -1)
   {
      if(arr[formerFather][5] == idNewNode)
      {
         arr[formerFather][5] = -1;
      }
      else if(arr[formerFather][6] == idNewNode)
      {
         arr[formerFather][6] = -1;
      }
   }
   arr[idNewNode][4] = -1;
   arr[idNewNode][5] = -1;
   arr[idNewNode][6] = -1;

   if(*root == -1)
   {
      *root = idNewNode;
      return;
   }
   int left = idNewNode, right = idNewNode, temp = -1;
   int end_splay = 0;
   int t = *root;
   while (end_splay == 0) {
      if(endTime > arr[t][3])//if(endTime > t->endTime)
      {
         temp = arr[t][6];//temp = t->right;
         if(temp == -1)//if(temp == NULL)
         {
            //cas "zig"
            arr[left][6] = t; //left->right = t;
            arr[t][4] = left; //t->father = left;
            arr[right][5] = -1;//right->left = NULL;
            end_splay = 1;
         }
         else if(endTime < arr[temp][3])//if(endTime < temp->endTime)
         {
            //cas "zig-zag" simplifie
            arr[left][6] = t;//left->right = t;
            arr[t][4] = left;//t->father = left;
            left = t;//left = t;
            t = temp;//t = temp;
         }
         else {
            //cas "zig-zig"
            arr[t][6] = arr[temp][5]; //t->right = temp->left;
            if(arr[temp][5] != -1)//if (temp->left != NULL)
               arr[  arr[temp][5]  ][  4  ] = t; //temp->left->father = t;
            arr[left][6] = temp;//left->right = temp;
            arr[temp][4] = left;//temp->father = left;
            arr[temp][5] = t;//temp->left = t;
            arr[t][4] = temp;//t->father = temp;
            left = temp;
            t = arr[temp][6];//t = temp->right;
            if(t == -1){//if (t == NULL) {
               arr[right][5] = -1;//right->left = NULL;
               end_splay = 1;
            }
         }
      }else {
         temp = arr[t][5]; //temp = t->left;
         if(temp == -1){//if (temp == NULL) {
            //cas "zig"
            arr[right][5] = t; //right->left = t;
            arr[t][4] = right; //t->father = right;
            arr[left][6] = -1;     //left->right = NULL;
            end_splay = 1;
         }
         else if (endTime > arr[temp][3]) {//if (endTime > temp->endTime) {
            //cas "zig-zag" simplifie
            arr[right][5] = t; //right->left = t;
            arr[t][4] = right; //t->father = right;
            right = t;//right = t;
            t = temp;//t = temp;
         }
         else {
            //cas "zig-zig"
            arr[t][5] = arr[temp][6]; //t->left = temp->right;
            if (arr[temp][6] != -1)//if (temp->right != NULL)
               arr[  arr[temp][6]  ][4] = t; //temp->right->father = t;
            arr[right][5] = temp; //right->left = temp;
            arr[temp][4] = right; //temp->father = right;
            arr[temp][6] = t; //temp->right = t;
            arr[t][4] = temp; //t->father = temp;
            right = temp;         //right = temp;
            t = arr[temp][5]; //t = temp->left;
            if(t == -1) {//if (t == NULL) {
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


void push(CalendarQueue* q, unsigned long arr[20250][7], unsigned long idElementInGroup, int *root){
    int leftBound = (arr[idElementInGroup*3][4]
                     * arr[idElementInGroup*3][5]
                     * arr[idElementInGroup*3][6] == -1 ? 1 : 0);

   int middle = (arr[idElementInGroup*3 + 1][4]
                     * arr[idElementInGroup*3 + 1][5]
                     * arr[idElementInGroup*3 + 1][6] == -1 ? 3 : 0);

   int rightBound = (arr[idElementInGroup*3 + 2][4]
                     * arr[idElementInGroup*3 + 2][5]
                     * arr[idElementInGroup*3 + 2][6] == -1 ? 5 : 0);
   int x = leftBound + rightBound + middle;
   Node* temp;
   unsigned long type, portID, endTime, idNewNode;
   switch(x)
   {
      case 9://Ca ba o A, B, C deu trong
      case 1://A
      case 4://A, B
      case 6://A, C
         idNewNode = idElementInGroup*3 + A;
         temp = dequeue(q);
         type = temp->type;
         portID = temp->portID;
         endTime = temp->endTime;
         free(temp);
         break;
      case 3://B
      case 8://B, C
         idNewNode = idElementInGroup*3 + B;
         temp = dequeue(q);
         type = temp->type;
         portID = temp->portID;
         endTime = temp->endTime;
         free(temp);
         break;
      case 5://C
         idNewNode = idElementInGroup*3 + C;
         temp = dequeue(q);
         type = temp->type;
         portID = temp->portID;
         endTime = temp->endTime;
         free(temp);
         break;
      case 0://Khong co o nao trong ca

         break;
   }

   if(x != 0)
        raw_add(type, idElementInGroup, portID, endTime, idNewNode, root, arr);
   return;
}

#endif
