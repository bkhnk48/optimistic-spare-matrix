#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "Node.c"
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

unsigned long save[25];
unsigned int indexes[25];

void putIntoQueue(unsigned long endTime, unsigned int idNewNode);
void insert(unsigned long endTime, unsigned int idNewNode);
unsigned int removeSoonestEvent();
unsigned long newwidth();
void resize(unsigned long newsize);
void localInit(unsigned long nbuck, unsigned long bwidth, unsigned long startprio);
void initqueue();
void enqueue(unsigned long endTime, unsigned int idNewNode);
unsigned int dequeue();
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
        arr[i][2] = idNewNode;
        //buckets[i] = entry;
    } else {
        current = bucket_index; //arr[bucket_index][2];
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

void insert(unsigned long endTime, unsigned int idNewNode){
    arr[idNewNode][0] = (unsigned int)(endTime >> 32);
    arr[idNewNode][1] = (unsigned int)(endTime);
    
    putIntoQueue(endTime, idNewNode);
}

unsigned int removeSoonestEvent(){
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
        if(inThisBucket){
            comp = compare(ulBuckettop, arr[bucket_index][0], arr[bucket_index][1]);
            inThisBucket = (comp > 0);
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

unsigned long newwidth(){
    unsigned long nsamples;
    int i;

    if(qsize < 2) 
        return 1;
    if(qsize <= 5)
        nsamples = qsize;
    else
        nsamples = 5 + qsize/10;

    if(nsamples > 25) 
        nsamples = 25;

    unsigned long oldlastprio = lastprio;
    unsigned long oldlastbucket = lastbucket;
    double oldbuckkettop = buckettop;

    // lay ra nsamples gia tri mau
    // luc lay ra mau ngan chan viec resize, resizeenable = false
    resizeenable = 0;
    //unsigned long *save = malloc(nsamples * sizeof(unsigned long));
    for(i = 0; i < nsamples; i++){
        save[i] = 0;
    }
    //Node* save = (Node*) calloc(nsamples,sizeof(Node));
    unsigned int tmp;
    for(i=0; i < nsamples; i++){
        //Node* tmp = removeSoonestEvent();
        tmp = removeSoonestEvent();
        save[i] = ((unsigned long)arr[tmp][0] << 32) + arr[tmp][1];
        indexes[i] = tmp;
        //save[i] = *tmp;
    }
    resizeenable = 1;

    //  tra lai cac gia tri da lay ra trong hang doi
    for(i = 0; i < nsamples; i++){
        putIntoQueue(save[i], indexes[i]);
        //insert(&save[i]);
    }
    lastprio = oldlastprio;
    lastbucket = oldlastbucket;
    buckettop = oldbuckkettop;

    // tinh toan gia tri cho new witdh
    unsigned long totalSeparation = 0;
    //double totalSeparation = 0;
    int end = nsamples;
    int cur = 0;
    int next = cur + 1;
    while(next != end){
        totalSeparation += save[next] - save[cur];
        //totalSeparation += save[next].endTime - save[cur].endTime;
        cur++;
        next++;
    }
    double twiceAvg = (double)totalSeparation / (nsamples - 1) * 2;

    totalSeparation = 0;
    end = nsamples;
    cur = 0;
    next = cur + 1;
    unsigned long diff;
    while(next != end){
        diff = save[next] - save[cur];
        //double diff = save[next].endTime - save[cur].endTime;
        if(diff <= twiceAvg){
            totalSeparation += diff;
        }
        cur++;
        next++;
    }

    // gia tri width moi = 3 lan do phan tach gia tri trung binh
    totalSeparation *= 3;
    totalSeparation = totalSeparation < 1.0 ? 1.0 : totalSeparation;

    return totalSeparation;
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
    for(i = 0; i < nbuckets; i++){
        arr[i][2] = -1;
    }

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

// enqueue
void enqueue(unsigned long endTime, unsigned int idNewNode){
    insert(endTime, idNewNode);
    // nhan doi so luong calendar neu can
    if(qsize > top_threshold) 
        resize(2 * nbuckets);
}

// dequeue
unsigned int dequeue(){
    unsigned int tmp = removeSoonestEvent();

    /*thu hep so luong cua calendar neu can*/
    if(qsize < bot_threshold && nbuckets >= 2) 
        resize(nbuckets/2);
    return tmp;
}

/*in ra man hinh lich*/
void printBucket(unsigned int n){
    while(n != -1){
        printf("%ld ", ((unsigned long)arr[n][0] << 32) + arr[n][1]);
        n = arr[n][3];
    }
    return;
}

void printBuckets(){
    unsigned int i;
    for(i = 0; i < nbuckets; i++){
        printf("Day %d : ",i);
        unsigned int tmp = arr[i][2];
        printBucket(tmp);
        printf("\n");
    }
    printf("\nCount of event : %ld\n",qsize);
    printf("so luong bucket : %ld\n",nbuckets);
    printf("buckettop : %f\n",buckettop);
    printf("lastbuckket : %ld\n",lastbucket);
    printf("lastprio : %ld\n",lastprio);
    printf("width : %d\n",width);
    printf("bot : %ld\n",bot_threshold);
    printf("top : %ld\n",top_threshold);
}