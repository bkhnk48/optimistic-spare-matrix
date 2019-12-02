struct QUEUE{
    int id;
    int hostID;
    int dstID;
    struct QUEUE *next;
};

typedef struct QUEUE *Queue;
