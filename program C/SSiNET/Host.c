#include<stdio.h>
#include "Init.c"


int CYCLE_PACKET = 100;

int getDestination(int idOfPacket)
{
    return 0;
}

void show(int **Hosts, int numOfHosts)
{
    int i = 0, j = 0;
    for(i = 0; i < numOfHosts; i++)
    {
        printf("\tHost[%d] dai dien cho node: %d, ket noi ben ngoai qua Link %d",
                    i, Hosts[i][14], Hosts[i][12]
                );
        if(Hosts[i][1] == -1 && Hosts[i][2] == -1)
        {
            //printf("Hosts[%d] khong co goi tin nao ca", Hosts[i][14]);
        }
        if(Hosts[i][9] == -1 ||  Hosts[i][10] == -1)
        {
            //printf(", khong co su kien (h) nao ca");
        }
        if(Hosts[i][11] == -1 ||  Hosts[i][12] == -1)
        {
            //printf(", khong co su kien (c) nao ca");
        }
        if(Hosts[i][13] == 0)
        {
            //printf(", khong co packet nao duoc gui den ca");
        }
        printf("\n");
    }

}

int getHostID(int index)
{
    if(index >= 0 && index <= 3)
    {
        return index;
    }
    if(index >= 4 && index <= 7)
    {
        return ((index - 4) + 8);
    }
    if(index >= 8 && index <= 11)
    {
        return ((index - 8) + 16);
    }
    if(index >= 12 && index <= 15)
    {
        return ((index - 12) + 24);
    }
    return 0;
}

/*
This data structure respresents the host
struct HOST{
    int BUFFER_SIZE: BUFFER_SIZE
    int linkID: ID of link from host itself
    int hostID: id cua host (trong danh sach cac nut cua toan mang)

    int **allEvents;
                   //allEvents[0]: credit count
                   //allEvents[1]: id of the sent packet
                   //allEvents[2]: id of received packet in next switch
                   //allEvents[3]: time to execute event (I)
                   //allEvents[4]: time to execute event (B)
                   //allEvents[5]: time to execute event (C)
    int **outport; //an array with size = 2 x BUFFER_SIZE
        //outport[0][i]: all ids of packets in outport
        //outport[1][i]: all destinations of packets in outport
};
 */
void assignHosts(Host *Hosts, int *IsHost, int numOfHosts, int BUFFER_SIZE)
{
    int i = 0, j = 0;
    for(i = 0; i < numOfHosts; i++)
    {
        Hosts[i] = malloc(sizeof(struct HOST));
        int *allEvents = NULL;
        allEvents = malloc(sizeof * allEvents * 7);
        //luu tru credit: so slot con trong o inport cua switch ke tiep
        allEvents[0] = BUFFER_SIZE; 
        //luu tru id cua packet vua duoc gui di
        allEvents[1] = -1;
        //luu tru id cua packet vua duoc cai next switch nhan. La packet gan lien voi su kien (I)
        allEvents[2] = -1;
        //luu tru thoi gian su kien (I) ket thuc
        allEvents[3] = -1;
        //luu tru thoi gian su kien (B) ket thuc
        allEvents[4] = -1;
        //luu tru thoi gian su kien (C) ket thuc
        allEvents[5] = -1;
        //luu tru so luong cac goi tin co trong outport
        allEvents[6] = 0;

        Hosts[i]->allEvents = allEvents;

        Hosts[i]->BUFFER_SIZE = BUFFER_SIZE;

        int *outport = NULL;
        outport = malloc(sizeof * outport * BUFFER_SIZE);
        for(j = 0; j < BUFFER_SIZE; j++)
        {
            outport[j] = -1;
        }

        int *dstIDs = NULL;
        dstIDs = malloc(sizeof * dstIDs * BUFFER_SIZE);
        for(j = 0; j < BUFFER_SIZE; j++)
        {
            dstIDs[j] = -1;
        }


        Hosts[i]->outport = outport;
        Hosts[i]->dstIDs = dstIDs;
      
        //ID of link from it:
        Hosts[i]->linkID = -1;

        //# of packets received in current interval
        //Hosts[i][13] = 0; 

        //id cua host (trong danh sach cac nut cua toan mang)
        Hosts[i]->hostID = getHostID(i);
        IsHost[Hosts[i]->hostID] = 1;

        //interval hien tai
        //Hosts[i][15] = 0;
    }
}

void assignLinkField(Host *Hosts, int **Links, int *IsHost, int numOfLinks, int Bandwidth)
{
    int i, j; //int isHostAtDst = 0;
    int isHostAtSrc = 0, index = 0, count = 0; 
    for(i = 0; i < numOfLinks; i++)
    {
        int idSrcNode = Links[i][0];
        int idDstNode = Links[i][1];
        Links[i][2] = -1;//Khong co goi tin tren duong truyen
        //printf("\tidSrcNode = %d ", idSrcNode);
        isHostAtSrc = IsHost[idSrcNode];
        Links[i][9] = Bandwidth;
        Links[i][10] = IsHost[idSrcNode];
        Links[i][11] = IsHost[idDstNode];
        count += isHostAtSrc;
        //printf("isHost = %d ", isHost);
        index = index*(1 - isHostAtSrc) + isHostAtSrc*(count - isHostAtSrc);
        //printf("index = %d\n", index);
        Hosts[index]->linkID = Hosts[index]->linkID*(1 - isHostAtSrc) + isHostAtSrc*i;
    }
}


void showHosts(Graph graph)
{
    //show(graph->Hosts, graph->numOfHosts);
}

void setSourceQueue(Queue **sourceQueue, int numOfHosts)
{
    int i, j;
    for(i = 0; i < numOfHosts; i++)
    {
        Queue first = NULL;
        first = malloc(sizeof(Queue));
        first->id = -1;
        first->dstID = -1;
        first->hostID = -1;
        Queue last = NULL;
        last = malloc(sizeof(Queue));
        last->id = -1;
        last->dstID = -1;
        last->hostID = -1;
        first->next = last;
        last->next = NULL;
        sourceQueue[i][0] = first;
        sourceQueue[i][1] = last;
    }
}