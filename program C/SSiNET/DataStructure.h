//This data structure represents the source queue in each host
struct QUEUE{
    int id;
    int hostID;
    int dstID;
    struct QUEUE *next;
};

typedef struct QUEUE *Queue;

/*This data structure respresents the host
    struct HOST{
        int BUFFER_SIZE: BUFFER_SIZE
        int linkID: ID of link from host itself
        int hostID: id cua host (trong danh sach cac nut cua toan mang)

        int *allEvents;
                    //allEvents[0]: credit count
                    //allEvents[1]: id of the sent packet
                    //allEvents[2]: id of received packet in next switch
                    //allEvents[3]: time to execute event (I)
                    //allEvents[4]: time to execute event (B)
                    //allEvents[5]: time to execute event (C)
                    //allEvents[6]: num of packets in outport
        int *outport; //an array with size = BUFFER_SIZE
                    //outport: all ids of packets in outport
        int *dstIDs; //dstIDs: all destinations of packets in outport
    };
    */
struct HOST{
    int BUFFER_SIZE;
    int linkID;
    int hostID;

    int *allEvents;
    int *outport;
    int *dstIDs;
};

typedef struct HOST *Host;