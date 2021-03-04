#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Node.c"
#define RIGHT_MASK 0x7fffffff
#define LEFT_MASK ((unsigned long)0x7fffffff << 32)
#define MASK_INT 65535

unsigned long *data; //[384];
unsigned int **arr; //[384][5];
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

void putIntoQueue(unsigned long endTime, unsigned int idNewNode);
void insert(int type, int idElementInGroup,
                int portID, 
                unsigned long endTime,
                unsigned int idNewNode);
int removeSoonestEvent();
unsigned long newwidth();
void resize(unsigned long newsize);
void localInit(unsigned long nbuck, unsigned long bwidth, 
                                    unsigned long startprio);
void initqueue();
void enqueue(int type, int idElementInGroup,
                int portID, 
                unsigned long endTime,
                unsigned int idNewNode);
int dequeue();
void printBuckets();

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

void putIntoQueue(unsigned long endTime, unsigned int idNewNode){
    int current, next, comp;
    unsigned long priority = endTime;

    // i la vi tri bucket ma entry chen vao
    int i;
    i = priority / width;
    i = i % nbuckets;
    

    int bucket_index = arr[i][2];
    // chen i vao vi tri hop li tren buckets[i]
    //if(buckets[i] == NULL || buckets[i]->endTime >= priority){
    int atTop = (bucket_index == -1);
    if(!atTop){
        comp = compare(priority, arr[bucket_index][0], arr[bucket_index][1]);
        if(comp <= 0)
            atTop = 1;
    }

    if(atTop){
        arr[idNewNode][3] = bucket_index;
        //entry->next = buckets[i];
        arr[idNewNode][2] = idNewNode;
        //buckets[i] = entry;
    } else {
        current = arr[bucket_index][2];
        //Node* current = buckets[i];
        //while(current->next != NULL ){
        while(arr[current][3] != -1 ){
            next = arr[current][3];
            comp = compare(priority, arr[next][0], arr[next][1]);
            if(comp > 0){
            //if(current->next->endTime < priority)
                current = next;
                //current = current->next;
            }
            else break;
        }

        arr[idNewNode][3] = arr[current][3];
        //entry->next = current->next;
        arr[current][3] = idNewNode;
        //current->next = entry;
    }

    if(priority < lastprio){
        double n = priority / width;
        buckettop = (n+1)*width + 0.5*width;
    }

    // cap nhat qsize : so event cua hang doi
    qsize++;

    return;
}

void insert(int type, int idElementInGroup,
                int portID, 
                unsigned long endTime,
                unsigned int idNewNode){
    data[idNewNode] = ((unsigned long)idElementInGroup << 32)
                           | ((portID) & MASK_INT) << 16 
                           | (type & MASK_INT);

    arr[idNewNode][0] = (unsigned int)(endTime >> 32);
    arr[idNewNode][1] = (unsigned int)(endTime);
    
    putIntoQueue(endTime, idNewNode);
}

