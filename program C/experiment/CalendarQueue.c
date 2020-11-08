#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Node.c"

void insert(Node* entry);
Node* removeSoonestEvent();
double newwidth();
void resize(int newsize);
void localInit(int nbuck, double bwidth, double startprio);
void initqueue();
void enqueue(Node* entry);
Node* dequeue();
void printBucket(Node* n);
void printBuckets();

//node** a;
Node** buckets;
double width;
int nbuckets;
int firstsub;
int resizeenable;
int qsize;
double lastprio;
int lastbucket;
double buckettop;
int bot_threshold;
int top_threshold;

void insert(Node* entry){
    double priority = entry->endTime;

    // i la vi tri bucket ma entry chen vao
    int i;
    i = priority / width;
    i = i % nbuckets;

    // tren i vao vi tri hop li tren buckets[i]
    if(buckets[i] == NULL || buckets[i]->endTime >= priority){
        entry->next = buckets[i];
        buckets[i] = entry;
    } else {
        Node* current = buckets[i];
        while(current->next != NULL ){
            if(current->next->endTime < priority)
                current = current->next;
            else break;
        }

        entry->next = current->next;
        current->next = entry;
    }

    if(priority < lastprio){
        int n = priority / width;
        buckettop = (n+1)*width + 0.5*width;
    }

    // cap nhat qsize : so event cua hang doi
    qsize++;

    return;
}

Node* removeSoonestEvent(){
    int i;
    if(qsize == 0) return NULL;

    i = lastbucket;
    while(1){
        if(buckets[i] != NULL && buckets[i]->endTime < buckettop){
            Node* tmp = buckets[i];
            buckets[i] = tmp->next;

            lastbucket = i;
            lastprio = tmp->endTime;
            qsize--;

            return tmp;
        } else {
            i++; if(i==nbuckets) i=0;
            buckettop += width;
            if(i == lastbucket) break;
        }
    }

    // neu khong tim thay gia tri nho nhat trong nam
    // quay lai tim gia tri nho nhat trong tat ca cac gia tri dau cua buckets
    int minbucket;
    double minpri;

    // start : vi tri dau tien buckets[i] != NULL
    int start;
    for(start=0; start<nbuckets; start++)
        if(buckets[start] != NULL){
            lastbucket = start;
            lastprio = buckets[start]->endTime;
            minpri = buckets[start]->endTime;
            minbucket = start;
            break;
        }

    // tim vi tri buckets[i] != NULL ma nho nhat
    for(int i = start+1; i<nbuckets; i++)
        if(buckets[i] != NULL){
            if(buckets[i]->endTime < minpri){
                lastbucket = i;
                lastprio = buckets[i]->endTime;
                minpri = buckets[i]->endTime;
                minbucket = i;
            }
        }

    Node* foo = buckets[minbucket];
    buckets[minbucket] = foo->next;

    int n = lastprio / width;
    buckettop = (n+1) * width + 0.5*width;
    qsize--;

    return foo;
}

double newwidth(){
    int nsamples;

    if(qsize < 2) return 1.0;
    if(qsize <= 5)
        nsamples = qsize;
    else
        nsamples = 5 + qsize/10;

    if(nsamples > 25) nsamples = 25;

    double oldlastprio = lastprio;
    int oldlastbucket = lastbucket;
    double oldbuckkettop = buckettop;


    // lay ra nsamples gia tri mau
    // luc lay ra mau ngan chan viec resize, resizeenable = false
    resizeenable = 0;
    Node* save = (Node*) calloc(nsamples,sizeof(Node));
    for(int i=0; i<nsamples; i++){
        Node* tmp = removeSoonestEvent();
        save[i] = *tmp;
    }
    resizeenable = 1;

    //  tra lai cac gia tri da lay ra trong hang doi
    for(int i=0; i<nsamples; i++){
        insert(&save[i]);
    }
    lastprio = oldlastprio;
    lastbucket = oldlastbucket;
    buckettop = oldbuckkettop;

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

    return totalSeparation;
}

void resize(int newsize){
    double bwidth;
    int i;
    int oldnbuckets;
    Node** oldbuckets;

    if(!resizeenable) return;

    bwidth = newwidth();
    oldbuckets = buckets;
    oldnbuckets = nbuckets;

    localInit(newsize,bwidth,lastprio);

    // them lai cac phan tu vao calendar moi
    for(int i=0; i<oldnbuckets; i++){
        Node* foo = oldbuckets[i];
        while(foo!=NULL){ // tranh viec lap vo han
            Node* tmp = new_node(foo->type,foo->idElementInGroup,
                                            foo->portID,foo->endTime);
            insert(tmp);
            foo = foo->next;
        }
    }

    return;
}

void localInit(int nbuck, double bwidth, double startprio){
    int i;
    long int n;

    // khoi tao cac tham so
    buckets = (Node**) calloc(nbuck,sizeof(Node));
    width = bwidth;
    nbuckets = nbuck;

    // khoi tao cac bucket
    qsize = 0;
    for(int i=0; i<nbuckets; i++){
        buckets[i] = NULL;
    }

    // khoi tao cac chi so ban dau cua bucket dau tien
    lastprio = startprio;
    n = startprio / width;
    lastbucket = n % nbuckets;
    buckettop = (n+1)*width + 0.5*width;

    // khoi tao 2 linh canh dau vao cuoi
    bot_threshold = nbuckets/2 - 2;
    top_threshold = 2*nbuckets;
}

void initqueue(){
    localInit(2,1,0.0);
    resizeenable = 1;
}

// enqueue
void enqueue(Node* entry){
    insert(entry);
    // nhan doi so luong calendar neu can
    if(qsize>top_threshold) resize(2*nbuckets);
}

// dequeue
Node* dequeue(){
    Node* tmp = removeSoonestEvent();

    /*thu hep so luong cua calendar neu can*/
    if(qsize < bot_threshold) resize(nbuckets/2);
    return tmp;
}

/*in ra man hinh lich*/
void printBucket(Node* n){
    while(n!=NULL){
        printf("%ld ",n->endTime);
        n = n->next;
    }
    return;
}
void printBuckets(){
    for(int i=0; i<nbuckets; i++){
        printf("Day %d : ",i);
        Node* tmp = buckets[i];
        printBucket(tmp);
        printf("\n");
    }
    printf("\nCount of event : %d\n",qsize);
    printf("so luong bucket : %d\n",nbuckets);
    printf("buckettop : %.1f\n",buckettop);
    printf("lastbuckket : %d\n",lastbucket);
    printf("lastprio : %.1f\n",lastprio);
    printf("width : %.1f\n",width);
    printf("bot : %.1d\n",bot_threshold);
    printf("top : %.1d",top_threshold);
}
