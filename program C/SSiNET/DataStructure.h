//This data structure represents the source queue in each host
struct QUEUE{
    int id;
    int hostID;
    int dstID;
    struct QUEUE *next;
};

typedef struct QUEUE *Queue;

//This data structure respresents the host
struct HOST{
    int BUFFER_SIZE;
    int linkID;
    int hostID;

    int *allEvents;
    int **outport;
};

typedef struct HOST *Host;