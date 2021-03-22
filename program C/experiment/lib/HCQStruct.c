#include "Node.c"

#ifndef _TYPES_OF_HCQSTRUCT_
#define _TYPES_OF_HCQSTRUCT_

typedef struct PureCalendarQueue{
    Node** buckets;
    int width;
    unsigned long nbuckets;
    //int firstsub;
    int resizeenable;
    unsigned long qsize;
    unsigned long lastprio;
    unsigned long lastbucket;
    double buckettop;
    unsigned long bot_threshold;
    unsigned long top_threshold; 
    Node* soonest;
} CQStruct;

void insert(Node* entry, CQStruct *CQ);
Node* removeSoonestEvent(CQStruct *CQ);
unsigned long newwidth(CQStruct *CQ);
void resize(unsigned long newsize, CQStruct *CQ);
void localInit(unsigned long nbuck, unsigned long bwidth, unsigned long startprio, CQStruct *CQ);
void initqueue(CQStruct *CQ);
void enqueue(Node* entry, CQStruct *CQ);
Node* dequeue(CQStruct *CQ);
void printBucket(Node* n, CQStruct *CQ);
void printBuckets(CQStruct *CQ);

void insert(Node* entry, CQStruct *CQ){
    
    unsigned long priority = entry->endTime;

    // i la vi tri bucket ma entry chen vao
    int i;
    i = priority / CQ->width;
    i = i % (CQ->nbuckets);

    // chen i vao vi tri hop li tren buckets[i]
    if(CQ->buckets[i] == NULL || CQ->buckets[i]->endTime >= priority){
        entry->next = CQ->buckets[i];
        CQ->buckets[i] = entry;
    } else {
        Node* current = CQ->buckets[i];
        while(current->next != NULL ){
            if(current->next->endTime < priority)
                current = current->next;
            else break;
        }

        entry->next = current->next;
        current->next = entry;
    }

    if(priority < CQ->lastprio){
        double n = priority / CQ->width;
        CQ->buckettop = (n+1)*(CQ->width) + 0.5*(CQ->width);
    }

    // cap nhat qsize : so event cua hang doi
    CQ->qsize++;

    return;
}

Node* removeSoonestEvent(CQStruct *CQ){
    unsigned long i;
    if(CQ->qsize == 0) return NULL;

    i = CQ->lastbucket;
    while(1){
        if(CQ->buckets[i] != NULL && CQ->buckets[i]->endTime < CQ->buckettop){
            Node* tmp = CQ->buckets[i];
            CQ->buckets[i] = tmp->next;

            CQ->lastbucket = i;
            CQ->lastprio = tmp->endTime;
            CQ->qsize--;

            return tmp;
        } else {
            i++; if(i == CQ->nbuckets) i=0;
            CQ->buckettop += CQ->width;
            if(i == CQ->lastbucket) break;
        }
    }

    // neu khong tim thay gia tri nho nhat trong nam
    // quay lai tim gia tri nho nhat trong tat ca cac gia tri dau cua buckets
    unsigned long minbucket;
    unsigned long minpri;

    // start : vi tri dau tien buckets[i] != NULL
    unsigned long start;
    for(start = 0; start < CQ->nbuckets; start++)
        if(CQ->buckets[start] != NULL){
            CQ->lastbucket = start;
            CQ->lastprio = CQ->buckets[start]->endTime;
            minpri = CQ->buckets[start]->endTime;
            minbucket = start;
            break;
        }

    
    // tim vi tri buckets[i] != NULL ma nho nhat
    for(i = start+1; i < CQ->nbuckets; i++)
        if(CQ->buckets[i] != NULL){
            if(CQ->buckets[i]->endTime < minpri){
                CQ->lastbucket = i;
                CQ->lastprio = CQ->buckets[i]->endTime;
                minpri = CQ->buckets[i]->endTime;
                minbucket = i;
            }
        }

    Node* foo = CQ->buckets[minbucket];
    CQ->buckets[minbucket] = foo->next;

    double n = CQ->lastprio / CQ->width;
    CQ->buckettop = (n+1) * (CQ->width) + 0.5*(CQ->width);
    (CQ->qsize)--;

    return foo;
}

unsigned long newwidth(CQStruct *CQ){
    unsigned long nsamples;

    if(CQ->qsize < 2) return 1;
    if(CQ->qsize <= 5)
        nsamples = CQ->qsize;
    else
        nsamples = 5 + CQ->qsize/10;

    if(nsamples > 25) nsamples = 25;

    unsigned long oldlastprio = CQ->lastprio;
    unsigned long oldlastbucket = CQ->lastbucket;
    double oldbuckkettop = CQ->buckettop;


    // lay ra nsamples gia tri mau
    // luc lay ra mau ngan chan viec resize, resizeenable = false
    CQ->resizeenable = 0;
    Node* save = (Node*) calloc(nsamples,sizeof(Node));
    unsigned long i;
    for(i=0; i < nsamples; i++){
        Node* tmp = removeSoonestEvent(CQ);
        save[i] = *tmp;
    }
    CQ->resizeenable = 1;

    //  tra lai cac gia tri da lay ra trong hang doi
    for(i=0; i<nsamples; i++){
        insert(&save[i], CQ);
    }
    CQ->lastprio = oldlastprio;
    CQ->lastbucket = oldlastbucket;
    CQ->buckettop = oldbuckkettop;

    // tinh toan gia tri cho new witdh
    double totalSeparation = 0;
    int end = nsamples;
    int cur = 0;
    int next = cur + 1;
    while(next != end){
        totalSeparation += save[next].endTime - save[cur].endTime;
        cur++;
        next++;
    }
    double twiceAvg = totalSeparation / (nsamples - 1) * 2;

    totalSeparation = 0;
    end = nsamples;
    cur = 0;
    next = cur + 1;
    while(next != end){
        double diff = save[next].endTime - save[cur].endTime;
        if(diff <= twiceAvg){
            totalSeparation += diff;
        }
        cur++;
        next++;
    }

    // gia tri width moi = 3 lan do phan tach gia tri trung binh
    totalSeparation *= 3;
    totalSeparation = totalSeparation < 1.0 ? 1.0 : totalSeparation;

    return (unsigned long)totalSeparation;
}

