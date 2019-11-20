#include<stdio.h>

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
        if(Hosts[i][1] == -1 && Hosts[i][2] == -1)
        {
            printf("Hosts[%d] khong co goi tin nao ca", Hosts[i][14]);
        }
        if(Hosts[i][9] == -1 ||  Hosts[i][10] == -1)
        {
            printf(", khong co su kien (h) nao ca");
        }
        if(Hosts[i][11] == -1 ||  Hosts[i][12] == -1)
        {
            printf(", khong co su kien (c) nao ca");
        }
        if(Hosts[i][13] == 0)
        {
            printf(", khong co packet nao duoc gui den ca");
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

void assignHosts(int **Hosts, int *IsHost, int numOfHosts)
{
    int i = 0, j = 0;
    for(i = 0; i < numOfHosts; i++)
    {
        //Moi Hosts[i]
      //co thong tin ve chu ky tao goi tin,
      Hosts[i][0] = CYCLE_PACKET;
      //id be nhat cua goi tin con trong SourceQueue
      Hosts[i][1] = -1;
      //id lon nhat cua goi tin con trong SourceQueue
      Hosts[i][2] = -1;
      //5 ids cua 5 goi tin trong outport (-1 neu nhu trong tai cho trong do cua outport ko chua goi tin nao ca)
      for(j = 0; j < 5; j++)
      {
        Hosts[i][3 + j] = -1;
      }
      //id cua nut dich
      Hosts[i][8] = -1;
      //event (I): thoi diem ket thuc (thoi diem sinh ra + CREDIT_DELAY)
      Hosts[i][9] = -1;  //Hosts[i][10] = -1;
      //event (C) cho gui goi tin tu outport cua Host len LINK: thoi diem ket thuc (thoi diem sinh ra + RETRY_TIME)
      Hosts[i][10] = -1;
      //Hosts[i][11] = -1;  Hosts[i][12] = -1;
      
      //event (B) cho gui goi tin tu source queue den outport cua host: thoi diem sinh ra, thoi diem ket thuc (thoi diem sinh ra)
      Hosts[i][11] = -1;

      //ID of link from it:
      Hosts[i][12] = -1;

      //# of packets received in current interval
      Hosts[i][13] = 0; 

      //id cua host (trong danh sach cac nut cua toan mang)
      Hosts[i][14] = getHostID(i);
      IsHost[Hosts[i][14]] = 1;

      //interval hien tai
      Hosts[i][15] = 0;
    }
}

