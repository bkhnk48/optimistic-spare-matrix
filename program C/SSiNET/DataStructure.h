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
    int *basicInfo;
    int **allEvents;
};

typedef struct HOST *Host;