int removeSoonestEvent(){
    unsigned long i;
    
    if(qsize == 0) return -1;

    i = lastbucket;
    int bucket_index;
    int comp, tmp;
    unsigned long ulBuckettop;
    int inThisBucket;
    while(1){
        bucket_index = arr[i][2];
        ulBuckettop = (unsigned long)buckettop;
        //if(buckets[i] != NULL && buckets[i]->endTime < buckettop){
        inThisBucket = (bucket_index != -1);
        if(!inThisBucket){
            comp = compare(ulBuckettop, arr[bucket_index][0], arr[bucket_index][1]);
            if(comp > 0)
                inThisBucket = 1;
        }
        if(inThisBucket){
            tmp = bucket_index;
            //Node* tmp = buckets[i];
            arr[i][2] = arr[tmp][3];
            //buckets[i] = tmp->next;

            lastbucket = i;
            lastprio = ((unsigned long)arr[tmp][0] << 32) + arr[tmp][1];
            //lastprio = tmp->endTime;
            qsize--;

            return tmp;
        } else {
            i++; 
            if(i == nbuckets){ 
                i = 0;
            }
            buckettop += width;
            if(i == lastbucket){
                break;
            }
        }
    }

    // neu khong tim thay gia tri nho nhat trong nam
    // quay lai tim gia tri nho nhat trong tat ca cac gia tri dau cua buckets
    unsigned long minbucket;
    unsigned long minpri;

    // start : vi tri dau tien buckets[i] != NULL
    unsigned long start;
    for(start = 0; start < nbuckets; start++)
        //if(buckets[start] != NULL){
        if(arr[start][2] != -1){
            lastbucket = start;
            lastprio = ((unsigned long)arr[start][0] << 32) + arr[start][1];
            //lastprio = buckets[start]->endTime;
            minpri = lastprio;
            //minpri = buckets[start]->endTime;
            minbucket = start;
            break;
        }

    
    // tim vi tri buckets[i] != NULL ma nho nhat
    for(i = start + 1; i < nbuckets; i++){
        bucket_index = arr[i][2];
        //if(buckets[i] != NULL){
        if(bucket_index != -1){
            comp = compare(minpri, arr[bucket_index][0], arr[bucket_index][1]);
            //if(buckets[i]->endTime < minpri){
            if(comp > 0){
                lastbucket = i;
                lastprio = ((unsigned long)arr[bucket_index][0] << 32) + arr[bucket_index][1];
                //lastprio = buckets[i]->endTime;
                minpri = lastprio;
                //minpri = buckets[i]->endTime;
                minbucket = i;
            }
        }
    }

    int foo = arr[minbucket][2];
    //Node* foo = buckets[minbucket];
    arr[minbucket][2] = arr[foo][3];
    //buckets[minbucket] = foo->next;

    int n = lastprio / width;
    buckettop = (n+1) * width + 0.5*width;
    qsize--;

    return foo;
}

void resize(unsigned long newsize){
    unsigned long i;
    unsigned long bwidth;
    unsigned int foo;
    
    unsigned long oldnbuckets;
    unsigned int *oldbuckets = malloc(nbuckets * sizeof(unsigned int));
    //Node** oldbuckets;

    if(!resizeenable) return;

    bwidth = newwidth();
    for(i = 0; i < width; i++){
        oldbuckets[i] = arr[i][2];
    }
    //oldbuckets = buckets;
    oldnbuckets = nbuckets;

    localInit(newsize, bwidth, lastprio);

    unsigned int id; unsigned long endTime;
    // them lai cac phan tu vao calendar moi
    for(i = 0; i < oldnbuckets; i++){
        foo = oldbuckets[i];
        //Node* foo = oldbuckets[i];
        while(foo != -1){ // tranh viec lap vo han
        //while(foo != NULL){
            id = data[foo] >> 32;
            endTime = ((unsigned long)arr[foo][0] << 32) + arr[foo][1];
            //Node* tmp = new_node(foo->type,foo->idElementInGroup,
            //                               foo->portID,foo->endTime);
            putIntoQueue(endTime, id);
            //insert(tmp);

            foo = arr[foo][3];
            //foo = foo->next;
        }
    }

    return;
}

void localInit(unsigned long nbuck, unsigned long bwidth, unsigned long startprio){
    unsigned long i;
    double n;

    width = bwidth;
    nbuckets = nbuck;

    // khoi tao cac bucket
    qsize = 0;

    // khoi tao cac chi so ban dau cua bucket dau tien
    lastprio = startprio;
    n = startprio / width;
    lastbucket = ((unsigned long)n) % nbuckets;
    buckettop = (n+1)*width + 0.5*width;

    // khoi tao 2 linh canh dau vao cuoi
    bot_threshold = nbuckets/2 - 2;
    top_threshold = 2 * nbuckets;
}

void initqueue(){
    localInit(2,1,0.0);
    resizeenable = 1;
}