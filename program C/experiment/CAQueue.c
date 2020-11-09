#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Node.c"
#include "LinkedList.c"
#include <limits.h>

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

void push(CalendarQueue* q, unsigned long arr[20250][7], unsigned long idElementInGroup ){

}
