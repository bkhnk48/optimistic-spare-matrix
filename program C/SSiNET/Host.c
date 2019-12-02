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
//This data structure respresents the host
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
void assignHosts(Host *Hosts, int *IsHost, int numOfHosts)
{
    int i = 0, j = 0;
    for(i = 0; i < numOfHosts; i++)
    {

      //Moi Hosts[i]
      //luu tru credit: so slot con trong o inport cua switch ke tiep
      //Hosts[i][0] = 0;
      //id be nhat cua goi tin con trong SourceQueue
      //Hosts[i][1] = -1;
      //id lon nhat cua goi tin con trong SourceQueue
      //Hosts[i][2] = -1;
      //5 ids cua 5 goi tin trong outport (-1 neu nhu trong tai cho trong do cua outport ko chua goi tin nao ca)
      for(j = 0; j < 5; j++)
      {
        //Hosts[i][3 + j] = -1;
      }
      //id cua nut dich
      //Hosts[i][8] = -1;
      //event (I): thoi diem ket thuc (thoi diem sinh ra + CREDIT_DELAY)
      //Hosts[i][9] = -1;  //Hosts[i][10] = -1;
      //event (C) cho gui goi tin tu outport cua Host len LINK: thoi diem ket thuc (thoi diem sinh ra + RETRY_TIME)
      //Hosts[i][10] = -1;
      //Hosts[i][11] = -1;  Hosts[i][12] = -1;
      
      //event (B) cho gui goi tin tu source queue den outport cua host: thoi diem sinh ra, thoi diem ket thuc (thoi diem sinh ra)
      //Hosts[i][11] = -1;

      //ID of link from it:
      //Hosts[i][12] = -1;

      //# of packets received in current interval
      //Hosts[i][13] = 0; 

      //id cua host (trong danh sach cac nut cua toan mang)
      //Hosts[i][14] = getHostID(i);
      //IsHost[Hosts[i][14]] = 1;

      //interval hien tai
      //Hosts[i][15] = 0;
    }
}

void assignLinkID(int **Hosts, int **Links, int *IsHost, int numOfLinks)
{
    int i, j; //int isHostAtDst = 0;
    int isHostAtSrc = 0, index = 0, count = 0; 
    for(i = 0; i < numOfLinks; i++)
    {
        int idSrcNode = Links[i][0];
        int idDstNode = Links[i][1];
        //printf("\tidSrcNode = %d ", idSrcNode);
        isHostAtSrc = IsHost[idSrcNode];
        Links[i][10] = IsHost[idSrcNode];
        Links[i][11] = IsHost[idDstNode];
        count += isHostAtSrc;
        //printf("isHost = %d ", isHost);
        index = index*(1 - isHostAtSrc) + isHostAtSrc*(count - isHostAtSrc);
        //printf("index = %d\n", index);
        Hosts[index][12] = Hosts[index][12]*(1 - isHostAtSrc) + isHostAtSrc*i;
    }
}


void showHosts(Graph graph)
{
    show(graph->Hosts, graph->numOfHosts);
}