void resize(unsigned long newsize, CQStruct *CQ){
    unsigned long bwidth;
    
    unsigned long oldnbuckets;
    Node** oldbuckets;

    if(!CQ->resizeenable) return;

    bwidth = newwidth(CQ);
    oldbuckets = CQ->buckets;
    oldnbuckets = CQ->nbuckets;

    localInit(newsize, bwidth, CQ->lastprio, CQ);

    unsigned long i;
    // them lai cac phan tu vao calendar moi
    for(i = 0; i < oldnbuckets; i++){
        Node* foo = oldbuckets[i];
        while(foo != NULL){ // tranh viec lap vo han
            Node* tmp = new_node(foo->type,foo->idElementInGroup,
                                            foo->portID,foo->endTime);
            insert(tmp, CQ);
            foo = foo->next;
        }
    }
    free(oldbuckets);
    return;
}

void localInit(unsigned long nbuck, unsigned long bwidth, unsigned long startprio, CQStruct *CQ){
    unsigned long i;
    double n;

    // khoi tao cac tham so
    CQ->buckets = (Node**) calloc(nbuck,sizeof(Node));
    CQ->width = bwidth;
    CQ->nbuckets = nbuck;

    // khoi tao cac bucket
    CQ->qsize = 0;
    for(i = 0; i < CQ->nbuckets; i++){
        CQ->buckets[i] = NULL;
    }

    // khoi tao cac chi so ban dau cua bucket dau tien
    CQ->lastprio = startprio;
    n = startprio / (CQ->width);
    CQ->lastbucket = ((unsigned long)n) % (CQ->nbuckets);
    CQ->buckettop = (n+1)*(CQ->width) + 0.5*(CQ->width);

    // khoi tao 2 linh canh dau vao cuoi
    CQ->bot_threshold = (CQ->nbuckets)/2 - 2;
    CQ->top_threshold = 2*(CQ->nbuckets);
}

void initqueue(CQStruct *CQ){
    CQ->soonest = (Node*)malloc(sizeof(Node));
    CQ->soonest->endTime = UINT_MAX;
    CQ->soonest->idElementInGroup = -1;
    CQ->soonest->next = NULL;
    CQ->soonest->portID = -1;
    CQ->soonest->type = A;
    localInit(2,1,0.0, CQ);
    CQ->resizeenable = 1;
}

//enqueue
void enqueue(Node* entry, CQStruct *CQ){
    if(CQ->soonest->endTime == UINT_MAX){
        CQ->soonest->endTime = entry->endTime;
        CQ->soonest->idElementInGroup = entry->idElementInGroup;
        CQ->soonest->portID = entry->portID;
        CQ->soonest->type = entry->type;
        free(entry);
        return;
    }
    else if(entry->endTime < CQ->soonest->endTime){
        unsigned long tmp = CQ->soonest->endTime;
        int tmpID = CQ->soonest->idElementInGroup;
        CQ->soonest->endTime = entry->endTime;
        entry->endTime = tmp;
        CQ->soonest->idElementInGroup = entry->idElementInGroup;
        entry->idElementInGroup = tmpID;
        tmpID = CQ->soonest->portID;
        CQ->soonest->portID = entry->portID;
        entry->portID = tmpID;
        enum TypesOfEvent tmpType = CQ->soonest->type;
        CQ->soonest->type = entry->type;
        entry->type = tmpType;
    }
    insert(entry, CQ);
    // nhan doi so luong calendar neu can
    if(CQ->qsize > CQ->top_threshold) 
        resize(2*(CQ->nbuckets), CQ);
}

// dequeue
Node* dequeue(CQStruct *CQ){
    Node* soonest = NULL;
    if(CQ->soonest->endTime != UINT_MAX){
        soonest = malloc(sizeof(Node));
        soonest->endTime = CQ->soonest->endTime;
        CQ->soonest->endTime = UINT_MAX;
        soonest->idElementInGroup = CQ->soonest->idElementInGroup;
        CQ->soonest->idElementInGroup = -1;
        soonest->next = NULL;
        soonest->portID = CQ->soonest->portID;
        soonest->type = CQ->soonest->type;
    }

    if(CQ->qsize > 0){
        Node* tmp = removeSoonestEvent(CQ);
        if(tmp != NULL){
        CQ->soonest->endTime = tmp->endTime;
        CQ->soonest->idElementInGroup = tmp->idElementInGroup;
        CQ->soonest->portID = tmp->portID;
        CQ->soonest->type = tmp->type;
        free(tmp);
        }         
    }

    /*thu hep so luong cua calendar neu can*/
    if(CQ->qsize < CQ->bot_threshold && CQ->nbuckets >= 2) 
        resize(CQ->nbuckets/2, CQ);
    return soonest;
}

#endif